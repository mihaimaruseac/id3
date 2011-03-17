/*!
 * @file id3missing.h
 * @brief Treating of missing values.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Treating of missing values.
 *
 * Contains the various implementations of routines to treat missing
 * attributes from the description.
 *
 * @section QUOTE
 * Intelligence is what you use when you don't know what to do. (Jean Piaget)
 *
 * @bug No known bugs.
 */

#ifndef _ID3MISS_H
#define _ID3MISS_H

/**
 * @brief Treats missing numeric attributes by filling with the average of the
 * other values.
 *
 * @param descr The description
 * @param lset The learning set
 * @param attr_index Index of attribute with missing values
 * @param miss_index Index in missing structure.
 */
void numeric_maj_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index);

/**
 * @brief Treats missing numeric attributes by using probability theory.
 *
 * Because the numitors in the formulas are always constant, this function
 * uses only integer operations to give better results.
 *
 * @param descr The description
 * @param lset The learning set
 * @param attr_index Index of attribute with missing values
 * @param miss_index Index in missing structure.
 */
void numeric_prb_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index);

/**
 * @brief Treats missing numeric attributes by using ID3 to classify.
 *
 * Spawns a new process, after writing this description to files in a new
 * format!
 *
 * @param descr The description (used when writing the informations)
 * @param lset The learning set
 * @param attr_index Index of attribute with missing values
 * @param miss_index Index in missing structure.
 */
void numeric_id3_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index);

/**
 * @brief Treats missing discrete attributes by filling with the average of the
 * other values.
 *
 * @param descr The description
 * @param lset The learning set
 * @param attr_index Index of attribute with missing values
 * @param miss_index Index in missing structure.
 */
void discrete_maj_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index);

/**
 * @brief Treats missing discrete attributes by using probability theory.
 *
 * Because the numitors in the formulas are always constant, this function
 * uses only integer operations to give better results.
 *
 * @param descr The description
 * @param lset The learning set
 * @param attr_index Index of attribute with missing values
 * @param miss_index Index in missing structure.
 */
void discrete_prb_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index);

/**
 * @brief Treats missing discrete attributes by using ID3 to classify.
 *
 * Spawns a new process, after writing this description to files in a new
 * format!
 *
 * @param descr The description (used when writing the informations)
 * @param lset The learning set
 * @param attr_index Index of attribute with missing values
 * @param miss_index Index in missing structure.
 */
void discrete_id3_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index);

#endif


