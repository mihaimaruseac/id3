/*!
 * @file id3missing.c
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
		struct example_set *lset, int attr_index,
		int miss_index)
{
	int sum, count, i;

	count = 0;
	sum = 0;
	for (i = 0; i < lset->N; i++) {
		SKIPIF(MISS_INDEX(lset->examples[i]->miss, miss_index));
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
		struct example_set *lset, int attr_index,
		int miss_index)
{
	int K, i, j, max, imax, c, **counts, C, *vals, ind, v;

	K = descr->K;
	C = lset->N;
	counts = calloc(K, sizeof(counts[0]));
	for (i = 0; i < K; i++)
		counts[i] = calloc(C, sizeof(counts[i][0]));
	vals = calloc(C, sizeof(vals[0]));

	C = 0;
	for (i = 0; i < lset->N; i++) {
		SKIPIF(MISS_INDEX(lset->examples[i]->miss, miss_index));
		c = lset->examples[i]->class_id;
		v = lset->examples[i]->attr_ids[attr_index];
		for (ind = 0; ind < C; ind++)
			if (vals[ind] == v) {
				counts[c][ind]++;
				break;
			}
		SKIPIF(ind < C);
		counts[c][C]++;
		vals[C++] = v;
	}

	for (i = 0; i < lset->N; i++)
		if (MISS_INDEX(lset->examples[i]->miss, miss_index)) {
			c = lset->examples[i]->class_id;
			imax = 0;
			max = counts[c][imax];
			for (j = 1; j < C; j++)
				if (max < counts[c][j]) {
					max = counts[c][j];
					imax = j;
				}
			lset->examples[i]->attr_ids[attr_index] = vals[imax];
			lset->examples[i]->miss ^= 1 << miss_index;
		}

	for (i = 0; i < K; i++)
		free(counts[i]);
	free(counts);
	free(vals);
}

void discrete_maj_fill_missing(const struct description *descr,
		struct example_set *lset, int attr_index,
		int miss_index)
{
	int *counts, i, max, imax, C;

	C = descr->attribs[attr_index]->C;
	counts = calloc(C, sizeof(counts[0]));
	for (i = 0; i < lset->N; i++) {
		SKIPIF(MISS_INDEX(lset->examples[i]->miss, miss_index));
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
		struct example_set *lset, int attr_index,
		int miss_index)
{
	int K, C, i, **counts, c, j, max, imax;

	K = descr->K;
	C = descr->attribs[attr_index]->C;
	counts = calloc(K, sizeof(counts[0]));
	for (i = 0; i < K; i++)
		counts[i] = calloc(C, sizeof(counts[i][0]));

	for (i = 0; i < lset->N; i++) {
		SKIPIF(MISS_INDEX(lset->examples[i]->miss, miss_index));
		c = lset->examples[i]->class_id;
		counts[c][lset->examples[i]->attr_ids[attr_index]]++;
	}

	for (i = 0; i < lset->N; i++)
		if (MISS_INDEX(lset->examples[i]->miss, miss_index)) {
			c = lset->examples[i]->class_id;
			imax = 0;
			max = counts[c][imax];
			for (j = 1; j < C; j++)
				if (max < counts[c][j]) {
					max = counts[c][j];
					imax = j;
				}
			lset->examples[i]->attr_ids[attr_index] = imax;
			lset->examples[i]->miss ^= 1 << miss_index;
		}

	for (i = 0; i < K; i++)
		free(counts[i]);
	free(counts);
}

