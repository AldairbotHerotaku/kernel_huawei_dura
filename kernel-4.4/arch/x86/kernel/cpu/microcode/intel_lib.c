
#include <linux/firmware.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

#include <asm/microcode_intel.h>
#include <asm/processor.h>
#include <asm/msr.h>

static inline bool cpu_signatures_match(unsigned int s1, unsigned int p1,
					unsigned int s2, unsigned int p2)
{
	if (s1 != s2)
		return false;

	/* Processor flags are either both 0 ... */
	if (!p1 && !p2)
		return true;

	/* ... or they intersect. */
	return p1 & p2;
}

int microcode_sanity_check(void *mc, int print_err)
{
	unsigned long total_size, data_size, ext_table_size;
	struct microcode_header_intel *mc_header = mc;
	struct extended_sigtable *ext_header = NULL;
	int sum, orig_sum, ext_sigcount = 0, i;
	struct extended_signature *ext_sig;

	total_size = get_totalsize(mc_header);
	data_size = get_datasize(mc_header);

	if (data_size + MC_HEADER_SIZE > total_size) {
		if (print_err)
			pr_err("error! Bad data size in microcode data file\n");
		return -EINVAL;
	}

	if (mc_header->ldrver != 1 || mc_header->hdrver != 1) {
		if (print_err)
			pr_err("error! Unknown microcode update format\n");
		return -EINVAL;
	}
	ext_table_size = total_size - (MC_HEADER_SIZE + data_size);
	if (ext_table_size) {
		if ((ext_table_size < EXT_HEADER_SIZE)
		 || ((ext_table_size - EXT_HEADER_SIZE) % EXT_SIGNATURE_SIZE)) {
			if (print_err)
				pr_err("error! Small exttable size in microcode data file\n");
			return -EINVAL;
		}
		ext_header = mc + MC_HEADER_SIZE + data_size;
		if (ext_table_size != exttable_size(ext_header)) {
			if (print_err)
				pr_err("error! Bad exttable size in microcode data file\n");
			return -EFAULT;
		}
		ext_sigcount = ext_header->count;
	}

	/* check extended table checksum */
	if (ext_table_size) {
		int ext_table_sum = 0;
		int *ext_tablep = (int *)ext_header;

		i = ext_table_size / DWSIZE;
		while (i--)
			ext_table_sum += ext_tablep[i];
		if (ext_table_sum) {
			if (print_err)
				pr_warn("aborting, bad extended signature table checksum\n");
			return -EINVAL;
		}
	}

	/* calculate the checksum */
	orig_sum = 0;
	i = (MC_HEADER_SIZE + data_size) / DWSIZE;
	while (i--)
		orig_sum += ((int *)mc)[i];
	if (orig_sum) {
		if (print_err)
			pr_err("aborting, bad checksum\n");
		return -EINVAL;
	}
	if (!ext_table_size)
		return 0;
	/* check extended signature checksum */
	for (i = 0; i < ext_sigcount; i++) {
		ext_sig = (void *)ext_header + EXT_HEADER_SIZE +
			  EXT_SIGNATURE_SIZE * i;
		sum = orig_sum
			- (mc_header->sig + mc_header->pf + mc_header->cksum)
			+ (ext_sig->sig + ext_sig->pf + ext_sig->cksum);
		if (sum) {
			if (print_err)
				pr_err("aborting, bad checksum\n");
			return -EINVAL;
		}
	}
	return 0;
}
EXPORT_SYMBOL_GPL(microcode_sanity_check);

/*
 * Returns 1 if update has been found, 0 otherwise.
 */
int find_matching_signature(void *mc, unsigned int csig, int cpf)
{
	struct microcode_header_intel *mc_hdr = mc;
	struct extended_sigtable *ext_hdr;
	struct extended_signature *ext_sig;
	int i;

	if (cpu_signatures_match(csig, cpf, mc_hdr->sig, mc_hdr->pf))
		return 1;

	/* Look for ext. headers: */
	if (get_totalsize(mc_hdr) <= get_datasize(mc_hdr) + MC_HEADER_SIZE)
		return 0;

	ext_hdr = mc + get_datasize(mc_hdr) + MC_HEADER_SIZE;
	ext_sig = (void *)ext_hdr + EXT_HEADER_SIZE;

	for (i = 0; i < ext_hdr->count; i++) {
		if (cpu_signatures_match(csig, cpf, ext_sig->sig, ext_sig->pf))
			return 1;
		ext_sig++;
	}
	return 0;
}

/*
 * Returns 1 if update has been found, 0 otherwise.
 */
int has_newer_microcode(void *mc, unsigned int csig, int cpf, int new_rev)
{
	struct microcode_header_intel *mc_hdr = mc;

	if (mc_hdr->rev <= new_rev)
		return 0;

	return find_matching_signature(mc, csig, cpf);
}
EXPORT_SYMBOL_GPL(has_newer_microcode);
