/* 2025-07-23
 * This program is nothing special as it is a direct implementation of the Verhoeff
 * checksum.
 *
 * We have lookup tables where each digit gets scrambled based on position then that
 * digit gets combined with the running checksum using the multiplication table and finally
 * if we are generating a check digit we get the inverse of that to equal 0 (If correct)
 *
 * This works because the mathematical structure of the D5 group ensures that valid numbers
 * always produce a checksum of 0 and invalid produce non zero.
 *
 * Compile with:
 * gcc -std=c2x -O3 -march=native -flto -Wall -o ver verhoeff.c
 */
#include <stdio.h>
#include <stdlib.h>

// Multiplication Table
int d[10][10] = {
                    { 0,1,2,3,4,5,6,7,8,9 },
                    { 1,2,3,4,0,6,7,8,9,5 },
                    { 2,3,4,0,1,7,8,9,5,6 },
                    { 3,4,0,1,2,8,9,5,6,7 },
                    { 4,0,1,2,3,9,5,6,7,8 },
                    { 5,9,8,7,6,0,4,3,2,1 },
                    { 6,5,9,8,7,1,0,4,3,2 },
                    { 7,6,5,9,8,2,1,0,4,3 },
                    { 8,7,6,5,9,3,2,1,0,4 },
                    { 9,8,7,6,5,4,3,2,1,0 }
                };

// Inverse table
int inv[10] = { 0,4,3,2,1,5,6,7,8,9 };

// Permutation table
int num[8][10] = {
                    { 0,1,2,3,4,5,6,7,8,9 },
                    { 1,5,7,6,2,8,3,0,9,4 },
                    { 5,8,0,3,7,9,6,1,4,2 },
                    { 8,9,1,6,0,4,3,5,2,7 },
                    { 9,4,5,3,1,2,6,8,7,0 },
                    { 4,2,8,6,5,7,3,9,0,1 },
                    { 2,7,9,3,8,0,6,4,1,5 },
                    { 7,0,4,6,9,1,3,2,5,8 }
                 };

/* Behold, Verhoeff checksum */
int checksum(int n[], int len) {
    register int c = 0;
    register int i = 0;

    for (i = 0; i < len; i++) {
        c = d[c][num[i % 8][n[i]]];
    }

    return n[0] == 0 ? inv[c] : c;
}

int main([[maybe_unused]] int argc, char *argv[argc+1])  {
    // Verify comand line argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <check digit><number>, use 0 to generate\n", argv[0]);
	return -1;
    }

    int pn[64];
    register int i = 0;

    for (i = 0; argv[1][i]; i++) {
        if (i > 63) {
	    fprintf(stderr, "Number is too big\n");
	    return -1;
	}
	if (argv[1][i] < '0' || argv[1][i] > '9') {
	    fprintf(stderr, "Not a valid number\n");
	    return -1;
	}

    	pn[i] = argv[1][i] - '0'; 
    }  

    /* We need to reverse the digits except the first digit which is either 0
     * or the check digit which should always be first */
    for (register int j = 1; j < (i-1)/2 + 1; j++) {
        int temp = pn[j];
        pn[j] = pn[i-j];
        pn[i-j] = temp;
    }

    int res = checksum(pn, i);

    if (!pn[0]) {
	printf("check digit = %d\n", res);
    } else {
        printf("%s\n", res == 0 ? "valid" : "invalid");
    }

    return 0;
}          
