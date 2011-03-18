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

#ifndef _ID3GRAPH_H
#define _ID3GRAPH_H

#define TABS 2 /**< @brief Tab stop for ASCII art display */

/**
 * @brief Bootstraps the outputing process.
 *
 * @param id3 File to read classifier from
 * @param out Where to output
 * @param graph_mode Graph output mode
 * @return 0 on success, error code otherwise
 */
int id3_output_graph(FILE *id3, FILE *out, int graph_mode);

/**
 * @brief Outputs a classifier to graph form (depending on output type).
 *
 * Only a dispatch to the good method.
 *
 * @param descr The description to use
 * @param cls The classifier
 * @param graph_mode Mode of output
 * @param out Where to output
 */
static void graph_classifier(const struct description *descr,
		const struct classifier *cls, int graph_mode, FILE* out);

/**
 * @brief Outputs the classifier to ASCII art.
 *
 * @param descr The description to use
 * @param cls The classifier
 * @param out Where to output
 * @param level Level of indentation
 */
static void graph_ascii(const struct description *descr,
		const struct classifier *cls, FILE* out, int level);

/**
 * @brief Outputs the classifier using Dot.
 *
 * @param descr The description to use
 * @param cls The classifier
 * @param out Where to output
 */
static void graph_dot(const struct description *descr,
		const struct classifier *cls, FILE* out);

/**
 * @brief Outputs the classifier to Scheme/Lisp constructs.
 *
 * @param descr The description to use
 * @param cls The classifier
 * @param out Where to output
 */
static void graph_scheme(const struct description *descr,
		const struct classifier *cls, FILE* out);

/**
 * @brief Outputs the classifier to if-then constructs
 *
 * @param descr The description to use
 * @param cls The classifier
 * @param out Where to output
 */
static void graph_ifthen(const struct description *descr,
		const struct classifier *cls, FILE* out);

#endif

