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

int dot_output(const struct description *descr,
		const struct classifier *cls, FILE *out, int level)
{
	char *name, *dname, *aname;
	int i, l, ll, type;

	if (cls->C == 0) {
		name = descr->classes[cls->id];
		fprintf(out, "%s%d [label=%s];\n", name, level, name);
		return level + 1;
	}

	name = descr->attribs[cls->id]->name;
	type = descr->attribs[cls->id]->type;
	fprintf(out, "%s%d [label=%s][shape=box];\n", name, level, name);
	l = level++;
	for (i = 0; i < cls->C; i++) {
		ll = level;
		level = dot_output(descr, cls->cls[i], out, level);
		if (cls->cls[i]->C == 0)
			dname = descr->classes[cls->cls[i]->id];
		else
			dname = descr->attribs[cls->cls[i]->id]->name;
		if (type == NUMERIC)
			if (i < cls->C - 1)
				fprintf(out, "%s%d -- %s%d [label=\"<%d\"]"
					"[fontsize=10];\n", name, l, dname,
					ll, cls->values[i]);
			else
				fprintf(out, "%s%d -- %s%d[label=\">=%d\"]"
					"[fontsize=10];\n", name, l, dname,
					ll, cls->values[i - 1]);
		else {
			aname = (char *)
				descr->attribs[cls->id]->ptr[cls->values[i]];
			fprintf(out, "%s%d -- %s%d[label=\"%s\"]"
				"[fontsize=10];\n", name, l, dname, ll, aname);
		}
	}
	return level;
}

void graph_dot(const struct description *descr,
		const struct classifier *cls, FILE *out)
{
	fprintf(stderr, "TODO output dot %s %d\n", __FILE__, __LINE__);
	fprintf(out, "graph {\n");
	dot_output(descr, cls, out, 0);
	fprintf(out, "}\n");
}

void g_sch_print(const struct description *descr,
		const struct classifier *cls, FILE *out, int level)
{
	if (cls->C == 0)
		fprintf(out, "'%s", descr->classes[cls->id]);
	else {
		fprintf(out, "\n");
		g_sch_if(descr, cls, out, level + 2);
		fprintf(out, "%*c", TABS * (1 + level), ' ');
	}
}

void g_sch_num_branches(const struct description *descr,
		const struct classifier *cls, FILE *out, int level)
{
	int i, aid;
	char *name;
	int val;

	aid = cls->id;
	name = descr->attribs[aid]->name;
	i = cls->C - 2;
	val = cls->values[i];
	fprintf(out, "%*c((>= %s %d) ", TABS * level, ' ', name, val);
	g_sch_print(descr, cls->cls[i], out, level);
	fprintf(out, ")\n");
	for (; i >= 0; i--) {
		val = cls->values[i];
		fprintf(out, "%*c((< %s %d) ", TABS * level, ' ', name, val);
		g_sch_print(descr, cls->cls[i], out, level);
		fprintf(out, ")\n");
	}
}

void g_sch_dsc_branches(const struct description *descr,
		const struct classifier *cls, FILE *out, int level)
{
	int i, aid;
	char *val;

	aid = cls->id;
	for (i = 0; i < cls->C; i++) {
		val = (char *)descr->attribs[aid]->ptr[cls->values[i]];
		fprintf(out, "%*c((eqv? %s '%s) ", TABS * level, ' ',
				descr->attribs[aid]->name, val);
		g_sch_print(descr, cls->cls[i], out, level);
		fprintf(out, ")\n");
	}
}

void g_sch_cond(const struct description *descr,
		const struct classifier *cls, FILE *out, int level)
{
	fprintf(out, "%*c(\n", TABS * level, ' ');
	if (descr->attribs[cls->id]->type == NUMERIC)
		g_sch_num_branches(descr, cls, out, level + 1);
	else
		g_sch_dsc_branches(descr, cls, out, level + 1);
	fprintf(out, "%*c)\n", TABS * level, ' ');
}

void g_sch_if(const struct description *descr,
		const struct classifier *cls, FILE *out, int level)
{
	fprintf(out, "%*c(cond\n", TABS * level, level ? ' ' : 0);
	g_sch_cond(descr, cls, out, level + 1);
	fprintf(out, "%*c)\n", TABS * level, level ? ' ' : 0);
}

void graph_scheme(const struct description *descr,
		const struct classifier *cls, FILE* out)
{
	g_sch_if(descr, cls, out, 0);
}

void graph_ifthen(const struct description *descr,
		const struct classifier *cls, FILE* out)
{
	fprintf(stderr, "TODO output if %s %d\n", __FILE__, __LINE__);
}

