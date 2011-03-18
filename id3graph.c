/*!
 * @file id3graph.h
 * @brief Displaying id3 classifiers
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Displaying id3 classifiers
 *
 * Contains the definitions for the various modes to display an id3
 * classifier.
 *
 * @section QUOTE
 * The wheel needs reinventing, but not just yet. (Nir Oren)
 *
 * @bug No known bugs.
 */

#include "globals.h"
#include "id3graph.h"

int id3_output_graph(FILE *id3, FILE *out, int graph_mode)
{
	struct description *descr;
	struct classifier *cls;

	descr = read_description_file(id3);
	cls = read_classifier(id3);
	CHECK(cls != NULL, fail);

	free_description(descr);
	free_and_set_NULL(descr);
	free_classifier(cls);
	return 0;
fail:
	free_description(descr);
	free_and_set_NULL(descr);
	free_classifier(cls);
	return set_error(EINVAL);
}

