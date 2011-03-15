/*!
 * @file fillmissing.c
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

#include "globals.h"
#include "id3missing.h"

void numeric_maj_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void numeric_prb_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void numeric_id3_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void discrete_maj_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void discrete_prb_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

void discrete_id3_fill_missing(const struct description *descr,
		struct example_set *lset, const int attr_index,
		const int miss_index)
{
	fprintf(stderr, "TODO%d\n", __LINE__);
}

