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

/**
 * @brief Bootstraps the outputing process.
 *
 * @param id3 File to read classifier from
 * @param out Where to output
 * @param graph_mode Graph output mode
 * @return 0 on success, error code otherwise
 */
int id3_output_graph(FILE *id3, FILE *out, int graph_mode);

#endif

