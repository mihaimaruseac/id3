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
int id3_learn(int num_handle, int missing_handle, FILE *attr_file, FILE
		*learn_file, FILE *id3_file);

#endif

