#include "tests.h"
#include "string.h"
#define K_1 3
#define K_2 2
#define K_3 -1
#define K_4 -3
#define K_5 29

// See full documentation in header file
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  char out[] = "def";
  encode (in, K_1);
  return strcmp (in, out) != 0; //0 if success, 1 if failure
}

// See full documentation in header file
int test_encode_cyclic_lower_case_special_char_positive_k ()
{
  char in[] = "x y,z. a@b&c]";
  char out[] = "z a,b. c@d&e]";
  encode (in, K_2);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
  char in[] = " a$c.,eg ";
  char out[] = " z$b.,df ";
  encode (in, K_3);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_negative_k ()
{
  char in[] = "xyzabc";
  char out[] = "uvwxyz";
  encode (in, K_4);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_upper_case_positive_k ()
{
  char in[] = "ABCDEF";
  char out[] = "DEFGHI";
  encode (in, K_5);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "def";
  char out[] = "abc";
  decode (in, K_1);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_special_char_positive_k ()
{
  // your code goes here
  char in[] = "z a,b. c@d&e]";
  char out[] = "x y,z. a@b&c]";
  decode (in, K_2);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
  // your code goes here
  char in[] = " z$b.,df ";
  char out[] = " a$c.,eg ";
  decode (in, K_3);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_negative_k ()
{
  // your code goes here
  char in[] = "uvwxyz";
  char out[] = "xyzabc";
  decode (in, K_4);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_upper_case_positive_k ()
{
  // your code goes here
  char in[] = "DEFGHI";
  char out[] = "ABCDEF";
  decode (in, K_5);
  return strcmp (in, out) != 0;
}
