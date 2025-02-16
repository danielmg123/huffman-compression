/**************************************************************
*
* Daniel Morales-Garcia
*
***************************************************************/

#include <stdio.h>
#include "huffman.h"

/* opens encoded data file from first command line argument, and     */
/* decodes and prints data to file from second command line argument */
int main(int argc, char** argv)
{
  char* infile;
  char* outfile;
  FILE* in;
  FILE* out;

  /* error case for argument count */
  if(argc != 3) 
  {
    printf("wrong number of args\n");
    return 1;
  }

  /* assigns arguments to strings */
  infile = argv[1];
  outfile = argv[2];

  /* opens file if file exists */
  in = fopen(infile, "rb");
  if(in == NULL)
  {
    printf("couldn't open %s for reading\n", infile);
    return 2;
  }

  /* opens file for writing */
  out = fopen(outfile, "wb");
  if(out == NULL)
  {
    printf("couldn't open %s for writing\n", outfile);
    return 3;
  }

  /* decodes in file and prints data to out file */
  decodeFile(in, out);

  /* closes files */
  fclose(in);
  fclose(out);

  return 0;
}
