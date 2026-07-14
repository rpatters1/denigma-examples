<!--
Copyright 2026 Robert G. Patterson.
SPDX-License-Identifier: MIT
-->

# WASM MusicXML Example

Builds a small WebAssembly wrapper around Denigma's `denigma::musicxml` target.

The wrapper accepts a MUSX byte buffer and returns MusicXML as a byte buffer. It
uses `denigma::BufferRandomAccessReader`, so callers control all file I/O. The
MusicXML converter can emit multiple documents (score plus linked parts); this
example uses the default options, which emit only the score.

By default, CMake uses `FetchContent` to fetch Denigma from the `main` branch.
Pass `-DDENIGMA_SOURCE_DIR=/path/to/denigma` to use a local checkout instead.

The example defaults to C++20. Pass `-DDENIGMA_EXAMPLES_CXX_STANDARD=23` to build with a newer standard.

## Build

From the repository root:

```sh
emcmake cmake -S . -B build-wasm
cmake --build build-wasm --target denigma_wasm_musicxml
```

With a local Denigma checkout:

```sh
emcmake cmake -S . -B build-wasm -DDENIGMA_SOURCE_DIR=/path/to/denigma
cmake --build build-wasm --target denigma_wasm_musicxml
```

## Test

```sh
node examples/wasm-musicxml/test/smoke.mjs \
  build-wasm/examples/wasm-musicxml/denigma_wasm_musicxml.js \
  examples/wasm-musicxml/test/data/sample.musx
```

## Browser Demo

Serve the repository root with any static web server after building. For example:

```sh
python3 -m http.server 8000
```

Then open `http://localhost:8000/examples/wasm-musicxml/web/` and select
`examples/wasm-musicxml/test/data/sample.musx`, or another MUSX file, in the
file picker.
