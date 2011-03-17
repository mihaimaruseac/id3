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
 * This function is recursive. The ending condition is done by the tag
 * variable, which is used to filter the remaining examples (and to create the
 * id3 tree nodes).
 *
 * @param descr The description of the problem.
 * @param lset The learning set.
 * @param num_handle How to handle numeric arguments.
 * @param tag Tag used to filter the learning set.
 */
static void id3_learn(const struct description *descr,
		const struct example_set *lset, int num_handle, int tag);

/**
 * @brief Fills the missing spots.
 *
 * @param descr The description of the problem.
 * @param lset The learning set.
 * @param missing_handle How to handle missing arguments.
 */
static void id3_treat_missing(const struct description *descr,
		struct example_set *lset, int missing_handle);

/**
 * @brief Builds indexes for numeric attributes.
 *
 * The indexes are used to implement handling of numeric attributes and to
 * sort the input set, if needed. Subsets of the input will keep the same
 * index, although some values may be missing in them.
 *
 * @param descr The description of the problem.
 * @param lset The learning set
 */
static void id3_build_index(const struct description *descr,
		const struct example_set *lset);

/**
 * @brief Computes the count of eamples having a specified filter value.
 *
 * @param lset The learning set
 * @param tag The filter value
 * @return Count of examples with specified tag
 */
static int id3_filtered_count(const struct example_set *lset, int tag);

/**
 * @brief Computes the information given by a set of examples (I_{DT})
 *
 * @param descr The description of the problem
 * @param lset The learning set
 * @param tag Tag to filter the learning set
 * @param count Count of filtered values
 * @return I_{DT}
 */
static double id3_I_decision_tree(const struct description *descr,
		const struct example_set *lset, int tag, int count);

/**
 * @brief Computes the expected average information obtained by splitting on a
 * numeric attribute.
 *
 * @param descr The description of the problem
 * @param lset The learning set
 * @param index Index of attribute
 * @param num_handle How to handle numeric values
 * @param tag Tag to filter the learning set
 * @param count Count of filtered values
 * @return E_A
 */
static double test_split_numeric(const struct description *descr,
		const struct example_set *lset,
		int index, int num_handle, int tag, int count);

/**
 * @brief Computes the expected average information obtained by splitting on a
 * discrete attribute.
 *
 * @param descr The description of the problem
 * @param lset The learning set
 * @param index Index of attribute
 * @param tag Tag to filter the learning set
 * @param count Count of filtered values
 * @return E_A
 */
static double test_split_discrete(const struct description *descr,
		const struct example_set *lset,
		int index, int tag, int count);

#endif

