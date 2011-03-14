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
static int error_in_set = 0;

/**
 * @brief Local variable used to detect the missing value attributes count.
 */
static int missing_count = 0;

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
		attr->ptr[i] = strdup(tmp);
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
		if (strncmp(string, attr->ptr[index], l) == 0)
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
			return 0;

	CHECK(missing_count < MISS_COUNT, fail);
	set->missing[missing_count++] = index;
	return 0;

fail:
	return 1;
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
		if (strncmp(tmp, "?", 1) == 0)
			CHECK(record_missing(i, set) == 0, fail);
		else {
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
		free_and_set_NULL(ptr->ptr[i]);

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

int set_error(int err)
{
	errno = err;
	return err;
}

