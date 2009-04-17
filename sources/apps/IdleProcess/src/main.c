#include <inttypes.h>
#include "hsystem_api.h"


int main(void) {

	uint32_t idle_counter = 0;

	/* Start Shell via System Call */
	start_process("Shell");

	while (1) {

		idle_counter++;

		if ((idle_counter >= 65536)) {
			idle_counter = 0;
		}
	}

	return 0;
}

