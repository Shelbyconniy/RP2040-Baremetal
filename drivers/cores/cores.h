#include <stdint.h>

extern uint32_t _stack1_top, _stack1_bot; // 0x20041000, top -> down
extern uint32_t _vector_st;

#define STACK1_DEFAULT          &_stack1_top
#define VECTOR_TABLE_DEFAULT    &_vector_st

void core1_start(uint32_t *vector_table, uint32_t *stack_pointer, void (*func_ptr) (void));