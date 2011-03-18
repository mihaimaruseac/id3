/*!
 * @file id3test.h
 * @brief Classifying instances using an id3 tree.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Classifying instances using an id3 tree.
 *
 * Contains functions used to classify examples with an id3 classifier.
 *
 * @section QUOTE
 * You've got to stop looking at the big picture. (Gunnar Grimnes)
 *
 * @bug No known bugs.
 */

#ifndef _ID3TEST_H
#define _ID3TEST_H

/**
 * @brief Bootstraps the classifying phase.
 *
 * @param id3 File containing the classifier
 * @param test File containing the test instances
 * @param out File to output to.
 * @return error status or 0
 */
int id3_test(FILE *id3, FILE *test, FILE *out);

#endif

