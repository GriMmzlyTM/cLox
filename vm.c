#include "common.h"
#include "vm.h"
#include <stdio.h>
#include "debug.h"

VM vm;

void initVM() {
	resetStack();
}

void freeVM() {

}

void resetStack() {
	vm.stackTop = vm.stack;
}

void push(Value value)
{
	*vm.stackTop = value;
	vm.stackTop++;
}

Value pop()
{
	vm.stackTop--;
	return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.instructionPointer++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
	do { \
		double r = pop(); \
		double l = pop(); \
		push(l op r); \
	} while(false)

	while(1) {
#ifdef DEBUG_TRACE_EXECUTION
		printf("         ");
		for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
			printf("[ ");
			printValue(*slot);
			printf(" ]");
		}
		printf("\n");

		disassembleInstructions(vm.chunk, (int)(vm.instructionPointer - vm.chunk->code));
#endif

		uint8_t instruction;
		switch (instruction = READ_BYTE()) {
		case OP_ADD: BINARY_OP(+); break;
		case OP_SUBSTRACT: BINARY_OP(-); break;
		case OP_MULTIPLY: BINARY_OP(*); break;
		case OP_DIVIDE: BINARY_OP(/ ); break;
		case OP_NEGATE: push(-pop()); break;
		case OP_CONSTANT: {
			Value constant = READ_CONSTANT();
			push(constant);
			break;
		}
		case OP_RETURN: {
			printValue(pop());
			printf("\n");
			return INTERPRET_OK;
		}
		}
	}

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
	vm.chunk = chunk;
	vm.instructionPointer = vm.chunk->code;
	return run();
}