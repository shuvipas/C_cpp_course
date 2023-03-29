#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define ENCODE "encode"
#define DECODE "decode"
#define TEST "test"
#define TEST_ARG 2
#define COMAND_ARG 5
#define BASE 10
#define MOD 26
#define BUFFER_SIZE 1024

/**
 * Checks if a given character array represents an integer.
 * @param k -The input character array to be checked.
 * @return 'EXIT_SUCCESS' if the input character represents a valid integer 
 * and EXIT_FAILURE otherwise.
*/
int is_int(char k[]);

/**
 * A command-line interface (CLI) tester function for are cipher program.
 * It takes command-line arguments as input and checks if the input
 *  is valid or not.
 *  The function returns 'EXIT_SUCCESS' if the input is valid,
 *  and 'EXIT_FAILURE' otherwise.
 * @param arg_c The number of command-line arguments.
 * @param arg_v An array of pointers to the command-line arguments.
 * @return 'EXIT_SUCCESS' if the input is valid, and 'EXIT_FAILURE' otherwise.
*/
int cli_tester (int arg_c, char *arg_v[]);

/**
 * This funct is used to test the cipher program by calling are encoder and
 *  decoder with various kind of strings and shift parameters.
 *  @return  'EXIT_SUCCESS' if all the tests pass, 
 * and 'EXIT_FAILURE' otherwise.
*/
int cipher_tester (void);

/**
 * Encodes or decodes a given input file and writes the result to
 *  an output file.
 * @param k the integer key value used for encoding or decoding.
 * @param command a string indicating whether to encode or decode.
 * @param in_path a string containing the file path for the input file.
 * @param out_path a string containing the file path for the output file.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
*/
int cipher_command (int k, char *command, char *in_path, char *out_path);


int main (int argc, char *argv[])
{
  int tester = cli_tester (argc, argv);
  if(tester)
  {
    return EXIT_FAILURE;
  }
  if(!strcmp(argv[1],TEST))
  {
    if(cipher_tester())
    {
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  long l_k = strtol(argv[2], NULL, BASE);  
  int k = l_k % MOD;
  int cipher_success = cipher_command(k,argv[1], argv[3],argv[4]); 
  if(cipher_success)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int cipher_command (int k, char *command, char *in_path, char *out_path)
{
  FILE *in_file = fopen(in_path, "r");
  if(in_file == NULL)
  {
    fprintf(stderr,"The given file is invalid.\n");
    return EXIT_FAILURE;
  }
  FILE *out_file = fopen(out_path, "w");
  if(out_file == NULL)
  {
    fclose(in_file);
    fprintf(stderr,"The given file is invalid.\n");
    return EXIT_FAILURE;
  }
  char input[BUFFER_SIZE];
  if(!strcmp(command,ENCODE))
  {
    while(fgets( input, BUFFER_SIZE, in_file)!= NULL)
    {
      encode(input, k);
      fprintf(out_file, "%s", input);
    }
  }
  else if(!strcmp(command,DECODE))
  {
    while(fgets( input, BUFFER_SIZE, in_file)!= NULL)
    {
      decode(input, k);
      fprintf(out_file, "%s", input);
    }
  }
  fclose(out_file);
  fclose(in_file);
  return EXIT_SUCCESS;
}

int cipher_tester (void)
{
  int test = 0;
  test += test_encode_non_cyclic_lower_case_positive_k ();
  test += test_encode_cyclic_lower_case_special_char_positive_k ();
  test += test_encode_non_cyclic_lower_case_special_char_negative_k ();
  test += test_encode_cyclic_lower_case_negative_k ();
  test += test_encode_cyclic_upper_case_positive_k ();
  test += test_decode_non_cyclic_lower_case_positive_k ();
  test += test_decode_cyclic_lower_case_special_char_positive_k ();
  test += test_decode_non_cyclic_lower_case_special_char_negative_k ();
  test += test_decode_cyclic_lower_case_negative_k ();
  test += test_decode_cyclic_upper_case_positive_k (); 
  if(test != 0)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


int cli_tester (int arg_c, char *arg_v[])
{
  if((arg_c != TEST_ARG)&&(arg_c != COMAND_ARG))
  {
    fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
    return EXIT_FAILURE;
  }
  else if (arg_c == TEST_ARG)
  {
    if(strcmp(arg_v[1],TEST))
    {
      fprintf(stderr, "Usage: cipher test\n");
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  else
  {
    if(strcmp(arg_v[1],DECODE)&&strcmp(arg_v[1],ENCODE))
    {
      fprintf(stderr,"The given command is invalid.\n");
      return EXIT_FAILURE;
    }
    else if (is_int(arg_v[2]))
    {
      fprintf(stderr,"The given shift value is invalid.\n");
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
}

int is_int(char k[])
{
  int i= 0;
  while(k[i] != '\0')
  {
    if((k[i] == '.')|| !isdigit(k[i]))
    {
      if(!((i==0)&&k[i]=='-'))
      {
        return EXIT_FAILURE;
      }
    }
    i++;
  }
  return EXIT_SUCCESS;
}