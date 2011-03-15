/*!
 * @file fillmissing.c
 * @brief Treating of missing values.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Treating of missing values.
 *
 * Contains the various implementations of routines to treat missing
 * attributes from the description.
 *
 * @section QUOTE
 * Intelligence is what you use when you don't know what to do. (Jean Piaget)
 *
 * @bug No known bugs.
 */

#include "globals.h"
#include "id3missing.h"

void numeric_maj_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	int sum, count, i;

	count = 0;
	sum = 0;
	for (i = 0; i < lset->N; i++) {
		if (MISS_INDEX(lset->examples[i]->miss, miss_index))
			continue;
		sum += lset->examples[i]->attr_ids[attr_index];
		count += 1;
	}

	sum /= count;
	for (i = 0; i < lset->N; i++)
		if (MISS_INDEX(lset->examples[i]->miss, miss_index)) {
			lset->examples[i]->attr_ids[attr_index] = sum;
			lset->examples[i]->miss ^= 1 << miss_index;
		}
}

void numeric_prb_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void numeric_id3_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void discrete_maj_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	int *counts, i, max, imax, C;

	C = descr->attribs[attr_index]->C;
	counts = calloc(C, sizeof(counts[0]));
	for (i = 0; i < lset->N; i++) {
		if (MISS_INDEX(lset->examples[i]->miss, miss_index))
			continue;
		counts[lset->examples[i]->attr_ids[attr_index]]++;
	}

	imax = 0;
	max = counts[imax];
	for (i = 1; i < C; i++)
		if (max < counts[i]) {
			max = counts[i];
			imax = i;
		}

	for (i = 0; i < lset->N; i++)
		if (MISS_INDEX(lset->examples[i]->miss, miss_index)) {
			lset->examples[i]->attr_ids[attr_index] = imax;
			lset->examples[i]->miss ^= 1 << miss_index;
		}

	free(counts);
}

void discrete_prb_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void discrete_id3_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

