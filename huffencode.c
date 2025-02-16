/**************************************************************
*
* Daniel Morales-Garcia
*
***************************************************************/

#include <stdio.h>
#include "huffman.h"

/* opens file from first command line argument, and creates file */
/* with encoded data from second command line argument           */
int main(int argc, char** argv)
{
  char* infile;
  char* outfile;
  FILE* in;
  FILE* out;

  /* error case for incorrect number of arguments */
  if(argc != 3) 
  {
    printf("wrong number of args\n");
    return 1;
  }

  infile = argv[1];
  outfile = argv[2];

  /* opens file to read data */
  in = fopen(infile, "rb");
  if(in == NULL)
  {
    printf("couldn't open %s for reading\n", infile);
    return 2;
  }

  /* opens file to write encoded data to */
  out = fopen(outfile, "wb");
  if(out == NULL)
  {
    printf("couldn't open %s for writing\n", outfile);
    return 3;
  }
  
  /* encodes data from in file and prints data to out file */
  encodeFile(in, out);

  /* closes files */
  fclose(in);
  fclose(out);

  return 0;
}
