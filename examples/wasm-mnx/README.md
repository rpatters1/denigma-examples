<!--
Copyright 2026 Robert G. Patterson.
SPDX-License-Identifier: MIT
-->

# WASM MNX Example

Builds a small WebAssembly wrapper around Denigma's `denigma::mnx` target.

The wrapper accepts a MUSX byte buffer and returns MNX JSON as a byte buffer. It
uses `denigma::BufferRandomAccessReader`, so callers control all file I/O.

By default, CMake fetches Denigma from the `main` branch. Pass
`-DDENIGMA_SOURCE_DIR=../denigma` to use a local checkout instead.

The example defaults to C++20. Pass `-DDENIGMA_EXAMPLES_CXX_STANDARD=23` to build with a newer standard.

## Build

From the `denigma-examples` repo:

```sh
emcmake cmake -S . -B build-wasm
cmake --build build-wasm --target denigma_wasm_mnx
```

With a local Denigma checkout:

```sh
emcmake cmake -S . -B build-wasm -DDENIGMA_SOURCE_DIR=../denigma
cmake --build build-wasm --target denigma_wasm_mnx
```

## Test

```sh
node examples/wasm-mnx/test/smoke.mjs \
  build-wasm/examples/wasm-mnx/denigma_wasm_mnx.js \
  examples/wasm-mnx/test/data/sample.musx
```

## Browser Demo

Serve the repository root with any static web server after building. For example:

```sh
python3 -m http.server 8000
```

Then open `http://localhost:8000/examples/wasm-mnx/web/` and select
`examples/wasm-mnx/test/data/sample.musx`, or another MUSX file, in the file
picker.
