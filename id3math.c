/*!
 * @file id3math.c
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

#include <math.h>

#include "globals.h"
#include "id3math.h"

/**
 * @brief Static value used to detect when something is 0 or below.
 */
static double ZERO = 1e-5l;

double log2(double x)
{
	if (x < ZERO)
		return 0;
	return log(x) / log(2);
}

double entropy(double p)
{
	if (p < ZERO)
		return 0;
	if (p > 1)
		return 0;
	return -p * log2(p);
}

