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

/**
 * Prints the usage information.
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

static void learning_instance(int argc, char **argv)
{
	if (argc < 5 || argc > 7)
		usage();

	exit(EXIT_SUCCESS);
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

