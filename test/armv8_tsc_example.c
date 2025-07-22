#include "armv8_tsc.h"

#include <stdio.h>

static void loop()
{
	for (int i=0; i<1000000; i++)
	{
#if defined(_MSC_VER)
		__nop();
#else
		__asm__ __volatile__("");
#endif
	}
}

int main()
{
	uint64_t ticks0 = armv8_cntvct();
	loop();
	uint64_t ticks1 = armv8_cntvct();
	uint64_t freq = armv8_cntfrq();

	printf("cntvct : %zu ticks @ %zu MHz = %.2f msec\n",
		(size_t)(ticks1 - ticks0),
		(size_t)(freq / 1000000),
		(ticks1 - ticks0) * 1000.0 / freq);

	if (armv8_tsc_init())
	{
		uint64_t cycles0 = armv8_pmccntr();
		loop();
		uint64_t cycles1 = armv8_pmccntr();

		printf("pmccntr: %zu cycles\n", (size_t)(cycles1 - cycles0));
		armv8_tsc_done();
	}
	else
	{
		printf("perf not available! not enough privileges?\n");
	}
}
