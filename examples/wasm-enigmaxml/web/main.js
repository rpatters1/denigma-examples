// Copyright 2026 Robert G. Patterson.
// SPDX-License-Identifier: MIT

import createModule from '../../../build-wasm/examples/wasm-enigmaxml/denigma_wasm_enigmaxml.js';

const status = document.querySelector('#status');
const output = document.querySelector('#output');
const input = document.querySelector('#file');
const PREVIEW_CHAR_LIMIT = 4000;

input.disabled = true;
status.textContent = 'Loading Denigma WebAssembly...';

let Module;

try {
  Module = await createModule({
    print(text) {
      console.log(text);
    },
    printErr(text) {
      console.error(text);
    }
  });
  input.disabled = false;
  status.textContent = 'Choose a MUSX file.';
} catch (error) {
  status.textContent = `Unable to load Denigma WebAssembly: ${formatError(error)}`;
}

function formatError(error) {
  return error instanceof Error ? error.message : String(error);
}

function readPointer(ptr) {
  return Module.getValue(ptr, '*');
}

const POINTER_SIZE = 4;
const SIZE_T_TYPE = 'i32';

function readSize(ptr) {
  return Module.getValue(ptr, SIZE_T_TYPE);
}

function convert(bytes) {
  const inputPtr = Module._denigma_malloc(bytes.byteLength);
  Module.HEAPU8.set(bytes, inputPtr);

  const outputPtrPtr = Module._denigma_malloc(POINTER_SIZE);
  const outputSizePtr = Module._denigma_malloc(POINTER_SIZE);
  const errorPtrPtr = Module._denigma_malloc(POINTER_SIZE);
  Module.setValue(outputPtrPtr, 0, '*');
  Module.setValue(outputSizePtr, 0, SIZE_T_TYPE);
  Module.setValue(errorPtrPtr, 0, '*');

  try {
    const rc = Module._denigma_musx_to_enigmaxml(inputPtr, bytes.byteLength, outputPtrPtr, outputSizePtr, errorPtrPtr);
    if (rc !== 0) {
      const errorPtr = readPointer(errorPtrPtr);
      const message = errorPtr ? Module.UTF8ToString(errorPtr) : 'conversion failed';
      if (errorPtr) Module._denigma_free(errorPtr);
      throw new Error(message);
    }

    const outputPtr = readPointer(outputPtrPtr);
    const outputSize = readSize(outputSizePtr);
    const result = new TextDecoder().decode(Module.HEAPU8.subarray(outputPtr, outputPtr + outputSize));
    Module._denigma_free(outputPtr);
    return result;
  } finally {
    Module._denigma_free(inputPtr);
    Module._denigma_free(outputPtrPtr);
    Module._denigma_free(outputSizePtr);
    Module._denigma_free(errorPtrPtr);
  }
}

input.addEventListener('change', async () => {
  const file = input.files?.[0];
  if (!file) return;
  if (!Module) {
    status.textContent = 'Denigma WebAssembly is not loaded.';
    return;
  }

  status.textContent = `Extracting ${file.name}...`;
  output.textContent = '';

  try {
    const bytes = new Uint8Array(await file.arrayBuffer());
    const xml = convert(bytes);
    const preview = xml.slice(0, PREVIEW_CHAR_LIMIT);
    const suffix = xml.length > PREVIEW_CHAR_LIMIT
      ? `\n\n... truncated after ${PREVIEW_CHAR_LIMIT} of ${xml.length} characters.`
      : '';
    status.textContent = `Extracted ${file.name}: ${bytes.byteLength} MUSX bytes to ${xml.length} XML characters.`;
    output.textContent = `${preview}${suffix}`;
  } catch (error) {
    status.textContent = `Extraction failed: ${formatError(error)}`;
  }
});
