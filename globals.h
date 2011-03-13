/*!
 * @file globals.h
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

#ifndef _GLOBALS_H
#define _GLOBALS_H

#define UNKNOWN_VALUE 0 /**< @brief option not set or value not known */

#define NUM_DIV 1 /**< @brief handle numeric attributes by a single binary split */
#define NUM_FULL 2 /**< @brief full handling of numeric attributes */

#define MISS_MAJ 1 /**< @brief replace missing attributes with the majority */
#define MISS_PRB 2 /**< @brief use probabilities to guess the missing values */
#define MISS_ID3 3 /**< @brief use ID3 to detect values for missing values */

#endif

