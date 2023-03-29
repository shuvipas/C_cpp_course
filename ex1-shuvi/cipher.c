#include "cipher.h"
#include <ctype.h>
#include <stdio.h>
/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

#define MOD 26 //number of charecters in the alphabet
#define ALOW 'a' //first low char
#define AUP 'A' //first upper char

/**
* calculates the caesar cipher for a given character and key.
* @param c- The character to encrypt/decrypt.
* @param k- The key to use for encryption/decryption.
* @return The encrypted character.
*/
char caesar_cipher_calculator(char c, int k);

// See full documentation in header file
void encode (char s[], int k)
{
  int i = 0;
  while (s[i]!= '\0')
  {
    s[i] = caesar_cipher_calculator(s[i], k);
    i++;
  }
}

// See full documentation in header file
void decode (char s[], int k)
{
  k = -k;
  int i = 0;
  while (s[i]!= '\0')
  {
    s[i] = caesar_cipher_calculator(s[i], k);
    i++;
  }
}


char caesar_cipher_calculator(char c, int k)
{
  int new_k =k;
  if (isalpha((int) c))
      {
        if (islower((int)c))
        {
            new_k = ((c-ALOW + k)%MOD + MOD)%MOD;
            c = ALOW + new_k;
        }
        else
        {
          new_k = ((c-AUP + k)%MOD + MOD)%MOD;
          c = AUP + new_k; 
        }
       }
  return c;
}