<!--
Copyright 2026 Robert G. Patterson.
SPDX-License-Identifier: MIT
-->

# Denigma Examples

Standalone examples for consuming Denigma libraries from external projects.

The repository currently includes `examples/wasm-mnx`, which builds a WebAssembly
wrapper around Denigma's MNX converter target, and `examples/wasm-enigmaxml`,
which extracts Finale Enigma XML in WebAssembly, and
`examples/text-expression-classifier`, which prints classification results for
each text expression definition in an Enigma XML file using `tinyxml2` for XML
parsing.

Denigma documentation is available at [rpatters1.github.io/denigma](https://rpatters1.github.io/denigma/).
The source repository is [project-attacca/denigma](https://github.com/project-attacca/denigma).

## Requirements

- CMake 3.24 or newer
- Emscripten for the WebAssembly example
- Node.js for the smoke test

By default, the WebAssembly examples use CMake `FetchContent` to fetch Denigma
from the `main` branch. Pass `DENIGMA_SOURCE_DIR` to point at a local checkout.

The examples default to C++20. Pass `-DDENIGMA_EXAMPLES_CXX_STANDARD=23` to build with a newer standard.

## Build the WASM Examples

Build the MNX example:

```sh
emcmake cmake -S . -B build-wasm \
  -DDENIGMA_EXAMPLES_BUILD_WASM_MNX=ON
cmake --build build-wasm --target denigma_wasm_mnx
```

Build the Enigma XML example:

```sh
emcmake cmake -S . -B build-wasm \
  -DDENIGMA_EXAMPLES_BUILD_WASM_ENIGMAXML=ON
cmake --build build-wasm --target denigma_wasm_enigmaxml
```

## Smoke Test

MNX:

```sh
node examples/wasm-mnx/test/smoke.mjs \
  build-wasm/examples/wasm-mnx/denigma_wasm_mnx.js \
  examples/wasm-mnx/test/data/sample.musx
```

Enigma XML:

```sh
node examples/wasm-enigmaxml/test/smoke.mjs \
  build-wasm/examples/wasm-enigmaxml/denigma_wasm_enigmaxml.js \
  examples/wasm-mnx/test/data/sample.musx
```

## CLI Example

```sh
cmake -S . -B build-cli
cmake --build build-cli --target text_expression_classifier
build-cli/examples/text-expression-classifier/text_expression_classifier \
  examples/text-expression-classifier/test/data/exps.enigmaxml
```
