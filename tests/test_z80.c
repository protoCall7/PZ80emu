/** \file */

#include <glib.h>
#include <stdlib.h>
#include "z80.h"
#include "memory.h"
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

static void test_load_reg16_from_nn(test_fixture *tf, gconstpointer data) {
	uint8_t *memory = calloc(1024, sizeof(uint8_t));

	memory[0x0000] = 0xEE;
	memory[0x0001] = 0xFF;

	_load_reg16_nn(&tf->test_cpu->bc, memory, &tf->test_cpu->pc);

	g_assert(tf->test_cpu->bc.W == 0xFFEE);
}

static void test_nop(test_fixture *tf, gconstpointer data) {
	// int run(z80 *cpu, uint8_t *memory, long cycles);
	// make memory with a nop in it

	uint8_t *memory = calloc(1, sizeof(uint8_t));

	g_assert(run(tf->test_cpu, memory, 1) == 1);
	g_assert(tf->test_cpu->pc.W == 1);
}

static void test_ld_a(test_fixture *tf, gconstpointer data) {
	uint8_t memory[9] = {0x3e, 0x05, 0x7f, 0x47, 0x4f, 0x57, 0x5f, 0x67, 0x6f};

	g_assert(run(tf->test_cpu, memory, 8) == 8);
	g_assert(tf->test_cpu->pc.W == 9);
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0505);
	g_assert(tf->test_cpu->de.W == 0x0505);
	g_assert(tf->test_cpu->hl.W == 0x0505);
}

static void test_ld_b(test_fixture *tf, gconstpointer data) {
	uint8_t memory[9] = {0x06, 0x05, 0x78, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68};

	g_assert(run(tf->test_cpu, memory, 8) == 8);
	g_assert(tf->test_cpu->pc.W == 9);
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0505);
	g_assert(tf->test_cpu->de.W == 0x0505);
	g_assert(tf->test_cpu->hl.W == 0x0505);
}

static void test_ld_c(test_fixture *tf, gconstpointer data) {
	uint8_t memory[9] = {0x0e, 0x05, 0x79, 0x41, 0x49, 0x51, 0x59, 0x61, 0x69};

	g_assert(run(tf->test_cpu, memory, 8) == 8);
	g_assert(tf->test_cpu->pc.W == 9);
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0505);
	g_assert(tf->test_cpu->de.W == 0x0505);
	g_assert(tf->test_cpu->hl.W == 0x0505);
}

static void test_ld_d(test_fixture *tf, gconstpointer data) {
	uint8_t memory[9] = {0x16, 0x05, 0x7a, 0x42, 0x4a, 0x52, 0x5a, 0x62, 0x6a};

	g_assert(run(tf->test_cpu, memory, 8) == 8);
	g_assert(tf->test_cpu->pc.W == 9);
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0505);
	g_assert(tf->test_cpu->de.W == 0x0505);
	g_assert(tf->test_cpu->hl.W == 0x0505);
}

static void test_ld_e(test_fixture *tf, gconstpointer data) {
	uint8_t memory[9] = {0x1e, 0x05, 0x7b, 0x43, 0x4b, 0x53, 0x5b, 0x63, 0x6b};

	g_assert(run(tf->test_cpu, memory, 8) == 8);
	g_assert(tf->test_cpu->pc.W == 9);
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0505);
	g_assert(tf->test_cpu->de.W == 0x0505);
	g_assert(tf->test_cpu->hl.W == 0x0505);
}

static void test_ld_h(test_fixture *tf, gconstpointer data) {
	uint8_t memory[9] = {0x26, 0x05, 0x7c, 0x44, 0x4c, 0x54, 0x5c, 0x64, 0x6c};

	g_assert(run(tf->test_cpu, memory, 8) == 8);
	g_assert(tf->test_cpu->pc.W == 9);
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0505);
	g_assert(tf->test_cpu->de.W == 0x0505);
	g_assert(tf->test_cpu->hl.W == 0x0505);
}

static void test_ld_l(test_fixture *tf, gconstpointer data) {
	uint8_t memory[9] = {0x2e, 0x05, 0x7d, 0x45, 0x4d, 0x55, 0x5d, 0x65, 0x6d};

	g_assert(run(tf->test_cpu, memory, 8) == 8);
	g_assert(tf->test_cpu->pc.W == 9);
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0505);
	g_assert(tf->test_cpu->de.W == 0x0505);
	g_assert(tf->test_cpu->hl.W == 0x0505);
}

static void test_ld_reg8_ixn(test_fixture *tf, gconstpointer data) {
	memory *testmem = memory_new();
	testmem->memory_load(testmem, data);

	g_assert(run(tf->test_cpu, testmem->memory, 17));
	g_assert(tf->test_cpu->pc.W);
	g_assert(tf->test_cpu->a == 0x01);
	g_assert(tf->test_cpu->bc.W == 0x0203);
	g_assert(tf->test_cpu->de.W == 0x0405);
	g_assert(tf->test_cpu->hl.W == 0x0607);

	testmem->memory_free(testmem);
}

static void test_ld_reg8_iyn(test_fixture *tf, gconstpointer data) {
	memory *testmem = memory_new();
	testmem->memory_load(testmem, data);

	g_assert(run(tf->test_cpu, testmem->memory, 17));
	g_assert(tf->test_cpu->pc.W);
	g_assert(tf->test_cpu->a == 0x01);
	g_assert(tf->test_cpu->bc.W == 0x0203);
	g_assert(tf->test_cpu->de.W == 0x0405);
	g_assert(tf->test_cpu->hl.W == 0x0607);

	testmem->memory_free(testmem);
}

static void test_ld_mem_ixn_reg8(test_fixture *tf, gconstpointer data) {
	memory *testmem = memory_new();
	testmem->memory_load(testmem, data);

	g_assert(run(tf->test_cpu, testmem->memory, 17));
	g_assert(testmem->memory[0x0070] == 0x01);
	g_assert(testmem->memory[0x0071] == 0x02);
	g_assert(testmem->memory[0x0072] == 0x03);
	g_assert(testmem->memory[0x0073] == 0x04);
	g_assert(testmem->memory[0x0074] == 0x05);
	g_assert(testmem->memory[0x0075] == 0x06);
	g_assert(testmem->memory[0x0076] == 0x07);

	testmem->memory_free(testmem);
}

static void test_ld_mem_iyn_reg8(test_fixture *tf, gconstpointer data) {
	memory *testmem = memory_new();
	testmem->memory_load(testmem, data);

	g_assert(run(tf->test_cpu, testmem->memory, 17));
	g_assert(testmem->memory[0x0070] == 0x01);
	g_assert(testmem->memory[0x0071] == 0x02);
	g_assert(testmem->memory[0x0072] == 0x03);
	g_assert(testmem->memory[0x0073] == 0x04);
	g_assert(testmem->memory[0x0074] == 0x05);
	g_assert(testmem->memory[0x0075] == 0x06);
	g_assert(testmem->memory[0x0076] == 0x07);

	testmem->memory_free(testmem);
}

static void test_ld_hl(test_fixture *tf, gconstpointer data) {
	memory *testmem = memory_new();
	testmem->memory_load(testmem, data);

	g_assert(run(tf->test_cpu, testmem->memory, 50));
	g_assert(tf->test_cpu->a == 0x05);
	g_assert(tf->test_cpu->bc.W == 0x0400);
	g_assert(tf->test_cpu->de.W == 0x0201);
	g_assert(tf->test_cpu->hl.W == 0x6666);

	testmem->memory_free(testmem);
}

int main (int argc, char *argv[]) {
	g_test_init (&argc, &argv, NULL);

	g_test_add("/z80 functions/new_cpu()", test_fixture, NULL, setup_cpu, test_register_init, teardown_cpu);
	g_test_add("/z80 functions/reset_cpu()", test_fixture, NULL, setup_cpu, test_cpu_reset, teardown_cpu);
	g_test_add("/z80 helpers/_load_reg8_mem_pair()", test_fixture, NULL, setup_cpu, test_load_reg8_from_ram, teardown_cpu);
	g_test_add("/z80 helpers/_load_reg8_mem_idx_offset()", test_fixture, NULL, setup_cpu, test_load_reg8_from_offset_idx, teardown_cpu);
	g_test_add("/z80 helpers/_load_mem_idx_offset_reg8()", test_fixture, NULL, setup_cpu, test_load_mem_offset_idx_from_reg8, teardown_cpu);
	g_test_add("/z80 helpers/_load_reg16_nn()", test_fixture, NULL, setup_cpu, test_load_reg16_from_nn, teardown_cpu);
	g_test_add("/z80 instructions/NOP", test_fixture, NULL, setup_cpu, test_nop, teardown_cpu);
	g_test_add("/z80 instructions/ld r,a", test_fixture, NULL, setup_cpu, test_ld_a, teardown_cpu);
	g_test_add("/z80 instructions/ld r,b", test_fixture, NULL, setup_cpu, test_ld_b, teardown_cpu);
	g_test_add("/z80 instructions/ld r,c", test_fixture, NULL, setup_cpu, test_ld_c, teardown_cpu);
	g_test_add("/z80 instructions/ld r,d", test_fixture, NULL, setup_cpu, test_ld_d, teardown_cpu);
	g_test_add("/z80 instructions/ld r,e", test_fixture, NULL, setup_cpu, test_ld_e, teardown_cpu);
	g_test_add("/z80 instructions/ld r,h", test_fixture, NULL, setup_cpu, test_ld_h, teardown_cpu);
	g_test_add("/z80 instructions/ld r,l", test_fixture, NULL, setup_cpu, test_ld_l, teardown_cpu);
	g_test_add("/z80 instructions/ld r,(ix+n)", test_fixture, "test_ld_ixn.bin", setup_cpu, test_ld_reg8_ixn, teardown_cpu);
	g_test_add("/z80 instructions/ld r,(iy+n)", test_fixture, "test_ld_iyn.bin", setup_cpu, test_ld_reg8_iyn, teardown_cpu);
	g_test_add("/z80 instructions/ld (ix+n),r", test_fixture, "test_ld_ixn_2.bin", setup_cpu, test_ld_mem_ixn_reg8, teardown_cpu);
	g_test_add("/z80 instructions/ld (iy+n),r", test_fixture, "test_ld_iyn_2.bin", setup_cpu, test_ld_mem_iyn_reg8, teardown_cpu);
	g_test_add("/z80 instructions/ld hl", test_fixture, "test_ld_hl.bin", setup_cpu, test_ld_hl, teardown_cpu);

	return g_test_run();
}
