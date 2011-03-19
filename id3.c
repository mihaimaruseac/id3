/*!
 * @file id3.c
 * @brief Main file
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3: Main file
 *
 * Contains the definition of the entry point and treats the
 * command line arguments and options.
 *
 * @section QUOTE
 * The question of whether a computer can think is no more interesting than
 * the question of whether a submarine can swim. (Dijkstra)
 *
 * @section LICENCE
 * This code is distributed as is, under WTF Public License.
 *
 * @bug No known bugs.
 * @todo Implement output.
 * @todo Implement testing.
 */

#include "globals.h"
#include "id3graph.h"
#include "id3learn.h"
#include "id3test.h"

/**
 * @brief Macro used to set a default value to a flag argument if it is still
 * UNKNOWN_VALUE.
 *
 * @param flag Flag to test and set
 * @param val Default value to use
 */
#define SET_DEFAULT(flag, val)\
	do {\
		if (flag == UNKNOWN_VALUE)\
			flag = val;\
	} while (0)

/**
 * @brief Macro used to test if a specified command line flag defines a value
 * or is eroneous.
 *
 * @return 1 if flag is ok, 0 otherwise
 */
#define SETS(var, val)\
	(strncmp(argv[i], val, strlen(val)) == 0 && var == UNKNOWN_VALUE)

/**
 * @brief Prints the usage information.
 */
static void usage()
{
	fprintf(stderr, "./id3 CMD [OPTIONS] FILES\n"
			"\tCMD can be one of:\n"
			"\t\tl - learn a classification,\n"
			"\t\tg - output classification graph,\n"
			"\t\tc - classify a set of examples.\n"
			"\n"
			"\tLearning options and arguments:\n"
			"\t\tFILES = ATTRFILE LEARNFILE ID3FILE(output)\n"
			"\t\tOPTIONS:\n"
			"\t\t\tHandling numeric attributes (one of):\n"
			"\t\t\t\t-ndiv - divide in two only once (default)\n"
			"\t\t\t\t-nfull - full handling\n"
			"\t\t\tHandling missing attributes (one of):\n"
			"\t\t\t\t-mmaj - replace with most frequent (default)\n"
			"\t\t\t\t-mprb - use probability theory\n"
			"\n"
			"\tGraph output options and arguments:\n"
			"\t\tFILES = ID3FILE [OUTFILE(output)]\n"
			"\t\t\tOUTFILE can be - (or missing) for stdout\n"
			"\t\tOPTIONS:\n"
			"\t\t\t-gascii - display in ASCII art (default)\n"
			"\t\t\t-gdot - display using Dot\n"
			"\t\t\t-gscheme - display Scheme-like (Lisp-like)\n"
			"\n"
			"\tClassification options and arguments:\n"
			"\t\tFILES = ATTRFILE TESTFILE [OUTFILE(output)]\n"
			"\t\t\tOUTFILE can be - (or missing) for stdout\n"
		);
	exit(EXIT_FAILURE);
}

/**
 * @brief Parses the cmd line for the case when we wish to learn a new classifier.
 */
static void learning_instance(int argc, char **argv)
{
	int num_handle, missing_handle;
	char *attr_file, *learn_file, *id3_file;
	FILE *attr, *learn, *id3;
	int i, status;

	if (argc < 5 || argc > 7)
		usage();

	num_handle = UNKNOWN_VALUE;
	missing_handle = UNKNOWN_VALUE;
	attr_file = NULL;
	learn_file = NULL;
	id3_file = NULL;

	for (i = 2; i < argc; i++)
		if (argv[i][0] == '-')/* option */
			if (SETS(num_handle, "-ndiv"))
				num_handle = NUM_DIV;
			else if (SETS(num_handle, "-nfull"))
				num_handle = NUM_FULL;
			else if (SETS(missing_handle, "-mmaj"))
				missing_handle = MISS_MAJ;
			else if (SETS(missing_handle, "-mprb"))
				missing_handle = MISS_PRB;
			else
				goto fail;
		else if (attr_file == NULL)
			attr_file = strdup(argv[i]);
		else if (learn_file == NULL)
			learn_file = strdup(argv[i]);
		else if (id3_file == NULL)
			id3_file = strdup(argv[i]);
		else
			goto fail;

	CHECK(id3_file != NULL, fail);
	CHECK(learn_file != NULL, fail);

	SET_DEFAULT(num_handle, NUM_DIV);
	SET_DEFAULT(missing_handle, MISS_MAJ);

	attr = fopen(attr_file, "r");
	if (attr == NULL) {
		perror("Cannot open attribute file");
		goto fail;
	}
	free(attr_file);

	learn = fopen(learn_file, "r");
	if (learn == NULL) {
		perror("Cannot open learn file");
		fclose(attr);
		goto fail;
	}
	free(learn_file);

	id3 = fopen(id3_file, "w");
	if (id3 == NULL) {
		perror("Cannot open output file");
		fclose(learn);
		fclose(attr);
		goto fail;
	}
	free(id3_file);

	status = id3_learn_bootstrap_file(num_handle, missing_handle, attr,
			learn, id3);
	if (status)
		perror("Error while learning");

	fclose(attr);
	fclose(learn);
	fclose(id3);

	exit(status);
fail:
	free_and_set_NULL(attr_file);
	free_and_set_NULL(learn_file);
	free_and_set_NULL(id3_file);
	usage();
}

/**
 * @brief Parses the cmd line for the case when we wish to represent an id3
 * tree corresponding to a classifier.
 */
static void graphing_instance(int argc, char **argv)
{
	char *id3_file, *out_file;
	FILE *id3, *out;
	int i, graph_mode, status;

	if (argc < 3 || argc > 5)
		usage();

	id3_file = NULL;
	out_file = NULL;
	graph_mode = UNKNOWN_VALUE;

	for (i = 2; i < argc; i++)
		if (argv[i][0] == '-') /* option or stdout */
			if (strncmp(argv[i], "-", strlen(argv[i])) == 0 &&
					out_file == NULL)
				if (id3_file != NULL)
					out_file = strdup(argv[i]);
				else
					goto fail;
			else if (SETS(graph_mode, "-gascii"))
				graph_mode = TREE_ASCII;
			else if (SETS(graph_mode, "-gdot"))
				graph_mode = TREE_DOT;
			else if (SETS(graph_mode, "-gscheme"))
				graph_mode = TREE_SCHEME;
			else
				goto fail;
		else if (id3_file == NULL)
			id3_file = strdup(argv[i]);
		else if (out_file == NULL)
			out_file = strdup(argv[i]);
		else
			goto fail;

	CHECK(id3_file != NULL, fail);
	SET_DEFAULT(graph_mode, TREE_ASCII);

	id3 = fopen(id3_file, "r");
	if (id3 == NULL) {
		perror("Cannot open classifier file");
		goto fail;
	}
	free(id3_file);

	if (out_file == NULL || strncmp(out_file, "-", 1) == 0)
		out = stdout;
	else {
		out = fopen(out_file, "w");
		if (out == NULL) {
			perror("Cannot open output file");
			fclose(id3);
			goto fail;
		}
	}

	status = id3_output_graph(id3, out, graph_mode);
	if (status)
		perror("Error while outputing graph");

	fclose(id3);
	if (out_file && strncmp(out_file, "-", 1) != 0)
		fclose(out);
	free(out_file);

	exit(status);
fail:
	free_and_set_NULL(out_file);
	free_and_set_NULL(id3_file);
	usage();
}

/**
 * @brief Parses the cmd line for the case when we wish to represent an id3
 * tree corresponding to a classifier.
 */
static void classifying_instance(int argc, char **argv)
{
	char *id3_file, *test_file, *out_file;
	FILE *id3, *out, *test;
	int i, status;

	if (argc < 4 || argc > 5)
		usage();

	id3_file = NULL;
	test_file = NULL;
	out_file = NULL;

	for (i = 2; i < argc; i++)
		if (id3_file == NULL)
			id3_file = strdup(argv[i]);
		else if (test_file == NULL)
			test_file = strdup(argv[i]);
		else if (out_file == NULL)
			out_file = strdup(argv[i]);
		else
			goto fail;

	id3 = fopen(id3_file, "r");
	if (id3 == NULL) {
		perror("Cannot open classifier file");
		goto fail;
	}
	free(id3_file);

	test = fopen(test_file, "r");
	if (test == NULL) {
		perror("Cannot open test file");
		fclose(id3);
		goto fail;
	}
	free(test_file);

	if (out_file == NULL || strncmp(out_file, "-", 1) == 0)
		out = stdout;
	else {
		out = fopen(out_file, "w");
		if (out == NULL) {
			perror("Cannot open output file");
			fclose(test);
			fclose(id3);
			goto fail;
		}
	}

	status = id3_test(id3, test, out);
	if (status)
		perror("Error while testing");

	fclose(id3);
	fclose(test);
	if (out_file && strncmp(out_file, "-", 1) != 0)
		fclose(out);
	free(out_file);

	exit(status);
fail:
	free_and_set_NULL(out_file);
	free_and_set_NULL(test_file);
	free_and_set_NULL(id3_file);
	usage();
}

int main(int argc, char **argv)
{
	CHECK(argc >= 2, exit);

	if (strncmp(argv[1], "l", 1) == 0)
		learning_instance(argc, argv);

	if (strncmp(argv[1], "g", 1) == 0)
		graphing_instance(argc, argv);

	if (strncmp(argv[1], "c", 1) == 0)
		classifying_instance(argc, argv);

exit:
	usage();
	exit(EXIT_FAILURE);
}

