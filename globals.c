/*!
 * @file globals.c
 * @brief Global utilities and functions.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Global utilities and functions.
 *
 * Contains the definition of global functions used in the assignment.
 *
 * @section QUOTE
 * Artificial Intelligence, IT'S HERE. (Business Week cover,  July 9, 1984.)
 *
 * @bug No known bugs.
 */

#include "globals.h"

/**
 * @brief Local global variable used to detect errors in example or testing
 * sets.
 */
static int error_in_set;

/**
 * @brief Local variable used to detect the missing value attributes count.
 */
static int missing_count;

/**
 * @brief Local global variable used to detect the missing values while
 * printing.
 */
static int missing_indexes[MISS_COUNT];

void *free_and_set_NULL(void *ptr)
{
	if (ptr)
		free(ptr);
	return NULL;
}

struct attribute *read_attribute(FILE *file)
{
	struct attribute *attr;
	char *tmp = NULL;
	int i;

	attr = calloc(1, sizeof(*attr));

	CHECK(fscanf(file, "%ms", &tmp) == 1, fail);
	attr->name = strdup(tmp);
	tmp = free_and_set_NULL(tmp);

	CHECK(fscanf(file, "%ms", &tmp) == 1, fail);
	attr->type = UNKNOWN_VALUE;
	if (strncmp(tmp, "numeric", 7) == 0)
		attr->type = NUMERIC;
	else if (strncmp(tmp, "discret", 7) == 0)
		attr->type = DISCRETE;
	tmp = free_and_set_NULL(tmp);

	CHECK(attr->type != NUMERIC, ok);
	CHECK(fscanf(file, "%d", &attr->C) == 1, fail);
	attr->ptr = calloc(attr->C, sizeof(attr->ptr[0]));

	for (i = 0; i < attr->C; i++) {
		CHECK(fscanf(file, "%ms", &tmp) == 1, fail);
		attr->ptr[i] = (int)strdup(tmp);
		tmp = free_and_set_NULL(tmp);
	}

ok:
	return attr;

fail:
	free_attribute(attr);
	free_and_set_NULL(attr);
	return free_and_set_NULL(tmp);
}

struct description *read_description_file(FILE *file)
{
	struct description *desc;
	int i;
	char *tmp = NULL;

	desc = calloc(1, sizeof(*desc));

	CHECK(fscanf(file, "%d", &desc->K) == 1, fail);

	desc->classes = calloc(desc->K, sizeof(desc->classes[0]));
	for (i = 0; i < desc->K; i++) {
		CHECK(fscanf(file, "%ms", &tmp) == 1, fail);
		desc->classes[i] = strdup(tmp);
		tmp = free_and_set_NULL(tmp);
	}

	CHECK(fscanf(file, "%d", &desc->M) == 1, fail);
	desc->attribs = calloc(desc->M, sizeof(desc->attribs[0]));
	for (i = 0; i < desc->M; i++) {
		desc->attribs[i] = read_attribute(file);
		CHECK(desc->attribs[i] != NULL, fail);
	}

	return desc;

fail:
	free_and_set_NULL(tmp);
	free_description(desc);
	return free_and_set_NULL(desc);
}

struct example_set *read_learning_file(FILE *file,
		const struct description *descr)
{
	missing_count = 0;
	return read_set(file, 1, descr);
}

struct example_set *read_testing_file(FILE *file,
		const struct description *descr)
{
	return read_set(file, 0, descr);
}

struct example_set *read_set(FILE *file, int learning,
		const struct description *descr)
{
	struct example_set *set;
	int i;

	set = calloc(1, sizeof(*set));

	CHECK(fscanf(file, "%d", &set->N) == 1, fail);

	set->examples = calloc(set->N, sizeof(set->examples[0]));
	for (i = 0; i < set->N; i++) {
		set->examples[i] = read_example(file, learning, descr, set);
		CHECK(set->examples[i] != NULL, fail);
	}

	return set;

fail:
	free_example_set(set);
	return free_and_set_NULL(set);
}

int get_index_from_descr(const char *string,
		const struct attribute *attr)
{
	int index, l;

	if (attr->type == NUMERIC) {
		CHECK(sscanf(string, "%d", &index) == 1, fail);
		return index;
	}

	l = strlen(string);
	for (index = 0; index < attr->C; index++)
		if (strncmp(string, (char *)attr->ptr[index], l) == 0)
			return index;
fail:
	error_in_set = 1;
	return 0;
}

int record_missing(int index, struct example_set *set)
{
	int i;

	for (i = 0; i < missing_count; i++)
		if (set->missing[i] == index)
			return 1 << i;

	CHECK(missing_count < MISS_COUNT, fail);
	set->missing[missing_count++] = index;
	return 1 << (missing_count - 1);

fail:
	return 0;
}

struct example *read_example(FILE *file, int learning,
		const struct description *descr,
		struct example_set *set)
{
	struct example *ex;
	char *tmp = NULL;
	int i, l;

	ex = calloc(1, sizeof(*ex));
	ex->attr_ids = calloc(descr->M, sizeof(ex->attr_ids[0]));

	for (i = 0; i < descr->M; i++) {
		CHECK(fscanf(file, "%ms", &tmp) == 1, fail);
		if (strncmp(tmp, "?", 1) == 0) {
			l = record_missing(i, set);
			CHECK(l != 0, fail);
			ex->miss |= l;
		} else {
			error_in_set = 0;
			ex->attr_ids[i] = get_index_from_descr(tmp,
					descr->attribs[i]);
			CHECK(error_in_set == 0, fail);
		}
		tmp = free_and_set_NULL(tmp);
	}

	if (learning) {
		CHECK(fscanf(file, "%ms", &tmp) == 1, fail);
		l = strlen(tmp);
		for (i = 0; i < descr->K; i++)
			if (strncmp(tmp, descr->classes[i], l) == 0) {
				ex->class_id = i;
				break;
			}
		CHECK(i < descr->K, fail);
		tmp = free_and_set_NULL(tmp);
	}

	return ex;

fail:
	free_example(ex);
	free_and_set_NULL(ex);
	return free_and_set_NULL(tmp);
}

struct classifier *read_classifier(FILE *file)
{
	struct classifier *cls, *tmp;
	int i;

	cls = calloc(1, sizeof(*cls));
	CHECK(fscanf(file, "%d%d%d", &cls->tag, &cls->id, &cls->C) == 3, fail);
	cls->values = calloc(cls->C, sizeof(cls->values[0]));
	cls->cls = calloc(cls->C, sizeof(cls->cls[0]));
	for (i = 0; i < cls->C; i++)
		CHECK(fscanf(file, "%d", &cls->values[i]) == 1, fail);
	for (i = 0; i < cls->C; i++) {
		tmp = read_classifier(file);
		CHECK(tmp != NULL, fail);
		cls->cls[i] = tmp;
	}
	return cls;
fail:
	free_classifier(cls);
	return NULL;
}

void write_attribute(const struct attribute *attr, FILE *file)
{
	int i;

	fprintf(file, "%s ", attr->name);
	if (attr->type == NUMERIC) {
		fprintf(file, "numeric\n");
		return;
	}

	fprintf(file, "discret %d", attr->C);
	for (i = 0; i < attr->C; i++)
		fprintf(file, " %s", (char *)attr->ptr[i]);
	fprintf(file, "\n");
}

void write_description(const struct description *descr, FILE *file)
{
	int i;

	fprintf(file, "%d\n", descr->K);
	fprintf(file, "%s", descr->classes[0]);
	for (i = 1; i < descr->K; i++)
		fprintf(file, " %s", descr->classes[i]);
	fprintf(file, "\n%d\n", descr->M);
	for (i = 0; i < descr->M; i++)
		write_attribute(descr->attribs[i], file);
}

void write_set(const struct example_set *set,
	const struct description *descr,
	FILE *file)
{
	int i;

	for (i = 0; i < MISS_COUNT; i++)
		missing_indexes[i] = set->missing[i];

	fprintf(file, "%d\n", set->N);
	for (i = 0; i < set->N; i++)
		write_example(set->examples[i], descr, file);
}

int missing_value(const struct example *ex, int index)
{
	int i;

	for (i = 0; i < MISS_COUNT; i++)
		if (index == missing_indexes[i] && MISS_INDEX(ex->miss, i))
			return 1;
	return 0;
}

void write_example(const struct example *ex,
	const struct description *descr,
	FILE *file)
{
	int i;

	for (i = 0; i < descr->M; i++)
		if (missing_value(ex, i))
			fprintf(file, "? ");
		else if (descr->attribs[i]->type == NUMERIC)
			fprintf(file, "%d ", ex->attr_ids[i]);
		else
			fprintf(file, "%s ",
				(char *)descr->attribs[i]->
				ptr[ex->attr_ids[i]]);
	fprintf(file, "%s\n", descr->classes[ex->class_id]);
}

void write_classifier(const struct classifier *cls, FILE *file)
{
	int i;

	if (cls == NULL)
		return;

	fprintf(file, "%d %d %d", cls->tag, cls->id, cls->C);
	for (i = 0; i < cls->C; i++)
		fprintf(file, " %d", cls->values[i]);
	fprintf(file, "\n");

	for (i = 0; i < cls->C; i++)
		write_classifier(cls->cls[i], file);
}

void write_id3_temp_file(const struct description *descr,
		const struct classifier *cls,
		FILE *file)
{
	write_description(descr, file);
	write_classifier(cls, file);
}

void clear_filter_info(struct example_set *lset)
{
	int i;

	for (i = 0; i < lset->N; i++)
		lset->examples[i]->filter = 0;
}

void free_attribute(struct attribute *ptr)
{
	int i;

	if (ptr == NULL)
		return;
	if (ptr->name == NULL)
		return;
	free_and_set_NULL(ptr->name);

	if (ptr->ptr == NULL)
		return;

	CHECK(ptr->type != NUMERIC, ok);
	for (i = 0; i < ptr->C; i++)
		free_and_set_NULL((char *)ptr->ptr[i]);

ok:
	free_and_set_NULL(ptr->ptr);
}

void free_description(struct description *ptr)
{
	int i;

	if (ptr->classes == NULL)
		return;

	for (i = 0; i < ptr->K; i++)
		free_and_set_NULL(ptr->classes[i]);
	free(ptr->classes);

	if (ptr->attribs == NULL)
		return;

	for (i = 0; i < ptr->M; i++) {
		free_attribute(ptr->attribs[i]);
		free_and_set_NULL(ptr->attribs[i]);
	}
	free(ptr->attribs);
}

void free_example_set(struct example_set *ptr)
{
	int i;

	if (ptr->examples == NULL)
		return;

	for (i = 0; i < ptr->N; i++) {
		free_example(ptr->examples[i]);
		free_and_set_NULL(ptr->examples[i]);
	}
	free(ptr->examples);
}

void free_example(struct example *ptr)
{
	if (ptr == NULL)
		return;

	free_and_set_NULL(ptr->attr_ids);
}

void free_classifier(struct classifier *ptr)
{
	int i;

	if (ptr == NULL)
		return;

	for (i = 0; i < ptr->C; i++)
		free_classifier(ptr->cls[i]);
	free_and_set_NULL(ptr->cls);
	free_and_set_NULL(ptr->values);
	free_and_set_NULL(ptr);
}

int set_error(int err)
{
	errno = err;
	return err;
}

