<!--
Copyright 2026 Robert G. Patterson.
SPDX-License-Identifier: MIT
-->

# Denigma Examples

Standalone examples for consuming Denigma libraries from external projects.

The first example is `examples/wasm-mnx`, which builds a WebAssembly wrapper around
Denigma's MNX converter target.

## Requirements

- CMake 3.24 or newer
- Emscripten for the WebAssembly example
- Node.js for the smoke test

By default, the example uses CMake `FetchContent` to fetch Denigma from the
`main` branch. For local development, pass `DENIGMA_SOURCE_DIR`
to use a sibling checkout instead.

The examples default to C++20. Pass `-DDENIGMA_EXAMPLES_CXX_STANDARD=23` to build with a newer standard.

## Build the WASM MNX Example

Fetch Denigma from the configured git branch:

```sh
emcmake cmake -S . -B build-wasm \
  -DDENIGMA_EXAMPLES_BUILD_WASM_MNX=ON
cmake --build build-wasm --target denigma_wasm_mnx
```

Use a local Denigma checkout instead:

```sh
emcmake cmake -S . -B build-wasm \
  -DDENIGMA_SOURCE_DIR=../denigma \
  -DDENIGMA_EXAMPLES_BUILD_WASM_MNX=ON
cmake --build build-wasm --target denigma_wasm_mnx
```

## Smoke Test

```sh
node examples/wasm-mnx/test/smoke.mjs \
  build-wasm/examples/wasm-mnx/denigma_wasm_mnx.js \
  examples/wasm-mnx/test/data/sample.musx
```
