/*!
 * @file globals.h
 * @brief Global utilities and functions.
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Global utilities and functions.
 *
 * Contains the definition of global functions used in the assignment. It also
 * includes the most important headers, used throughout the project.
 *
 * @section QUOTE
 * Artificial Intelligence, IT'S HERE. (Business Week cover,  July 9, 1984.)
 *
 * @bug No known bugs.
 */

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Macro used to assert error conditions.
 *
 * Tests the condition and goes to a specific label if the condition is false
 * (an erroneous call).
 */
#define CHECK(cond, label)\
	do {\
		if (!(cond)) {\
			goto label;\
		} \
	} while (0)

#define UNKNOWN_VALUE -1 /**< @brief option not set or value not known */

#define NUM_DIV 1 /**< @brief handle numeric attributes by binary split */
#define NUM_FULL 2 /**< @brief full handling of numeric attributes */

#define MISS_MAJ 1 /**< @brief replace missing attributes with the majority */
#define MISS_PRB 2 /**< @brief use probabilities to guess the missing values */

#define TREE_ASCII 1 /**< @brief output ID3 tree in ASCII */
#define TREE_DOT 2 /**< @brief output ID3 tree using Dot */
#define TREE_SCHEME 3 /**< @brief output ID3 tree as Scheme/Lisp functions */

#define MISS_COUNT 2 /**< @brief max number of missing columns */

/**
 * @brief Macro used to determine if a bit is set in a flag.
 *
 * It is used to detect which attribute is missing in one example.
 */
#define MISS_INDEX(flag, index) ((flag) & (1 << (index)))

/**
 * @brief Macro used to shortcircuit a loop if a condition is true.
 */
#define SKIPIF(cond) if (cond) continue

/**
 * @brief Increments a variable if a condition is true.
 */
#define INCRIF(cond, var)\
	do {\
		if (cond)\
			var++;\
	} while (0)

/**
 * @brief Double division (with conversion).
 */
#define DIV(a, b) (((a) + 0.0l) / b)

/**
 * @brief Enumeration of possible types of an attribute.
 */
enum attr_type {
	/** Numeric attribute */
	NUMERIC,
	/** Discrete attribute */
	DISCRETE
};

/**
 * @brief Structure used to represent a single attribute.
 *
 * If the attribute is numeric, the ptr vector will be used only after reading
 * the learning example. It will be an index into the example data, used to
 * sort in ascending order the example data by this attribute. After
 * discretization, this vector will contain the limits used in the
 * discretization.
 *
 * Otherwise, the ptr will point to a vector of names, used for the discrete
 * values. Each name can be obtained by converting the integer to a char* and
 * looking at the resulting memory location.
 */
struct attribute {
	/** Name of the attribute */
	char *name;
	/** Type-dependent pointer (see description) */
	long long *ptr;
	/** Length of vector pointed to by ptr */
	int C;
	/** @param type Type of attribute */
	enum attr_type type;
};

/**
 * @brief Structure used to describe the learning problem.
 *
 * It contains the number of classes and their names and the number of
 * attributes and their values.
 */
struct description {
	/** Number of classes */
	int K;
	/** Names of classes */
	char **classes;
	/** Number of attributes */
	int M;
	/** Description of attributes */
	struct attribute **attribs;
};

/**
 * @brief Structure used to represent an example from the learning set or from
 * the problem set.
 *
 * This structure contains an integer to represent the class. Also, to
 * represent the attributes, we will use integers. Both values will point into
 * the description structure to the right value.
 *
 * To represent a missing attribute value we use a flag which will have a bit
 * set if the attribute corresponding to the index from struct
 * example_set->missing is missing in this example (using MISS_INDEX macro).
 */
struct example {
	/** ID of class (as given by struct description) */
	int class_id;
	/** ID of each attribute (as given by struct description and struct
	 * attribute)
	 */
	int *attr_ids;
	/** Flag for missing values */
	int miss;
	/** This flag may be used to represent the fact that this example
	 * should be discarded in one loop. It is the user's job to update
	 * this flag to relevant values. The only guarantee is that this flag
	 * is set to 0 when this structure is created.
	 */
	int filter;
};

/**
 * @brief Structure representing the learning set.
 *
 * The missing vector contains the indexes of missing value attributes.
 */
struct example_set {
	/** Number of samples */
	int N;
	/** Examples */
	struct example **examples;
	/** Missing columns */
	int missing[MISS_COUNT];
};

/**
 * @brief Structure representing the id3 classifier (or id3 tree).
 */
struct classifier {
	/** Id of classifier (tag used to generate it) */
	int tag;
	/** Id of attribute or class */
	int id;
	/** Count of branches or 0 if class */
	int C;
	/** Values to the given branch (if numeric, last value is always 0) */
	int *values;
	/** Branches (size is given by C) */
	struct classifier **cls;
};

/**
 * @brief Reads the description for one problem.
 *
 * @param file Problem description file.
 * @return Pointer to the description of the problem.
 */
struct description *read_description_file(FILE *file);

/**
 * @brief Reads the learning set for one problem.
 *
 * Wrapper function around read_set which is used to read both the learning
 * set and the test set.
 *
 * @param file Learning set description file.
 * @param descr The description of the entire model.
 * @return The learning set.
 */
struct example_set *read_learning_file(FILE *file,
		const struct description *descr);

/**
 * @brief Reads the testing set for one problem.
 *
 * Wrapper function around read_set which is used to read both the learning
 * set and the test set.
 *
 * @param file Learning set description file
 * @param descr The description of the entire model.
 * @return The learning set.
 */
struct example_set *read_testing_file(FILE *file,
		const struct description *descr);

/**
 * @brief Reads the classifier stored in a file.
 *
 * @return the classifier.
 */
struct classifier *read_classifier(FILE *file);

/**
 * @brief Writes the description to a file.
 *
 * File can be standard output or anything else.
 *
 * @param descr Description to write
 * @param file File to write to.
 */
void write_description(const struct description *descr, FILE *file);

/**
 * @brief Writes an example set to a file.
 *
 * @param set Set to write.
 * @param descr Description to use while writing.
 * @param file File to write to.
 */
void write_set(const struct example_set *set,
		const struct description *descr,
		FILE *file);

/**
 * @brief Writes a classifier to a file.
 *
 * @param cls The classifier
 * @param file File to write to.
 */
void write_classifier(const struct classifier *cls, FILE *file);

/**
 * @brief Writes the id3 intermediate representation to a file.
 *
 * The intermediate representation consists of the classifier and the
 * description. This file will also contain a copy of the description because
 * it is needed when building the tree or classifying other instances at a
 * later time.
 *
 * @param descr Description of the problem
 * @param cls The classifier
 * @param file File to write to.
 */
void write_id3_temp_file(const struct description *descr,
		const struct classifier *cls,
		FILE *file);

/**
 * @brief Deletes the filtering information associated by each example from
 * the example set.
 *
 * As you recall (see the struct example documentation), the filter flag can
 * be used to implement filtering of examples from the example set in a opaque
 * way to other users of the same structure. However, to ensure consistency,
 * each user must call this function to reset the filter value to the original
 * value.
 */
void clear_filter_info(const struct example_set *lset);

/**
 * @brief Frees the memory allocated to one description.
 *
 * Does NOT free the pointer itself.
 *
 * @param ptr Pointer to the soon to be freed area.
 */
void free_description(struct description *ptr);

/**
 * @brief Frees the memory allocated to one example set.
 *
 * Does NOT free the pointer itself.
 *
 * @param ptr Pointer to the soon to be freed area.
 */
void free_example_set(struct example_set *ptr);

/**
 * @brief Frees the id3 tree.
 *
 * Must be called with the root of the tree.
 *
 * @param ptr Pointer to root.
 */
void free_classifier(struct classifier *ptr);

/**
 * @brief Returns an error and sets the errno accordingly.
 *
 * @param err Error code
 * @return Error code.
 */
int set_error(int err);

/**
 * @brief Frees a pointer and returns a NULL.
 *
 * Used for temporary variables in loops.
 *
 * @param ptr Pointer to be freed.
 * @return NULL
 */
void *free_and_set_NULL(void *ptr);

/**
 * @brief Returns the index of minimum value from a vector of doubles.
 *
 * @param v the vector
 * @param size size of vector
 * @return index of minimum value
 */
int get_double_min(double *v, int size);

#endif

