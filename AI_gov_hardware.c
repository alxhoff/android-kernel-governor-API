/**
 * @file AI_gov_hardware.c
 * @author Alex Hoffman
 * @date 10 March 2018 
 * @brief Handles the governor's hardware functionality
 */

/* -- Includes -- */
/* Kernel includes. */
#include <linux/slab.h>

/* Governor includes. */
#include "AI_gov_hardware.h"
#include "AI_gov_phases.h"
#include "AI_gov_types.h"
#include "AI_gov_kernel_write.h"

signed int AI_gov_init(struct AI_gov_info** in)
{

	(*in) =	(struct AI_gov_info*)kcalloc(1, sizeof(struct AI_gov_info),
					GFP_KERNEL);

	if ((*in) == NULL) {
		KERNEL_DEBUG_MSG("[GOVERNOR] AI_governor: "
				"governor malloc failed\n");
		return -ENOMEM;
	}

	KERNEL_DEBUG_MSG("[GOVERNOR] AI_gov_init governor "
			"allocated \n");

	(*in)->hardware =
		(struct AI_gov_cur_HW*)kcalloc( 1,
				sizeof(struct AI_gov_cur_HW), GFP_KERNEL);

	if ((*in)->hardware == NULL) {
		KERNEL_DEBUG_MSG("[GOVERNOR] AI_governor: "
				"hardware malloc failed\n");
		return -ENOMEM;
	}

	KERNEL_DEBUG_MSG("[GOVERNOR] AI_gov_init hardware "
			"allocated \n");

	(*in)->hardware->freq_table =
			(struct AI_gov_freq_table* )kcalloc( 1,
					sizeof(struct AI_gov_freq_table), GFP_KERNEL);

	if ((*in)->hardware->freq_table == NULL) {
		KERNEL_DEBUG_MSG("[GOVERNOR] AI_governor: "
				"AI_freq_table malloc failed\n");
		return -ENOMEM;
	}

	KERNEL_DEBUG_MSG("[GOVERNOR] AI_gov_init freq table "
			"allocated \n");

#ifdef CPU_IS_BIG_LITTLE
	(*in)->hardware->is_big_little = true;
#else
	(*in)->hardware->is_big_little = false;
#endif

	(*in)->hardware->big_freq = 1200000000;
	(*in)->hardware->big_state = true;
	(*in)->hardware->cpu_count = 8;
	(*in)->hardware->little_freq = 900000000;

	return 0;
}

signed int AI_gov_change_freq_table(struct AI_gov_cur_HW* hardware,
		struct AI_gov_freq_table* freq_table )
{
	if(hardware->has_table == TRUE)
		kfree(hardware->freq_table);
	else
		return -1;

	hardware->freq_table = freq_table;

	return 0;
}
