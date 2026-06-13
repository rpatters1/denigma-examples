// Copyright 2026 Robert G. Patterson.
// SPDX-License-Identifier: MIT

#include <fstream>
#include <exception>
#include <iostream>
#include <optional>
#include <iterator>
#include <string>
#include <vector>

#include "denigma/classify/dynamics.h"
#include "musx/musx.h"

#define MUSX_USE_TINYXML2
#include "musx/xml/TinyXmlImpl.h"
#undef MUSX_USE_TINYXML2

namespace {

using DocumentPtr = musx::dom::DocumentPtr;

std::optional<DocumentPtr> loadDocument(const std::string& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        std::cerr << "Unable to open " << path << '\n';
        return std::nullopt;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    try {
        return musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(buffer);
    } catch (const std::exception& ex) {
        std::cerr << "Unable to parse " << path << ": " << ex.what() << '\n';
        return std::nullopt;
    }
}

std::string toString(denigma::classify::Dynamic dynamic)
{
    const std::string text = denigma::classify::dynamicCanonicalText(dynamic);
    if (!text.empty()) {
        return text;
    }
    switch (dynamic) {
    case denigma::classify::Dynamic::None:
        return "None";
    case denigma::classify::Dynamic::Other:
        return "Other";
    default:
        return "Unknown";
    }
}

std::string readExpressionText(const musx::dom::DocumentPtr& document, const musx::dom::others::TextExpressionDef& def)
{
    if (!document) {
        return {};
    }

    const auto textCtx = def.getRawTextCtx(musx::dom::SCORE_PARTID);
    const auto text = textCtx.getText(/*trimTags*/true, musx::util::EnigmaString::AccidentalStyle::Unicode);
    if (!text.empty()) {
        return text;
    }

    return def.description;
}

} // namespace

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " <input.musx>\n";
        return 2;
    }

    const auto document = loadDocument(argv[1]);
    if (!document) {
        return 1;
    }

    const auto& documentPtr = *document;

    const auto others = documentPtr->getOthers();
    if (!others) {
        std::cerr << "No <others> pool found in " << argv[1] << '\n';
        return 1;
    }

    const auto expressions = others->getArray<musx::dom::others::TextExpressionDef>(musx::dom::SCORE_PARTID);
    for (const auto& expression : expressions) {
        if (!expression) {
            continue;
        }

        const auto classification = denigma::classify::classifyDynamic(expression);
        const std::string text = readExpressionText(documentPtr, *expression);

        std::cout << expression->getCmper()
                  << '\t' << toString(classification.dynamic)
                  << '\t' << (classification.hasAdditionalText ? "extra" : "plain")
                  << '\t' << text
                  << '\n';
    }

    return 0;
}
