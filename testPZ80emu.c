/** \file */

#include <glib.h>
#include <stdlib.h>
#include "z80.h"
#include "utils.h"

// set up test fixture with a cpu object
typedef struct {
	z80 *testCpu;
} testFixture;

// initialize the cpu object
static void setupCpu(testFixture *tf, gconstpointer data) {
	tf->testCpu = new_cpu();
}

// free the cpu object
static void teardownCpu(testFixture *tf, gconstpointer data) {
	free(tf->testCpu);
}

// test the registers to ensure they were set to 0 on init
static void testRegisterInit(testFixture *tf, gconstpointer data) {
	g_assert(tf->testCpu->pc.W == 0);
	g_assert(tf->testCpu->counter == 0);
	g_assert(tf->testCpu->a == 0);

	// iterate over the flags bitfield and make sure all bits are unset
	for (int i = 0; i < 6; i++) {
		g_assert(!IS_SET(tf->testCpu->flags, i));
	}

	g_assert(tf->testCpu->bc.W == 0);
	g_assert(tf->testCpu->de.W == 0);
	g_assert(tf->testCpu->hl.W == 0);
	g_assert(tf->testCpu->ix.W == 0);
	g_assert(tf->testCpu->iy.W == 0);
	g_assert(tf->testCpu->_a == 0);
	g_assert(tf->testCpu->_bc.W == 0);
	g_assert(tf->testCpu->_de.W == 0);
	g_assert(tf->testCpu->_hl.W == 0);
	g_assert(tf->testCpu->sp.W == 0);
}

static void testCpuReset(testFixture *tf, gconstpointer data) {
	// set the program counter to non-zero and check to be sure it worked
	tf->testCpu->pc.W = 0x1234;
	g_assert(tf->testCpu->pc.W == 0x1234);

	// reset the cpu
	reset_cpu(tf->testCpu);

	g_assert(tf->testCpu->pc.W == 0);
}

int main (int argc, char *argv[]) {
    g_test_init (&argc, &argv, NULL);

	g_test_add("/z80/Register Init", testFixture, NULL, setupCpu, testRegisterInit, teardownCpu);
	g_test_add("/z80/CPU Reset", testFixture, NULL, setupCpu, testCpuReset, teardownCpu);

    return g_test_run();
}
