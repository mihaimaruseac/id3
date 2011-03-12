/*!
 * @file
 * @author Mihai Maruseac (mihai@rosedu.org)
 *
 * @section DESCRIPTION
 * IAUT1: ID3
 * Main file
 *
 * @section QUOTE
 * TODO
 *
 * @section LICENCE
 * This code is distributed as is, under WTF Public License.
 */

#include <stdio.h>
#include <stdlib.h>

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
			"\t\t\tHandling numeric attributes:\n"
			"\t\t\t\t-ndiv - divide in two only once\n"
			"\t\t\t\t-nfull - full handling\n"
			"\t\t\tHandling missing attributes:\n"
			"\t\t\t\t-mmaj - replace with the most frequent\n"
			"\t\t\t\t-mprb - use probability theory\n"
			"\t\t\t\t-mid3 - use ID3\n"
			"\n"
			"\tGraph output options and arguments:\n"
			"\t\tFILES = ID3FILE [OUTFILE(output)]\n"
			"\t\t\tOUTFILE can be - (or missing) for stdout\n"
			"\t\tOPTIONS:\n"
			"\t\t\t-gdot - display using Dot\n"
			"\t\t\t-gtree - display tree-like\n"
			"\n"
			"\tClassification options and arguments:\n"
			"\t\tFILES = ATTRFILE TESTFILE [OUTFILE(output)]\n"
			"\t\t\tOUTFILE can be - (or missing) for stdout\n"
		);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	usage();
	exit(EXIT_SUCCESS);
}

