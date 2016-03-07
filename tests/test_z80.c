/** \file */

#include <glib.h>
#include <stdlib.h>
#include "z80.h"
#include "utils.h"

// set up test fixture with a cpu object
typedef struct {
	z80 *test_cpu;
} test_fixture;

// initialize the cpu object
static void setup_cpu(test_fixture *tf, gconstpointer data) {
	tf->test_cpu = new_cpu();
}

// free the cpu object
static void teardown_cpu(test_fixture *tf, gconstpointer data) {
	free(tf->test_cpu);
}

// test the registers to ensure they were set to 0 on init
static void test_register_init(test_fixture *tf, gconstpointer data) {
	g_assert(tf->test_cpu->pc.W == 0);
	g_assert(tf->test_cpu->counter == 0);
	g_assert(tf->test_cpu->a == 0);

	// iterate over the flags bitfield and make sure all bits are unset
	for (int i = 0; i < 6; i++) {
		g_assert(!IS_SET(tf->test_cpu->flags, i));
	}

	g_assert(tf->test_cpu->bc.W == 0);
	g_assert(tf->test_cpu->de.W == 0);
	g_assert(tf->test_cpu->hl.W == 0);
	g_assert(tf->test_cpu->ix.W == 0);
	g_assert(tf->test_cpu->iy.W == 0);
	g_assert(tf->test_cpu->_a == 0);
	g_assert(tf->test_cpu->_bc.W == 0);
	g_assert(tf->test_cpu->_de.W == 0);
	g_assert(tf->test_cpu->_hl.W == 0);
	g_assert(tf->test_cpu->sp.W == 0);
}

static void test_cpu_reset(test_fixture *tf, gconstpointer data) {
	// set the program counter to non-zero and check to be sure it worked
	tf->test_cpu->pc.W = 0x1234;
	g_assert(tf->test_cpu->pc.W == 0x1234);

	// reset the cpu
	reset_cpu(tf->test_cpu);

	g_assert(tf->test_cpu->pc.W == 0);
}

static void test_load_reg8_from_ram(test_fixture *tf, gconstpointer data) {
	// create a block of memory
	uint8_t *memory = calloc(1024, sizeof(uint8_t));

	// check initial setup
	g_assert(memory[0x0123] == 0);
	g_assert(tf->test_cpu->bc.W == 0);
	g_assert(tf->test_cpu->a == 0);

	// store a value at 0x0123
	memory[0x0123] = 0xFF;
	g_assert(memory[0x0123] == 0xFF);

	// load the memory address into the bc register
	tf->test_cpu->bc.W = 0x0123;
	g_assert(tf->test_cpu->bc.W == 0x0123);

	// load 0xFF to the A register
	_load_reg8_mem_pair(&tf->test_cpu->a, &tf->test_cpu->bc, memory);
}

static void test_load_reg8_from_offset_idx(test_fixture *tf, gconstpointer data) {
	//_load_reg8_mem_idx_offset(uint8_t *reg, word *index_register, uint8_t *memory, word *pc)
	// create block of memory
	uint8_t *memory = calloc(1024, sizeof(uint8_t));

	// check initial setup
	g_assert(tf->test_cpu->a == 0);
	g_assert(tf->test_cpu->ix.W == 0);
	g_assert(memory[0x0000] == 0);
	g_assert(memory[0x0122] == 0);
	g_assert(memory[0x0123] == 0);
	g_assert(tf->test_cpu->pc.W == 0);

	// set index register
	tf->test_cpu->ix.W = 0x0122;
	g_assert(tf->test_cpu->ix.W == 0x0122);

	// put an offset into memory
	memory[0x0000] = 1;
	g_assert(memory[0x0000] == 1);

	// store a value in memory to load
	memory[0x0123] = 0xFF;
	g_assert(memory[0x0123] == 0xFF);

	// call the helper function
	_load_reg8_mem_idx_offset(&tf->test_cpu->a, &tf->test_cpu->ix, memory, &tf->test_cpu->pc);

	g_assert(tf->test_cpu->a == 0xFF);
	g_assert(tf->test_cpu->pc.W == 0x0001);
}

static void test_load_mem_offset_idx_from_reg8(test_fixture *tf, gconstpointer data) {
	//_load_mem_idx_offset_reg8(uint8_t *reg, word *index_register, uint8_t *memory, word *pc)
	// create block of memory
	uint8_t *memory = calloc(1024, sizeof(uint8_t));

	// check initial setup
	g_assert(tf->test_cpu->a == 0);
	g_assert(tf->test_cpu->ix.W == 0);
	g_assert(memory[0x0000] == 0);
	g_assert(memory[0x0122] == 0);
	g_assert(memory[0x0123] == 0);
	g_assert(tf->test_cpu->pc.W == 0);

	// set index register
	tf->test_cpu->ix.W = 0x0122;
	g_assert(tf->test_cpu->ix.W == 0x0122);

	// put an offset into memory
	memory[0x0000] = 1;
	g_assert(memory[0x0000] == 1);

	// store a value in register a to load
	tf->test_cpu->a = 0xFF;
	g_assert(tf->test_cpu->a == 0xFF);

	// call the helper function
	_load_mem_idx_offset_reg8(&tf->test_cpu->a, &tf->test_cpu->ix, memory, &tf->test_cpu->pc);

	g_assert(memory[0x0123] == 0xFF);
	g_assert(tf->test_cpu->pc.W == 0x0001);

}

int main (int argc, char *argv[]) {
	g_test_init (&argc, &argv, NULL);

	g_test_add("/z80/new_cpu()", test_fixture, NULL, setup_cpu, test_register_init, teardown_cpu);
	g_test_add("/z80/reset_cpu()", test_fixture, NULL, setup_cpu, test_cpu_reset, teardown_cpu);
	g_test_add("/z80/_load_reg8_mem_pair()", test_fixture, NULL, setup_cpu, test_load_reg8_from_ram, teardown_cpu);
	g_test_add("/z80/_load_reg8_mem_idx_offset()", test_fixture, NULL, setup_cpu, test_load_reg8_from_offset_idx, teardown_cpu);
	g_test_add("/z80/_load_mem_idx_offset_reg8()", test_fixture, NULL, setup_cpu, test_load_mem_offset_idx_from_reg8, teardown_cpu);

	return g_test_run();
}
