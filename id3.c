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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"

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
			"\t\t\t\t-mid3 - use ID3\n"
			"\n"
			"\tGraph output options and arguments:\n"
			"\t\tFILES = ID3FILE [OUTFILE(output)]\n"
			"\t\t\tOUTFILE can be - (or missing) for stdout\n"
			"\t\tOPTIONS:\n"
			"\t\t\t-gtree - display tree-like (default)\n"
			"\t\t\t-gdot - display using Dot\n"
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
	int i;

	if (argc < 5 || argc > 7)
		usage();

	num_handle = UNKNOWN_VALUE;
	missing_handle = UNKNOWN_VALUE;
	attr_file = UNKNOWN_VALUE;
	learn_file = UNKNOWN_VALUE;
	id3_file = UNKNOWN_VALUE;

	for (i = 2; i < argc; i++)
		if (argv[i][0] == '-')/* option */
			if (strncmp(argv[i], "-ndiv", 5) == 0 && num_handle ==
					UNKNOWN_VALUE)
				num_handle = NUM_DIV;
			else if (strncmp(argv[i], "-nfull", 6) == 0 &&
					num_handle == UNKNOWN_VALUE)
				num_handle = NUM_FULL;
			else if (strncmp(argv[i], "-mmaj", 5) == 0 &&
					missing_handle == UNKNOWN_VALUE)
				missing_handle = MISS_MAJ;
			else if (strncmp(argv[i], "-mprb", 5) == 0 &&
					missing_handle == UNKNOWN_VALUE)
				missing_handle = MISS_PRB;
			else if (strncmp(argv[i], "-mid3", 5) == 0 &&
					missing_handle == UNKNOWN_VALUE)
				missing_handle = MISS_ID3;
			else
				goto fail;
		else if (attr_file == UNKNOWN_VALUE)
			attr_file = strdup(argv[i]);
		else if (learn_file == UNKNOWN_VALUE)
			learn_file = strdup(argv[i]);
		else if (id3_file == UNKNOWN_VALUE)
			id3_file = strdup(argv[i]);
		else
			goto fail;

	if (id3_file == UNKNOWN_VALUE)
		goto fail;
	if (learn_file == UNKNOWN_VALUE)
		goto fail;

	if (num_handle == UNKNOWN_VALUE)
		num_handle = NUM_DIV;
	if (missing_handle == UNKNOWN_VALUE)
		missing_handle = MISS_MAJ;

	free(attr_file);
	free(learn_file);
	free(id3_file);
	exit(EXIT_SUCCESS);
fail:
	if (attr_file != UNKNOWN_VALUE)
		free(attr_file);
	if (learn_file != UNKNOWN_VALUE)
		free(learn_file);
	if (id3_file != UNKNOWN_VALUE)
		free(id3_file);
	usage();
}

static void graphing_instance(int argc, char **argv)
{
	exit(EXIT_SUCCESS);
}

static void classifying_instance(int argc, char **argv)
{
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	if (argc < 2)
		usage();

	if (strncmp(argv[1], "l", 1) == 0)
		learning_instance(argc, argv);

	if (strncmp(argv[1], "g", 1) == 0)
		graphing_instance(argc, argv);

	if (strncmp(argv[1], "c", 1) == 0)
		classifying_instance(argc, argv);

	usage();
	exit(EXIT_FAILURE);
}

