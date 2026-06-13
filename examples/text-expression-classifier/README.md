<!--
Copyright 2026 Robert G. Patterson.
SPDX-License-Identifier: MIT
-->

# Text Expression Classifier Example

Builds a small CLI that reads a MUSX file, loads it with `musxdom`, and prints
the classification for each `TextExpressionDef` found in the score.

It uses `tinyxml2` as the XML DOM backend rather than PugiXML.

Each output line contains:

`cmper<TAB>text<TAB>classification<TAB>plain|extra`

The example uses CMake `FetchContent` for `tinyxml2` and Denigma. By default it
fetches Denigma from the `main` branch of the public GitHub repository.

The example defaults to C++20. Pass `-DDENIGMA_EXAMPLES_CXX_STANDARD=23` to build with a newer standard.

## Build

From the repository root:

```sh
cmake -S . -B build-cli
cmake --build build-cli --target text_expression_classifier
```

## Run

```sh
build-cli/examples/text-expression-classifier/text_expression_classifier \
  path/to/input.musx
```
