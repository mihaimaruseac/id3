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

int set_error(int err)
{
	errno = err;
	return err;
}
