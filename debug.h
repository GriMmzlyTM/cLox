#pragma once

#include "chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstructions(Chunk* chunk, int offset);