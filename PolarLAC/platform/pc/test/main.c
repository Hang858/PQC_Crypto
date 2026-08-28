#include "test_correctness.h"
#include "test_speed.h"
#include "test_cpucycles.h"
#include "rand.h"
#include "api.h"
#include "polarlac_params.h"
#include "my_log.h"
#include "malloc.h"
#include <string.h>

static polarlac_level_t parse_level(const char *level_name)
{
	if (level_name == NULL) {
		return POLARLAC_DEFAULT_LEVEL;
	}
	if (strcmp(level_name, "light") == 0 || strcmp(level_name, "LAC_LIGHT") == 0) {
		return POLARLAC_LEVEL_LIGHT;
	}
	if (strcmp(level_name, "128") == 0 || strcmp(level_name, "LAC128") == 0) {
		return POLARLAC_LEVEL_128;
	}
	if (strcmp(level_name, "256") == 0 || strcmp(level_name, "LAC256") == 0) {
		return POLARLAC_LEVEL_256;
	}
	return POLARLAC_DEFAULT_LEVEL;
}

static void print_memory_stats(const char *label)
{
	LOG_D("\n--- Memory Stats (%s) ---\n", label);
	LOG_D("  Total heap size:      %u bytes\n", HEAP_SIZE);
	LOG_D("  Current allocated:   %u bytes\n", get_total_allocated_memory());
	LOG_D("  Current free:        %u bytes\n", get_total_free_memory());
	LOG_D("  Max heap usage:      %u bytes\n", get_max_heap_usage());
	LOG_D("  Memory blocks:       %u\n", get_total_block_count());
	LOG_D("  Heap utilization:    %.1f%%\n", 
		  (float)get_total_allocated_memory() / HEAP_SIZE * 100);
}

#ifndef DOWNLOAD_MODE
int main(int argc, char **argv)
{
	polarlac_level_t level = parse_level(argc >= 3 ? argv[2] : NULL);
	if (polarlac_set_level(level) != 0) {
		return -1;
	}
	const polarlac_params_t *params = polarlac_current_params();
	if (params == NULL) {
		return -1;
	}
	
	reset_max_heap_usage();
	print_memory_stats("Before Test");
	
	if(argc < 2 || argc > 3)
	{
		LOG_D("command format:\n");
		LOG_D("lac speed [light|128|256] : test the speed of lac\n");
		LOG_D("lac cpucycles [light|128|256]: test the cpucycles of lac\n");
		LOG_D("lac correctness [light|128|256]: test the correctness of lac\n");
		LOG_D("lac basicblock [light|128|256]: test the speed of basic blocks used in lac\n");
	}
	else
	{
		LOG_D("============== test %s ==============\n\n", params->name);
		if(strcmp(argv[1],"speed")==0)
		{
			test_pke_speed();
		}
		
		if(strcmp(argv[1],"cpucycles")==0)
		{		
			test_pke_cpucycles();
		}
		
		if(strcmp(argv[1],"correctness")==0)
		{
			test_pke_correctness();
			test_kem_fo_correctness();
		}
		
		if(strcmp(argv[1],"basicblock")==0)
		{
			test_hash_cpucycles();
			test_gen_psi_cpucycles();
			test_gen_a_cpucycles();
			test_poly_mul_cpucycles();
			test_poly_mul_speed();
		}

		LOG_D("============================================\n");
	}

	print_memory_stats("After Test");
	LOG_D("\n============================================\n");
	LOG_D("  Maximum Heap Usage:   %u bytes (%.1f%% of %uKB)\n", 
		  get_max_heap_usage(), 
		  (float)get_max_heap_usage() / HEAP_SIZE * 100,
		  HEAP_SIZE / 1024);
	LOG_D("============================================\n");

	return 0;
}
#endif
