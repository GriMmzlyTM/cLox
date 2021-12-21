#pragma once

#include "chunk.h"

#define STACK_MAX 256

typedef struct {
	Chunk* chunk;
	uint8_t* instructionPointer;
	Value* stackTop;
	Value stack[STACK_MAX];
} VM;

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM();
void freeVM();
void resetStack();

void push(Value value);
Value pop();

InterpretResult interpret(Chunk* chunk);
