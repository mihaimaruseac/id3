/*!
 * @file id3learn.h
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

#ifndef _ID3LEARN_H
#define _ID3LEARN_H

/**
 * @brief Bootstraps the learning phase by reading data from the input files.
 *
 * It is simply a wrapper function around more useful functions, defined in
 * this file.
 *
 * @param num_handle How to handle numeric arguments.
 * @param missing_handle How to handle missing arguments.
 * @param attr_file Filename for the attribute description file.
 * @param learn_file Filename for the example set file.
 * @param id3_file Filename used to output the classifier in a raw form.
 * @return The exit code for the learning process.
 */
int id3_learn_bootstrap_file(int num_handle, int missing_handle,
		FILE *attr_file, FILE *learn_file, FILE *id3_file);

/**
 * @brief The learning process (wrapper function).
 *
 * @param descr The description of the problem.
 * @param lset The learning set.
 * @param num_handle How to handle numeric arguments.
 */
static void id3_learn(const struct description *descr,
		const struct example_set *lset, int num_handle);

/**
 * @brief Fills the missing spots.
 *
 * @param descr The description of the problem.
 * @param lset The learning set.
 * @param missing_handle How to handle missing arguments.
 */
static void id3_treat_missing(const struct description *descr,
		struct example_set *lset, int missing_handle);

#endif

