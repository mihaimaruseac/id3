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
#define MISS_ID3 3 /**< @brief use ID3 to detect values for missing values */

#define TREE_ASCII 1 /**< @brief output ID3 tree in ASCII */
#define TREE_DOT 2 /**< @brief output ID3 tree using Dot */
#define TREE_SCHEME 3 /**< @brief output ID3 tree as Scheme/Lisp functions */
#define TREE_IFTHEN 4 /**< @brief output ID3 tree as if(...) code */

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
 * sort in ascending order the example data by this attribute. If at a later
 * time the example is splitted, the index will remain the same but some of
 * the values will not be relevant for that example. However, code doing this
 * will take care of that, thus, it is not a concern right now.
 *
 * Otherwise, the ptr will point to a vector of names, used for the discrete
 * values. Each name can be obtained by converting the integer to a char* and
 * looking at the resulting memory location.
 */
struct attribute {
	/** Name of the attribute */
	char *name;
	/** Type of attribute */
	enum attr_type type;
	/** Type-dependent pointer (see description) */
	int *ptr;
	/** Length of vector pointed to by ptr */
	int C;
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
 * @brief Reads a set to use when learning (learning = 1) or classifying
 * (learning = 0).
 *
 * If learning is 0 then there will be no missing values and no class
 * informations in the file. However, the structures would be too similar if
 * separate data types would be used for the two cases.
 *
 * @param file Description file
 * @param learning Flag describing what to expect
 * @param descr The description of the entire model.
 * @return The required set.
 */
static struct example_set *read_set(FILE *file, int learning,
		const struct description *descr);

/**
 * @brief Reads one attribute from the description file.
 *
 * Used in a loop in the main reading function.
 *
 * @param file File containing the description.
 * @return The read attribute.
 */
static struct attribute *read_attribute(FILE *file);

/**
 * @brief Reads one example from a file.
 *
 * Used in a loop to read the entire example set.
 *
 * @param file File containing the example
 * @param learning Flag describing what to expect (see read_set)
 * @param descr The description of the entire model.
 * @param set The example set from which this example is part of.
 * @return The read example.
 */
static struct example *read_example(FILE *file, int learning,
		const struct description *descr,
		struct example_set *set);

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
 * @brief Writes an attribute to a file.
 *
 * @param attr Attribute to be written.
 * @param file File to write to.
 */
static void write_attribute(const struct attribute *attr, FILE *file);

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
 * @brief Writes an example to a file.
 *
 * @param ex Example to be written.
 * @param descr Description to use while writing.
 * @param file File to write to.
 */
static void write_example(const struct example *ex,
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
 * @brief Tests if an example contains a missing value on the current
 * position.
 *
 * @param ex Example to test
 * @param index Where to look
 * @return 1 if missing, 0 otherwise.
 */
static int missing_value(const struct example *ex, int index);

/**
 * @brief Records a new missing value from the learning set.
 *
 * @param index Index of attribute
 * @param set Example set in which to record the missing value
 * @return Change in flag to record missing value or 0 on error
 */
static int record_missing(int index, struct example_set *set);

/**
 * @brief Gets the mapping between the read attribute value and the indexes
 * used in the example_set and example structures.
 *
 * @param string The read string
 * @param attr The corresponding attribute
 * @return The index from the mapping
 */
static int get_index_from_descr(const char *string,
		const struct attribute *attr);

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
void clear_filter_info(struct example_set *lset);

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
 * @brief Frees one attribute from a description.
 *
 * Does NOT free the pointer itself.
 *
 * @param ptr Pointer to the attribute
 */
static void free_attribute(struct attribute *ptr);

/**
 * @brief Frees one example from an example set.
 *
 * Does NOT free the pointer itselt.
 *
 * @param ptr Pointer to the example.
 */
static void free_example(struct example *ptr);

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

#endif

