#include <glib.h>
#include <stdint.h>
#include <stdlib.h>
#include "memory.h"

typedef struct {
	memory *mem;
} test_fixture;

static void setup_memory(test_fixture *tf, gconstpointer data) {
	tf->mem = memory_new();
}

static void teardown_memory(test_fixture *tf, gconstpointer data) {
	tf->mem->memory_free(tf->mem);
}

static void test_memory_load(test_fixture *tf, gconstpointer data) {
	g_assert(tf->mem->memory_load(tf->mem, data) == 11);

	uint8_t bin[11] = {  0x3e, 0x05, 0x06, 0x07, 0x80, 0x32, 0x10, 0x00, 0xc3, 0x00, 0x00 };

	for (int i = 0; i < 11; i++) {
		g_assert(tf->mem->memory[i] == bin[i]);
	}
}

static void test_memory_load_nofile(test_fixture *tf, gconstpointer data) {
	g_assert(tf->mem->memory_load(tf->mem, NULL) == -1);
}

int main(int argc, char *argv[]) {
	g_test_init(&argc, &argv, NULL);

	g_test_add("/memory/memory_load()", test_fixture, "data/test.bin", setup_memory, test_memory_load, teardown_memory);
	g_test_add("/memory/memory_load() bad input", test_fixture, NULL, setup_memory, test_memory_load_nofile, teardown_memory);

	return g_test_run();
}

