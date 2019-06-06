/*
 * SSH Attack (C implementation)
 * Author: Priyank Chheda 
 *         E: p.chheda29@gmail.com
 *         W: https://github.com/x899
 *
 * Two Password Cracking Attacks are implemented in this project
 * a. Dictionary Attack
 * b. Brute Force Attack
 *
 */


#ifndef SSHBRUTE_H
#define SSHBRUTE_H

// characters that will be used in brute force password generation
// you can also append uppercase letter and numeric value
static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
static const int alphabetSize = sizeof(alphabet) - 1;


/* Function Declaration */
void ssh_brute(char*, int, char* a[], int);
void ssh_dictionary(char*, int, char* a[], int, char* b[], int);

#endif
