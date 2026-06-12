// Copyright 2026 Robert G. Patterson.
// SPDX-License-Identifier: MIT

import { readFile } from 'node:fs/promises';
import { resolve } from 'node:path';
import { pathToFileURL } from 'node:url';

const [, , modulePathArg, musxPathArg] = process.argv;
if (!modulePathArg || !musxPathArg) {
  console.error('usage: node smoke.mjs <denigma_wasm_mnx.js> <input.musx>');
  process.exit(2);
}

const moduleUrl = pathToFileURL(resolve(modulePathArg)).href;
const createModule = (await import(moduleUrl)).default;
const Module = await createModule();

function readPointer(ptr) {
  return Module.getValue(ptr, '*');
}

const POINTER_SIZE = 4;
const SIZE_T_TYPE = 'i32';

function readSize(ptr) {
  return Module.getValue(ptr, SIZE_T_TYPE);
}

const input = await readFile(resolve(musxPathArg));
const inputPtr = Module._denigma_malloc(input.byteLength);
if (!inputPtr) {
  throw new Error('Unable to allocate input buffer');
}
Module.HEAPU8.set(input, inputPtr);

const outputPtrPtr = Module._denigma_malloc(POINTER_SIZE);
const outputSizePtr = Module._denigma_malloc(POINTER_SIZE);
const errorPtrPtr = Module._denigma_malloc(POINTER_SIZE);
Module.setValue(outputPtrPtr, 0, '*');
Module.setValue(outputSizePtr, 0, SIZE_T_TYPE);
Module.setValue(errorPtrPtr, 0, '*');

try {
  const rc = Module._denigma_musx_to_mnx_json(inputPtr, input.byteLength, outputPtrPtr, outputSizePtr, errorPtrPtr);
  if (rc !== 0) {
    const errorPtr = readPointer(errorPtrPtr);
    const message = errorPtr ? Module.UTF8ToString(errorPtr) : 'conversion failed';
    if (errorPtr) Module._denigma_free(errorPtr);
    throw new Error(message);
  }

  const outputPtr = readPointer(outputPtrPtr);
  const outputSize = readSize(outputSizePtr);
  const output = Buffer.from(Module.HEAPU8.subarray(outputPtr, outputPtr + outputSize)).toString('utf8');
  Module._denigma_free(outputPtr);

  const parsed = JSON.parse(output);
  if (!parsed || typeof parsed !== 'object') {
    throw new Error('MNX output did not parse to an object');
  }

  console.log(`Converted ${input.byteLength} bytes to ${outputSize} bytes of MNX JSON.`);
} finally {
  Module._denigma_free(inputPtr);
  Module._denigma_free(outputPtrPtr);
  Module._denigma_free(outputSizePtr);
  Module._denigma_free(errorPtrPtr);
}
