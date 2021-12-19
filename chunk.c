#include <stdlib.h>
#include <stdio.h>

#include "chunk.h"
#include "memory.h"


void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk->lines = NULL;
	chunk->lineIndex = 0;
	chunk->lineCapacity = 0;
	initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
	if (chunk->capacity < chunk->count + 1) {
		int oldCapacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(oldCapacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
	}

	chunk->code[chunk->count] = byte;

	if (chunk->lineCapacity < chunk->lineIndex + 2) {
		int oldCapacity = chunk->lineCapacity;
		chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
		chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->lineCapacity);
	}

	if (chunk->lineIndex > 0 && line == chunk->lines[chunk->lineIndex - 2]) chunk->lines[chunk->lineIndex - 1]++;
	else {
		chunk->lines[chunk->lineIndex] = line;
		chunk->lines[chunk->lineIndex + 1] = 1;
		chunk->lineIndex += 2;
	}

	chunk->count++;
}

int getLine(Chunk* chunk, int instructionIndex) {
	int accumulatedIndex = 0;
	for (int i = 1; i < chunk->lineCapacity; i += 2) {
		if (instructionIndex >= accumulatedIndex && instructionIndex <= accumulatedIndex + chunk->lines[i]) 
			return chunk->lines[i - 1];

		accumulatedIndex += chunk->lines[i];
	}
	printf("Could not find line for instruction index %i", instructionIndex);
	exit(1);
	return 0;
}

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	FREE_ARRAY(int, chunk->lines, chunk->capacity);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value)
{
	writeValueArray(&chunk->constants, value);
	return chunk->constants.count - 1;
}
