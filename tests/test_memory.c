#include <glib.h>
#include <stdint.h>
#include <stdlib.h>
#include "memory.h"

typedef struct {
	uint8_t *memory;
} test_fixture;

static void setup_memory(test_fixture *tf, gconstpointer data) {
	tf->memory = create_ram();
}

static void teardown_memory(test_fixture *tf, gconstpointer data) {
	free(tf->memory);
}

static void test_load_rom(test_fixture *tf, gconstpointer data) {
	g_assert(load_rom(data, tf->memory) == 11);

	uint8_t bin[11] = {  0x3e, 0x05, 0x06, 0x07, 0x80, 0x32, 0x10, 0x00, 0xc3, 0x00, 0x00 };

	for (int i = 0; i < 11; i++) {
		g_assert(tf->memory[i] == bin[i]);
	}
}

int main(int argc, char *argv[]) {
	g_test_init(&argc, &argv, NULL);

	g_test_add("/memory/load_rom()", test_fixture, "test.bin", setup_memory, test_load_rom, teardown_memory);

	return g_test_run();
}
