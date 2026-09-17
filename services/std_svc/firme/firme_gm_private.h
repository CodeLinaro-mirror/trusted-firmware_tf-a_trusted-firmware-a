#ifndef FIRME_GM_PRIVATE_H
#define FIRME_GM_PRIVATE_H

/* This value comes from read_gptbr_el3 */
#define GPT_L0BASE	((uintptr_t)((read_gptbr_el3() >> GPTBR_BADDR_SHIFT) & GPTBR_BADDR_MASK) << GPTBR_BADDR_VAL_SHIFT)

/*
 * Lookup table used to speed up granule transitions by associating GPIs and
 * relevant information such as descriptors, NSE fields, and transition
 * policies defined by FIRME rules.
 */
typedef struct {
	uint64_t desc;
	uint8_t nse;
	uint8_t nse2;
	uint16_t policy[3];
} firme_gpi_config_t;

/*
 * Internal structure to retrieve the values from get_gpi_params();
 * todo: extend this to gpi_geometry
 */
typedef struct {
	uint64_t gpt_l1_desc;
	uint64_t *gpt_l1_addr;
	unsigned int idx;
	unsigned int gpi_shift;
	unsigned int gpi;
} firme_gpi_info_t;

typedef struct {
	uint8_t pps_bits;
	uint8_t pgs_bits;

	/* Number of L1 entries in 2MB, depending on GPCCR_EL3.PGS */
	uint8_t l1_cnt_2mb;
} firme_gpt_config_t;

typedef void (*gpt_shatter_func)(uintptr_t base,
				 const firme_gpi_info_t *gpi_info,
				 uint64_t l1_desc);
typedef void (*gpt_tlbi_func)(uintptr_t base);

/*
 * Look-up structure for
 * invalidating TLBs of GPT entries by Physical address, last level.
 */
typedef struct {
	gpt_tlbi_func function;
	size_t mask;
} gpt_tlbi_lookup_t;

void firme_gm_gpi_init(void);

int firme_gm_gpi_set(uint64_t base, uint64_t gcnt, uint64_t attrs,
		     uint64_t flags, uint64_t *gcnt_ret);

#endif /* FIRME_GM_PRIVATE_H */
