/*!
 * @file id3math.h
 * @brief Math functions used for id3.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Math functions used for id3.
 *
 * Contains math functions used in ID3.
 *
 * @section QUOTE
 * AI is an engineering discipline built on an unfinished science. (Matt
 * Ginsberg)
 *
 * @bug No known bugs.
 */

#ifndef _ID3MATH_H
#define _ID3MATH_H

/**
 * @brief Computes the log2 of a value.
 *
 * @param x The value
 * @return log2(x)
 */
double log2(double x);

/**
 * @brief Computes the information entropy associated with a probability.
 *
 * @param p The probability (in [0..1], otherwise trimmed)
 * @return the entropy (informational quantity)
 */
double entropy(double p);

#endif

