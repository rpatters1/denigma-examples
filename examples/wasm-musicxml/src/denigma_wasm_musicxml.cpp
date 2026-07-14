// Copyright 2026 Robert G. Patterson.
// SPDX-License-Identifier: MIT

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <new>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "denigma/formats/musicxml.h"
#include "denigma/io/random_access_reader.h"

namespace {

void setOutput(std::uint8_t** outputData, std::size_t* outputSize, const std::string& payload)
{
    if (!outputData || !outputSize) {
        return;
    }

    auto* buffer = static_cast<std::uint8_t*>(::operator new(payload.size(), std::nothrow));
    if (!buffer) {
        *outputData = nullptr;
        *outputSize = 0;
        return;
    }

    std::memcpy(buffer, payload.data(), payload.size());
    *outputData = buffer;
    *outputSize = payload.size();
}

void setError(char** errorMessage, const std::string& message)
{
    if (!errorMessage) {
        return;
    }

    auto* buffer = static_cast<char*>(::operator new(message.size() + 1, std::nothrow));
    if (!buffer) {
        *errorMessage = nullptr;
        return;
    }

    std::memcpy(buffer, message.c_str(), message.size() + 1);
    *errorMessage = buffer;
}

std::string errorText(const denigma::ConversionResult& result)
{
    std::string message;
    for (const auto& diagnostic : result.diagnostics()) {
        if (diagnostic.severity == denigma::MessageSeverity::Error) {
            if (!message.empty()) {
                message += '\n';
            }
            message += diagnostic.message;
        }
    }
    if (message.empty()) {
        message = "Conversion failed.";
    }
    return message;
}

} // namespace

extern "C" {

void* denigma_malloc(std::size_t size)
{
    return ::operator new(size, std::nothrow);
}

void denigma_free(void* ptr)
{
    ::operator delete(ptr);
}

int denigma_musx_to_musicxml(const std::uint8_t* inputData,
                             std::size_t inputSize,
                             std::uint8_t** outputData,
                             std::size_t* outputSize,
                             char** errorMessage)
{
    if (outputData) {
        *outputData = nullptr;
    }
    if (outputSize) {
        *outputSize = 0;
    }
    if (errorMessage) {
        *errorMessage = nullptr;
    }

    try {
        if (!inputData && inputSize != 0) {
            setError(errorMessage, "Input buffer is null.");
            return 1;
        }

        auto bytes = std::span<const std::byte>(reinterpret_cast<const std::byte*>(inputData), inputSize);
        denigma::BufferRandomAccessReader reader(bytes);

        denigma::formats::musicxml::Options options;
        options.common.sourceName = "browser.musx";

        // The MusicXML converter can emit multiple documents (score plus parts).
        // With the default options it emits only the score; capture the first document.
        std::string score;
        bool haveScore = false;
        denigma::formats::musicxml::MusxToMusicXmlMultiOutputConverter converter;
        auto result = converter.convert(reader,
            [&](std::string_view /*suggestedName*/, std::span<const std::byte> data) {
                if (!haveScore) {
                    score.assign(reinterpret_cast<const char*>(data.data()), data.size());
                    haveScore = true;
                }
            },
            options);

        if (result.hasError()) {
            setError(errorMessage, errorText(result));
            return 1;
        }
        if (!haveScore) {
            setError(errorMessage, "Conversion produced no MusicXML output.");
            return 1;
        }

        setOutput(outputData, outputSize, score);
        if (!outputData || !*outputData) {
            setError(errorMessage, "Unable to allocate output buffer.");
            return 1;
        }

        return 0;
    } catch (const std::exception& ex) {
        setError(errorMessage, ex.what());
        return 1;
    } catch (...) {
        setError(errorMessage, "Unknown conversion error.");
        return 1;
    }
}

} // extern "C"
