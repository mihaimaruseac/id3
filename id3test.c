/*!
 * @file id3test.c
 * @brief Classifying instances using an id3 tree.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Classifying instances using an id3 tree.
 *
 * Contains functions used to classify examples with an id3 classifier.
 *
 * @section QUOTE
 * You've got to stop looking at the big picture. (Gunnar Grimnes)
 *
 * @bug No known bugs.
 */

#include "globals.h"
#include "id3test.h"

int id3_test(FILE *id3, FILE *test, FILE *out)
{
	struct description *descr;
	struct classifier *cls;
	struct example_set *lset;
	int i;

	descr = read_description_file(id3);
	CHECK(descr != NULL, nodescr);
	cls = read_classifier(id3);
	CHECK(cls != NULL, fail);
	lset = read_testing_file(test, descr);
	CHECK(lset != NULL, nolset);

	for (i = 0; i < lset->N; i++)
		classify(descr, cls, lset->examples[i], out);

	free_description(descr);
	free_and_set_NULL(descr);
	free_classifier(cls);
	free_example_set(lset);
	free_and_set_NULL(lset);
	return 0;
nolset:
	free_example_set(lset);
	free_and_set_NULL(lset);
fail:
	free_classifier(cls);
	free_description(descr);
	free_and_set_NULL(descr);
nodescr:
	return set_error(EINVAL);
}

void classify(const struct description *descr,
		const struct classifier *cls, const struct example *ex,
		FILE *out)
{
	int i;

	if (cls->C == 0) {
		fprintf(out, "%s\n", descr->classes[cls->id]);
		return;
	}

	if (descr->attribs[cls->id]->type == NUMERIC) {
		for (i = 0; i < cls->C - 1; i++)
			if (ex->attr_ids[cls->id] < cls->values[i]) {
				classify(descr, cls->cls[i], ex, out);
				return;
			}
		classify(descr, cls->cls[i], ex, out);
		return;
	}

	for (i = 0; i < cls->C; i++)
		if (ex->attr_ids[cls->id] == cls->values[i]) {
			classify(descr, cls->cls[i], ex, out);
			return;
		}
	fprintf(stderr, "TODO %s %d\n", __FILE__, __LINE__);
}

