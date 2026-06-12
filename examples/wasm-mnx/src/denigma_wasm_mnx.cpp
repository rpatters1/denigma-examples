// Copyright 2026 Robert G. Patterson.
// SPDX-License-Identifier: MIT

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <new>
#include <optional>
#include <sstream>
#include <span>
#include <string>

#include "denigma/formats/mnx.h"
#include "denigma/io/random_access_reader.h"

namespace {

void setOutput(std::uint8_t** outputData, std::size_t* outputSize, std::string payload)
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

int denigma_musx_to_mnx_json(const std::uint8_t* inputData,
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

        denigma::formats::mnx::Options options;
        options.common.sourceName = "browser.musx";
        options.indentSpaces = 2;

        std::ostringstream output;
        denigma::formats::mnx::MusxToMnxJsonConverter converter;
        converter.convert(reader, output, options);

        setOutput(outputData, outputSize, output.str());
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
