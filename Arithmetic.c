/**************************************************************************************************************************************
*	File: Arithmetic.c
*	Name: Andrew Craig
*	CruzID: acraig1
*	Assignment: pa3
***************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "BigInteger.h"


int main(int argc, char* argv[])
{
	//Check for appropriate number of arguments. 
	if(argc != 3)
	{
		fprintf(stderr, "Sparse Error: incorrect number of arguments\n");
		exit(EXIT_FAILURE);
	}

	//Open up input and output files.
	FILE *in = fopen(argv[1], "r");
	FILE *out = fopen(argv[2], "w");

	//Check to see if the files were successfully opened.
	if(in == NULL)
	{
		fprintf(stderr, "Unable to open file %s for reading\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	if(out == NULL)
	{
		fprintf(stderr, "Unable to open file %s for writing\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	int i;
	int a;
	int b;
	char* s1;
	char* s2;
	BigInteger A;
	BigInteger B;
	BigInteger C;
	BigInteger D;
	BigInteger E;
	BigInteger F;
	BigInteger G;
	BigInteger H;
	BigInteger I;
	BigInteger J;
	BigInteger K;
	BigInteger L;
	BigInteger M;
	BigInteger N;
	BigInteger O;
	BigInteger P;

	fscanf(in, "%d", &a);
	s1 = calloc(a + 1, sizeof(char));
	fscanf(in, "%s", s1);
	A = stringToBigInteger(s1);

	fscanf(in, "%d", &b);
	s2 = calloc(b + 1, sizeof(char));
	fscanf(in, "%s", s2);
	B = stringToBigInteger(s2);

	//A
	printBigInteger(out, A);
	fprintf(out, "\n\n");

	//B
	printBigInteger(out, B);
	fprintf(out, "\n\n");

	//A + B
	C = sum(A, B);
	printBigInteger(out, C);
	fprintf(out, "\n\n");

	//A - B										
	D = diff(A, B);
	printBigInteger(out, D);
	fprintf(out, "\n\n");

	//A - A
	E = diff(A, A);
	printBigInteger(out, E);
	fprintf(out, "\n\n");

	//3A - 2B									
	F = newBigInteger();
	for(i = 0; i < 3; i++)
	{
		add(F, F, A);
	}

	G = newBigInteger();
	for(i = 0; i < 2; i++)
	{
		add(G, G, B);
	}

	H = diff(F, G);
	printBigInteger(out, H);
	fprintf(out, "\n\n");

	//AB
	I = prod(A, B);
	printBigInteger(out, I);
	fprintf(out, "\n\n");

	//A^2										
	J = prod(A, A);								
	printBigInteger(out, J);
	fprintf(out, "\n\n");

	//B^2										
	K = prod(B, B);
	printBigInteger(out, K);
	fprintf(out, "\n\n");

	//9A^4 + 16B^5				
	L = copy(A);
	for(i = 0; i < 3; i++)
	{
		multiply(L, L, A);
	}

	M = newBigInteger();
	for(i = 0; i < 9; i++)
	{
		add(M, M, L);
	}

	N = copy(B);
	for(i = 0; i < 4; i++)
	{
		multiply(N, N, B);
	}

	O = newBigInteger();
	for(i = 0; i < 16; i++)
	{
		add(O, O, N);
	}

	P = sum(M, O);
	printBigInteger(out, P);
	fprintf(out, "\n\n");	

	free(s1);
	free(s2);

	freeBigInteger(&A);
	freeBigInteger(&B);
	freeBigInteger(&C);
	freeBigInteger(&D);
	freeBigInteger(&E);
	freeBigInteger(&F);
	freeBigInteger(&G);
	freeBigInteger(&H);
	freeBigInteger(&I);
	freeBigInteger(&J);
	freeBigInteger(&K);
	freeBigInteger(&L);
	freeBigInteger(&M);
	freeBigInteger(&N);
	freeBigInteger(&O);
	freeBigInteger(&P);

	//Close input and output files.
	fclose(in);
	fclose(out);

	return(0);
}