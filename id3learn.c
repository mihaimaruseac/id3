/*!
 * @file id3learn.c
 * @brief Learning part of the id3 classifier.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Learning part of the id3 classifier.
 *
 * Contains the definitions for the learning algorithms used in the ID3
 * classification.
 *
 * @section QUOTE
 * A year spent in artificial intelligence is enough to make one believe in
 * God. (Alan Perlis)
 *
 * @bug No known bugs.
 */

#include "globals.h"
#include "id3learn.h"
#include "id3missing.h"

int id3_learn_bootstrap_file(int num_handle, int missing_handle,
		FILE *attr_file, FILE *learn_file, FILE *id3_file)
{
	struct description *descr;
	struct example_set *lset;

	descr = read_description_file(attr_file);
	CHECK(descr != NULL, nodescr);
	lset = read_learning_file(learn_file, descr);
	CHECK(lset != NULL, nolset);

	/* fill in missing arguments */
	id3_treat_missing(descr, lset, missing_handle);
	/* built indexes for numeric arguments */
	id3_build_index(descr, lset);
	/* start the learning process */
	id3_learn(descr, lset, num_handle);

	free_description(descr);
	free_and_set_NULL(descr);
	free_example_set(lset);
	free_and_set_NULL(lset);

	return EXIT_SUCCESS;

nolset:
	free_description(descr);
	free_and_set_NULL(descr);
nodescr:
	return set_error(EINVAL); /* invalid file received */
}

void id3_learn(const struct description *descr,
		const struct example_set *lset,
		int num_handle)
{
#if 0
	write_description(descr, stdout);
	write_set(lset, descr, stdout);
#endif
}

void id3_treat_missing(const struct description *descr,
		struct example_set *lset, int missing_handle)
{
	int i, index;

	for (i = 0; i < MISS_COUNT; i++) {
		index = lset->missing[i];
		if (descr->attribs[index]->type == NUMERIC)
			switch (missing_handle) {
			case MISS_MAJ:
				numeric_maj_fill_missing(descr, lset,
						index, i);
				break;
			case MISS_PRB:
				numeric_prb_fill_missing(descr, lset,
						index, i);
				break;
			case MISS_ID3:
				numeric_id3_fill_missing(descr, lset,
						index, i);
				break;
			}
		else
			switch (missing_handle) {
			case MISS_MAJ:
				discrete_maj_fill_missing(descr, lset,
						index, i);
				break;
			case MISS_PRB:
				discrete_prb_fill_missing(descr, lset,
						index, i);
				break;
			case MISS_ID3:
				discrete_id3_fill_missing(descr, lset,
						index, i);
				break;
			}
	}
}

void id3_build_index(const struct description *descr,
		const struct example_set *lset)
{
	int i, j, k, ii, jj;

	for (i = 0; i < descr->M; i++) {
		if (descr->attribs[i]->type != NUMERIC)
			continue;
		descr->attribs[i]->C = lset->N;
		descr->attribs[i]->ptr = calloc(lset->N,
				sizeof(descr->attribs[i]->ptr));

		for (j = 0; j < lset->N; j++)
			descr->attribs[i]->ptr[j] = j;

		/* sorting in place in O(N^2) */
		for (j = 0; j < lset->N; j++)
			for (k = j + 1; k < lset->N; k++) {
				ii = descr->attribs[i]->ptr[j];
				jj = descr->attribs[i]->ptr[k];
				if (lset->examples[ii]->attr_ids[i] >
					lset->examples[jj]->attr_ids[i]) {
					descr->attribs[i]->ptr[k] = ii;
					descr->attribs[i]->ptr[j] = jj;
				}
			}
	}
}
