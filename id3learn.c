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
#include "id3math.h"
#include "id3missing.h"

/**
 * @brief
 * Static value used to remember the last tag used.
 */
static int last_tag;

int id3_learn_bootstrap_file(int num_handle, int missing_handle,
		FILE *attr_file, FILE *learn_file, FILE *id3_file)
{
	struct description *descr;
	struct example_set *lset;
	struct classifier *cls;

	descr = read_description_file(attr_file);
	CHECK(descr != NULL, nodescr);
	lset = read_learning_file(learn_file, descr);
	CHECK(lset != NULL, nolset);

	/* fill in missing arguments */
	id3_treat_missing(descr, lset, missing_handle);
	/* built indexes for numeric arguments */
	id3_build_index(descr, lset);
	/* discretization for numeric arguments */
	id3_discretization(descr, lset, num_handle);
	/* start the learning process */
	cls = id3_learn(descr, lset, 0);
	/* write results */
	write_id3_temp_file(descr, cls, id3_file);

	free_classifier(cls);
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

int id3_filtered_count(const struct example_set *lset, int tag)
{
	int c, i;

	c = 0;
	for (i = 0; i < lset->N; i++)
		if (lset->examples[i]->filter == tag)
			c++;

	return c;
}

double id3_I_decision_tree(const struct description *descr,
		const struct example_set *lset, int tag, int count)
{
	int i, n, j;
	float s;

	for (i = 0, s = 0; i < descr->K; i++) {
		for (j = 0, n = 0; j < lset->N; j++) {
			SKIPIF(lset->examples[j]->filter != tag);
			INCRIF(lset->examples[j]->class_id == i, n);
		}
		s += entropy(DIV(n, count));
	}

	return s;
}

double test_split_numeric(const struct description *descr,
		const struct example_set *lset,
		int index, int tag, int count)
{
	int i, c, aid, j, k, cc;
	double id3e, id3i;

	for (i = 0, id3e = 0; i < descr->attribs[index]->C; i++) {
		for (j = 0, c = 0; j < lset->N; j++) {
			SKIPIF(lset->examples[j]->filter != tag);
			aid = lset->examples[j]->attr_ids[index];
			SKIPIF(aid >= descr->attribs[index]->ptr[i]);
			SKIPIF(i && aid < descr->attribs[index]->ptr[i-1]);
			c++;
		}
		SKIPIF(c == 0);
		for (k = 0, id3i = 0; k < descr->K; k++) {
			for (j = 0, cc = 0; j < lset->N; j++) {
				SKIPIF(lset->examples[j]->filter != tag);
				aid = lset->examples[j]->attr_ids[index];
				SKIPIF(aid >= descr->attribs[index]->ptr[i]);
				SKIPIF(i &&
					aid < descr->attribs[index]->ptr[i-1]);
				INCRIF(lset->examples[j]->class_id == k, cc);
			}
			id3i += entropy(DIV(cc, c));
		}
		fprintf(stderr, "I(%d_%d) = %6.3lf\n", index, i, id3i);
		id3e += DIV(c, count) * id3i;
	}

	for (j = 0, c = 0; j < lset->N; j++) {
		SKIPIF(lset->examples[j]->filter != tag);
		aid = lset->examples[j]->attr_ids[index];
		SKIPIF(aid < descr->attribs[index]->ptr[i-1]);
		c++;
	}
	if (c == 0)
		goto end;
	for (k = 0, id3i = 0; k < descr->K; k++) {
		for (j = 0, cc = 0; j < lset->N; j++) {
			SKIPIF(lset->examples[j]->filter != tag);
			aid = lset->examples[j]->attr_ids[index];
			SKIPIF(aid < descr->attribs[index]->ptr[i-1]);
			INCRIF(lset->examples[j]->class_id == k, cc);
		}
		id3i += entropy(DIV(cc, c));
	}
	fprintf(stderr, "I(%d_%d) = %6.3lf\n", index, i, id3i);
	id3e += DIV(c, count) * id3i;

end:
	fprintf(stderr, "E(%d) = %6.3lf\n", index, id3e);

	return id3e;
}

double test_split_discrete(const struct description *descr,
		const struct example_set *lset,
		int index, int tag, int count)
{
	int i, c, j, k, cc;
	double id3e, id3i;

	for (i = 0, id3e = 0; i < descr->attribs[index]->C; i++) {
		for (j = 0, c = 0; j < lset->N; j++) {
			SKIPIF(lset->examples[j]->filter != tag);
			SKIPIF(lset->examples[j]->attr_ids[index] != i);
			c++;
		}
		SKIPIF(c == 0);
		for (k = 0, id3i = 0; k < descr->K; k++) {
			for (j = 0, cc = 0; j < lset->N; j++) {
				SKIPIF(lset->examples[j]->filter != tag);
				SKIPIF(lset->examples[j]->attr_ids[index] != i);
				INCRIF(lset->examples[j]->class_id == k, cc);
			}
			id3i += entropy(DIV(cc, c));
		}
		fprintf(stderr, "I(%d_%d) = %6.3lf\n", index, i, id3i);
		id3e += DIV(c, count) * id3i;
	}
	fprintf(stderr, "E(%d) = %6.3lf\n", index, id3e);

	return id3e;
}

struct classifier *id3_learn(const struct description *descr,
		const struct example_set *lset, int tag)
{
	double iad, gain, gbest, exp;
	int i, ibest, count;

	count = id3_filtered_count(lset, tag);
	iad = id3_I_decision_tree(descr, lset, tag, count);
	fprintf(stderr, "I_AD = %6.3lf\n", iad);

	gbest = 0.0l;
	/* for each attribute */
	for (i = 0; i < descr->M; i++) {
		if (descr->attribs[i]->type == NUMERIC)
			exp = test_split_numeric(descr, lset, i, tag, count);
		else
			exp = test_split_discrete(descr, lset, i, tag, count);
		gain = iad - exp;
		fprintf(stderr, "G(%d) = %6.3lf\n", i, gain);
		if (gbest < gain) {
			gbest = gain;
			ibest = i;
		}
	}
	fprintf(stderr, "Split on %d\n", ibest);

	return split_on(descr, lset, tag, ibest);
}

struct classifier *build_numeric_classifier(const struct description *descr,
		const struct example_set *lset, struct classifier *cls,
		int index)
{
	int i, cc, kc;

	kc = 1;
	cc = -1;
	last_tag++;
	for (i = 0; i < lset->N; i++) {
		SKIPIF(lset->examples[i]->filter != cls->tag);
		SKIPIF(index != cls->C - 1 &&
			lset->examples[i]->attr_ids[cls->id] >=
				cls->values[index]);
		SKIPIF(index && lset->examples[i]->attr_ids[cls->id]
				< cls->values[index - 1]);
		if (lset->examples[i]->class_id != cc) {
			if (cc != -1)
				kc = 0;
			else
				cc = lset->examples[i]->class_id;
		}
		lset->examples[i]->filter = last_tag;
	}
	fprintf(stderr, "Tag: %d -> CC: %d\n", last_tag, cc);

	/* single class */
	if (kc) {
		struct classifier *newcls = calloc(1, sizeof(*newcls));
		newcls->tag = last_tag;
		newcls->id = cc;
		newcls->C = 0;
		return newcls;
	}
	return id3_learn(descr, lset, last_tag);
}

struct classifier *build_discrete_classifier(const struct description *descr,
		const struct example_set *lset, struct classifier *cls,
		int index)
{
	int i, cc, kc;

	kc = 1;
	cc = -1;
	last_tag++;
	for (i = 0; i < lset->N; i++) {
		SKIPIF(lset->examples[i]->filter != cls->tag);
		SKIPIF(lset->examples[i]->attr_ids[cls->id]
				!= cls->values[index]);
		if (lset->examples[i]->class_id != cc) {
			if (cc != -1)
				kc = 0;
			else
				cc = lset->examples[i]->class_id;
		}
		lset->examples[i]->filter = last_tag;
	}
	fprintf(stderr, "Tag: %d -> CC: %d\n", last_tag, cc);

	/* single class */
	if (kc) {
		struct classifier *newcls = calloc(1, sizeof(*newcls));
		newcls->tag = last_tag;
		newcls->id = cc;
		newcls->C = 0;
		return newcls;
	}
	return id3_learn(descr, lset, last_tag);
}

struct classifier *build_classifier(const struct description *descr,
		const struct example_set *lset, struct classifier *cls,
		int index)
{
	if (descr->attribs[cls->id]->type == NUMERIC)
		return build_numeric_classifier(descr, lset, cls, index);
	return build_discrete_classifier(descr, lset, cls, index);
}

struct classifier *split_on(const struct description *descr,
		const struct example_set *lset, int tag, int id)
{
	struct classifier *cls;
	int i;

	cls = calloc(1, sizeof(*cls));
	cls->tag = tag;
	cls->id = id;
	cls->C = descr->attribs[id]->C;
	INCRIF(descr->attribs[id]->type == NUMERIC, cls->C);
	cls->values = calloc(cls->C, sizeof(cls->values[0]));
	cls->cls = calloc(cls->C, sizeof(cls->cls[0]));

	if (descr->attribs[id]->type == NUMERIC) {
		for (i = 0; i < cls->C - 1; i++)
			cls->values[i] = descr->attribs[id]->ptr[i];
		cls->values[i] = 0;
	} else
		for (i = 0; i < cls->C; i++)
			cls->values[i] = i;

	for (i = 0; i < cls->C; i++)
		cls->cls[i] = build_classifier(descr, lset, cls, i);

	return cls;
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
		SKIPIF(descr->attribs[i]->type != NUMERIC);
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

double split_e(const struct description *descr,
		const struct example_set *lset,
		int index, int limit)
{
	int i, cb, ca, aid, N, ccb, cca, K, k;
	double e;

	cb = 0;
	ca = 0;
	N = lset->N;
	for (i = 0; i < N; i++) {
		aid = lset->examples[i]->attr_ids[index];
		INCRIF(aid < limit, cb);
		INCRIF(aid >= limit, ca);
	}

	K = descr->K;
	e = 0;
	for (k = 0; k < K; k++) {
		ccb = 0;
		cca = 0;
		for (i = 0; i < N; i++) {
			SKIPIF(lset->examples[i]->class_id != k);
			aid = lset->examples[i]->attr_ids[index];
			INCRIF(aid < limit, ccb);
			INCRIF(aid >= limit, cca);
		}
		e += entropy(DIV(cca, ca)) * DIV(ca, N);
		e += entropy(DIV(ccb, cb)) * DIV(cb, N);
	}

	return e;
}

void id3_attr_discr(const struct description *descr,
		const struct example_set *lset,
		int index, int num_handle)
{
	int cclass, ccount, *candidates, i, C, cid, ix, aid, imin;
	double *entropies, min;

	candidates = calloc(lset->N, sizeof(candidates[0]));
	C = descr->attribs[index]->C;
	cclass = lset->examples[descr->attribs[index]->ptr[0]]->class_id;
	ccount = 0;
	for (i = 0; i < C; i++) {
		ix = descr->attribs[index]->ptr[i];
		cid = lset->examples[ix]->class_id;
		aid = lset->examples[ix]->attr_ids[index];
		SKIPIF(ccount && aid == candidates[ccount - 1]);
		if (cid != cclass) {
			cclass = cid;
			candidates[ccount++] = aid;
		}
	}

	entropies = calloc(ccount, sizeof(entropies[0]));
	for (i = 0; i < ccount; i++)
		entropies[i] = split_e(descr, lset, index, candidates[i]);

	imin = 0;
	min = entropies[imin];
	for (i = 1; i < ccount; i++)
		if (min > entropies[i]) {
			min = entropies[i];
			imin = i;
		}

	if (num_handle == NUM_DIV) {
		descr->attribs[index]->ptr[0] = candidates[imin];
		descr->attribs[index]->C = 1;
		goto end;
	}

	fprintf(stderr, "TODO %d\n", __LINE__);

	for (i = 0; i < ccount; i++)
		fprintf(stderr, "%d(%5.3lf) ", candidates[i], entropies[i]);
	fprintf(stderr, "\n");

end:
	free(candidates);
	free(entropies);
}

void id3_discretization(const struct description *descr,
		const struct example_set *lset, int num_handle)
{
	int i;

	for (i = 0; i < descr->M; i++) {
		SKIPIF(descr->attribs[i]->type != NUMERIC);
		id3_attr_discr(descr, lset, i, num_handle);
	}
}

