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

int id3_learn(int num_handle, int missing_handle, FILE *attr_file, FILE
		*learn_file, FILE *id3_file)
{
	struct description *descr;

	descr = read_description_file(attr_file);
	if (descr == NULL)
		return set_error(EINVAL); /* invalid file received */

	dump_description(descr, stdout);

	free_description(descr);
	free_and_set_NULL(descr);

	return EXIT_SUCCESS;
}

