/*!
 * @file id3graph.c
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
	CHECK(descr != NULL, nodescr);
	cls = read_classifier(id3);
	CHECK(cls != NULL, fail);
	graph_classifier(descr, cls, graph_mode, out);

	free_description(descr);
	free_and_set_NULL(descr);
	free_classifier(cls);
	return 0;
fail:
	free_classifier(cls);
	free_description(descr);
	free_and_set_NULL(descr);
nodescr:
	return set_error(EINVAL);
}

void graph_classifier(const struct description *descr,
		const struct classifier *cls, int graph_mode, FILE* out)
{
	switch (graph_mode) {
	case TREE_ASCII:
		graph_ascii(descr, cls, out, 0);
		break;
	case TREE_DOT:
		graph_dot(descr, cls, out);
		break;
	case TREE_SCHEME:
		graph_scheme(descr, cls, out);
		break;
	case TREE_IFTHEN:
		graph_ifthen(descr, cls, out);
		break;
	}
}

void graph_ascii(const struct description *descr,
		const struct classifier *cls, FILE* out, int level)
{
	int i, aid;
	char *name, *val;

	aid = cls->id;
	name = descr->attribs[aid]->name;

	if (cls->C == 0) {
		fprintf(out, "%*c==> %s\n", TABS * level + 1, ' ',
				descr->classes[cls->id]);
		return;
	}
	if (descr->attribs[aid]->type == NUMERIC) {
		for (i = 0; i < cls->C - 1; i++) {
			fprintf(out, "%*c%s < %d\n", TABS * level + 1,
					' ', name, cls->values[i]);
			graph_ascii(descr, cls->cls[i], out, level + 1);
		}
		fprintf(out, "%*c%s >= %d\n", TABS * level + 1, ' ',
				name, cls->values[i - 1]);
		graph_ascii(descr, cls->cls[i], out, level + 1);
	} else
		for (i = 0; i < cls->C; i++) {
			val = (char *)
				descr->attribs[aid]->
				ptr[cls->values[i]];
			fprintf(out, "%*c%s = %s\n", TABS * level + 1,
					' ', name, val);
			graph_ascii(descr, cls->cls[i], out, level + 1);
		}
}

void graph_dot(const struct description *descr,
		const struct classifier *cls, FILE* out)
{
	fprintf(stderr, "TODO output dot %s %d\n", __FILE__, __LINE__);
}

void graph_scheme(const struct description *descr,
		const struct classifier *cls, FILE* out)
{
	fprintf(stderr, "TODO output scheme %s %d\n", __FILE__, __LINE__);
}

void graph_ifthen(const struct description *descr,
		const struct classifier *cls, FILE* out)
{
	fprintf(stderr, "TODO output if %s %d\n", __FILE__, __LINE__);
}

