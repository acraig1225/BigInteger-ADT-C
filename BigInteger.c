/**************************************************************************************************************************************
*	File: BigInteger.c
*	Name: Andrew Craig
*	CruzID: acraig1
*	Assignment: pa3
***************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "BigInteger.h"
#define POWER 9
#define BASE power(10, POWER)


typedef struct BigIntegerObj
{
	int sign;
	List magnitude;
} BigIntegerObj;


//Description: Power function to not have to externally include math.h
long power(int base, int pow)
{
	long result = 1;
	int i;
	for(i = 0; i < pow; i++)
	{
		result *= base;
	}

	return result;
}


//Description: Helper function that can normalize a List L to the current BASE. Can also return the proper sign for the given list. (This functionality is mostly used for addition and subtraction.)
int normalize(List L, int sign)
{
	long carry = 0;
	long borrow = 0;
	long temp;

	if(!length(L))
	{
		return 0;
	}

	while(length(L) > 1 && back(L) == 0)
	{
		deleteBack(L);
	}

	moveFront(L);
	while(index(L) != -1)
	{
		if(carry != 0)
		{
			set(L, get(L) + carry);
			carry = 0;
		}
		else if(borrow != 0)
		{
			set(L, get(L) - borrow);
			borrow = 0;
		}

		if(get(L) > BASE - 1)
		{
			temp = get(L) % BASE;
			carry = get(L) / BASE;
			set(L, temp);
		}
		else if(get(L) < 0)
		{
			while(get(L) < 0)
			{
				set(L, get(L) + BASE);
				borrow++;
			}
		}
		moveNext(L);
	}

	if(carry != 0)
	{
		if(carry > BASE - 1)
		{
			append(L, carry);
			printList(stdout, L);
			normalize(L, 1);
		}
		else
		{
			append(L, carry);
		}
	}

	if(borrow != 0)
	{
		sign = -1;
		moveBack(L);
		set(L, get(L) - BASE * borrow);
		moveFront(L);
		while(index(L) != -1)
		{
			set(L, get(L) * -1);
			moveNext(L);
		}
		normalize(L, 1);
	}
	return sign;
}

//Constructors-Destructors*************************************************************************************************************
//Description: Returns a reference to a new BigInteger object in the zero state.
BigInteger newBigInteger()
{
	BigInteger N = malloc(sizeof(BigIntegerObj));
	N->sign = 0;
	N->magnitude = newList();
	return (N);
}

//Description: Frees heap memory associated with *pN, sets *pN to NULL.									
void freeBigInteger(BigInteger* pN)
{
	if(*pN != NULL && pN != NULL)
	{
		freeList(&(*pN)->magnitude);
		free(*pN);
		*pN = NULL;
	}
}

//Access Functions*********************************************************************************************************************
//Description: Returns -1 if N is negative, 1 if N is positive, and 0 if N is in the zero state
int sign(BigInteger N)
{
	if(N == NULL)
	{
		printf("BigInteger Error: calling sign() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}
	return (N->sign);
}										

//Description: Returns -1 if A<B, 1 if A>B, and 0 if A=B.															
int compare(BigInteger A, BigInteger B)
{
	if(A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling compare() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}
	
	//Check the signs
	//If signs are not equal
	//	Case 1: A = +, B = -
	//	Case 2: A = +, B = 0
	//	Case 3: A = 0, B = +
	//	Case 4: A = 0, B = -
	//	Case 5: A = -, B = +
	//	Case 6: A = -, B = 0
	if(sign(A) != sign(B))
	{
		if(sign(B) == 1)
		{
			return -1;
		}
		else if(sign(A) == 1)
		{
			return 1;
		}
		else if(sign(A) == 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	//Check the lengths
	//If lengths are not equal
	//	Case 1: A > B, A & B are positive
	//	Case 2: A > B, A & B are negative
	//	Case 3: A < B, A & B are positive
	//	Case 4: A < B, A & B are negative
	if(length(A->magnitude) != length(B->magnitude))
	{
		if(length(A->magnitude) < length(B->magnitude))
		{
			if(sign(A) == -1)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			if(sign(A) == -1)
			{
				return -1;
			}
			else
			{
				return 1;
			}			
		}
	}

	//Length and sign are equal so compare each digit from the front until a difference is found.
	moveBack(A->magnitude);
	moveBack(B->magnitude);
	while(index(A->magnitude) != -1)
	{
		if(get(A->magnitude) != get(B->magnitude))
		{
			if(get(A->magnitude) < get(B->magnitude))
			{
				return -1;
			}
			else
			{
				return 1;
			}
		}
		movePrev(A->magnitude);
		movePrev(B->magnitude);
	}

	//A & B are equal
	return 0;
}

//Description: Return true (1) if A and B are equal, false (0) otherwise.					
int equals(BigInteger A, BigInteger B)
{
	if(A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling equals() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	
	if(length(A->magnitude) != length(B->magnitude))
	{
		return 0;
	}
	else if(sign(A) != sign(B))
	{
		return 0;
	}
	else if(back(A->magnitude) == 0 && back(B->magnitude) == 0)
	{
		return 1;
	}
	else
	{
		moveBack(A->magnitude);
		moveBack(B->magnitude);
		while(index(A->magnitude) != -1)
		{
			if(get(A->magnitude) != get(B->magnitude))
			{
				return 0;
			}
			movePrev(A->magnitude);
			movePrev(B->magnitude);
		}
	}
	return 1;
}				

//Manipulation Procedures**************************************************************************************************************
//Description: Re-sets N to the zero state.
void makeZero(BigInteger N)
{
	if(N == NULL)
	{
		printf("BigInteger Error: calling makeZero() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	clear(N->magnitude);
	append(N->magnitude, 0);
	N->sign = 0;
}

//Description: Reverses the sign of N: positive <--> negative. Does nothing if N is in the zero state.								
void negate(BigInteger N)
{
	if(N == NULL)
	{
		printf("BigInteger Error: calling negate() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	N->sign = N->sign * -1;
}			
															
//BigInteger Arithmetic Operations*****************************************************************************************************
//Description: Returns a reference to a new BigInteger object representing the decimal integer represented in base 10 by the string s.
//Precondition: s is a non-empty string containing only base ten digits {0,1,2,3,4,5,6,7,8,9} and an optional sign {+, -} prefix.
BigInteger stringToBigInteger(char* s)
{
	int i = 0;
	if(strlen(s) == 0)
	{
		printf("BigInteger Error: calling stringToBigInteger() on an empty string\n");
		exit(EXIT_FAILURE);
	}
	if(s[0] == '-' || s[0] == '+')									//ignore first element in the string if it's a +/-
	{
		i++;
	}
	while(s[i] != '\0')
	{
		if(s[i] < 48 || s[i] > 57)
		{
			printf("BigInteger Error: calling stringToBigInteger() on a string containing non-base ten digits\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}

	int j = 0;
	long temp = 0;
	BigInteger N = newBigInteger();
	if(s[0] == '-' || s[0] == '+')									//first element is + or -
	{
		if(s[0] == '-')
		{
			N->sign = -1;
		}
		else
		{
			N->sign = 1;
		}	
		for(i = strlen(s) - 1; i > 0; i--)
		{
			temp += ((s[i] - '0') * power(10, j));
			j++;
			if(j == POWER)
			{
				append(N->magnitude, temp);
				j = 0;
				temp = 0;
			}
		}
		if(j != 0)
		{
			append(N->magnitude, temp);
		}
	}
	else															//first element is not + or -
	{
		N->sign = 1;
		for(i = strlen(s) - 1; i >= 0; i--)
		{
			temp += ((s[i] - '0') * power(10, j));
			j++;
			if(j == POWER)
			{
				append(N->magnitude, temp);
				j = 0;
				temp = 0;
			}
		}
		if(j != 0)
		{
			append(N->magnitude, temp);
		}
	}

	while(back(N->magnitude) == 0 && length(N->magnitude) > 1)		//strip off any leading zeros
	{
		deleteBack(N->magnitude);
	}

	//If the result is in zero state set sign to 0
	if(back(N->magnitude) == 0)
	{
		N->sign = 0;
	}

	return (N);
}						
															
//Description: Returns a reference to a new BigInteger object in the same state as N.															
BigInteger copy(BigInteger N)
{
	if(N == NULL)
	{
		printf("BigInteger Error: calling copy() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	BigInteger C = newBigInteger();
	moveFront(N->magnitude);
	while(index(N->magnitude) != -1)
	{
		append(C->magnitude, get(N->magnitude));
		moveNext(N->magnitude);
	}

	C->sign = N->sign;
	return (C);
}					

//Description: Places the sum of A and B in the existing BigInteger S, overwriting its current state: S = A + B
void add(BigInteger S, BigInteger A, BigInteger B)
{
	if(S == NULL || A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling add() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	BigInteger C = sum(A, B);;
	freeList(&S->magnitude);
	S->sign = C->sign;
	S->magnitude = copyList(C->magnitude);
	freeBigInteger(&C);
}		
												
//Description: Returns a reference to a new BigInteger object representing A + B.															
BigInteger sum(BigInteger A, BigInteger B)
{
	if(A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling sum() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}
	BigInteger C;
	
	//Edge Cases
	// A & B = 0
	// A = 0
	// B = 0
	// A = B 
	if(length(A->magnitude) == 0 && length(B->magnitude) == 0)
	{
		C = newBigInteger();
		return (C);
	}
	else if(length(A->magnitude) == 0)
	{
		C = copy(B);
		return (C);
	}
	else if(length(B->magnitude) == 0)
	{
		C = copy(A);
		return (C);
	}
	else if(A == B)
	{
		BigInteger D = stringToBigInteger("2");
		C = prod(A, D);
		freeBigInteger(&D);
		return (C);
	}

	long temp;
	moveFront(A->magnitude);
	moveFront(B->magnitude);
	C = newBigInteger();

	while(index(A->magnitude) != -1 && index(B->magnitude) != -1)
	{
		temp = (get(A->magnitude) * sign(A)) + (get(B->magnitude) * sign(B));
		append(C->magnitude, temp);
		moveNext(A->magnitude);
		moveNext(B->magnitude);
	}
	
	if(index(A->magnitude) != -1)			//catch any remaining values in A and B
	{
		while(index(A->magnitude) != -1)
		{
			append(C->magnitude, (get(A->magnitude) * sign(A)));
			moveNext(A->magnitude);
		}
	}
	else if(index(B->magnitude) != -1)
	{
		while(index(B->magnitude) != -1)
		{
			append(C->magnitude, (get(B->magnitude) * sign(B)));
			moveNext(B->magnitude);
		}
	}

	//normalize the result and determine sign at the same time
	C->sign = normalize(C->magnitude, 1);

	//If the result is in zero state set sign to 0
	if(back(C->magnitude) == 0)
	{
		C->sign = 0;
		append(C->magnitude, 0);
	}
	return(C);
}			

//Description: Places the difference of A and B in the existing BigInteger D, overwriting its current state: D = A - B.		
void subtract(BigInteger D, BigInteger A, BigInteger B)
{
	if(D == NULL || A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling add() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	BigInteger C = diff(A, B);
	freeList(&D->magnitude);
	D->sign = C->sign;
	D->magnitude = copyList(C->magnitude);
	freeBigInteger(&C);
}	

//Description: Returns a reference to a new BigInteger object representing A - B.															
BigInteger diff(BigInteger A, BigInteger B)
{
	if(A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling sum() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}
	BigInteger C;

	//Edge Cases
	// A & B = 0
	// A = 0
	// B = 0
	// A = B 
	if( (A == B) || (sign(A) == 0 && sign(B) == 0) )
	{
		C = newBigInteger();
		append(C->magnitude, 0);
		return (C);
	}
	else if(sign(A) == 0)
	{
		C = copy(B);
		negate(C);
		return (C);
	}
	else if(sign(B) == 0)
	{
		C = copy(A);
		return (C);
	}

	long temp;
	moveFront(A->magnitude);
	moveFront(B->magnitude);
	C = newBigInteger();

	while(index(A->magnitude) != -1 && index(B->magnitude) != -1)
	{
		temp = (get(A->magnitude) * sign(A)) - (get(B->magnitude) * sign(B));
		append(C->magnitude, temp);
		moveNext(A->magnitude);
		moveNext(B->magnitude);
	}
	
	if(index(A->magnitude) != -1)			//catch any remaining values in A and B
	{
		while(index(A->magnitude) != -1)
		{
			append(C->magnitude, (get(A->magnitude) * sign(A)));
			moveNext(A->magnitude);
		}
	}
	else if(index(B->magnitude) != -1)
	{
		while(index(B->magnitude) != -1)
		{
			append(C->magnitude, (get(B->magnitude) * sign(B) * -1));
			moveNext(B->magnitude);
		}
	}

	//normalize the result and determine sign at the same time
	C->sign = normalize(C->magnitude, 1);

	//If the result is in zero state set sign to 0
	if(back(C->magnitude) == 0)
	{
		C->sign = 0;
	}
	return(C);
}				

//Description: Places the product of A and B in the existing BigInteger P, overwriting its current state: P = A*B
void multiply(BigInteger P, BigInteger A, BigInteger B)
{
	if(P == NULL || A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling multiply() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	BigInteger C = prod(A, B);
	freeList(&P->magnitude);
	P->sign = C->sign;
	P->magnitude = copyList(C->magnitude);
	freeBigInteger(&C);
}	

//Description: Returns a reference to a new BigInteger object representing A*B															
BigInteger prod(BigInteger A, BigInteger B)
{
	if(A == NULL || B == NULL)
	{
		printf("BigInteger Error: calling sum() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}

	int i;
	int j;
	int k;
	BigInteger C;
	BigInteger D;								

	//If either A or B is 0 return a BigInteger in the zero state.
	if(sign(A) == 0 || sign(B) == 0)												
	{
		C = newBigInteger();
		append(C->magnitude, 0);
		return (C);
	}

	//Create an array of lists equal to the number of digits in B															
	List *product = calloc(length(B->magnitude), sizeof(List));
	C = newBigInteger();
	//copy B in case A = B
	D = copy(B);
	moveFront(A->magnitude);
	moveFront(D->magnitude);

	for(i = 0; i < length(D->magnitude); i++)
	{
		product[i] = newList();

		for(j = 0; j < length(A->magnitude); j++)
		{
			append(product[i],  get(A->magnitude) * get(D->magnitude));
			moveNext(A->magnitude);
		}
		
		//prepend zeros based on the current iteration
		for(k = 0; k < i; k++)				
		{
			prepend(product[i], 0);
		}

		//The current list in the array much be normalized during each iteration in order to avoid storing numbers larger than the maximum value of long 
		normalize(product[i], 1);
		
		moveNext(D->magnitude);
		moveFront(A->magnitude);
	}

	//Iterate through the array of Lists and add the current List to the next List
	//The final result of the product will be found in the last element in the array of Lists
	long temp;
	for(i = 0; i < length(D->magnitude) - 1; i++)
	{
		moveFront(product[i]);
		moveFront(product[i + 1]);
		while(index(product[i]) != -1)
		{
			temp = get(product[i]) + get(product[i + 1]);
			set(product[i + 1], temp);
			moveNext(product[i]);
			moveNext(product[i + 1]);
		}
	}
	
	//Move the result to the return value
	moveFront(product[i]);
	while(index(product[i]) != -1)
	{
		append(C->magnitude, get(product[i]));
		moveNext(product[i]);
	}

	//Normalize after all the addition of Lists have taken place
	normalize(C->magnitude, 1);

	//Determine sign
	if(sign(A) == -1 && sign(B) == -1)
	{
		C->sign = 1;
	}
	else if(sign(A) == -1 || sign(B) == -1)
	{
		C->sign = -1;
	}
	else
	{
		C->sign = 1;
	}

	//Deallocate data associated to the array of Lists
	for(i = 0; i < length(B->magnitude); i++)
	{
		freeList(&product[i]);
		free(product[i]);
	}

	free(product);
	freeBigInteger(&D);
	return (C);
}				

//Other Operations*********************************************************************************************************************
//Description: Prints a base 10 string representation of N to filestream out.
void printBigInteger(FILE* out, BigInteger N)
{
	if(N == NULL)
	{
		printf("BigInteger Error: calling printBigInteger() on NULL BigInteger reference\n");
		exit(EXIT_FAILURE);
	}
	if(out == NULL)
	{
		printf("BigInteger Error: calling printBigInteger() on NULL FILE reference\n");
		exit(EXIT_FAILURE);
	}
	
	if(sign(N) == 0)
	{
		fprintf(out, "%d", 0);
	}
	else
	{
		if(N->sign == -1)
		{
			fprintf(out, "%c", '-');
		}

		moveBack(N->magnitude);
		fprintf(out, "%ld", get(N->magnitude));		//print the front of the number

		movePrev(N->magnitude);
		while(index(N->magnitude) != -1)
		{
			fprintf(out, "%0*ld", POWER, get(N->magnitude));
			movePrev(N->magnitude);
		}
	}

}				
