#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COMPARE(x, y) ( ((x) < (y)) ? -1 : ((x) == (y)) ? 0 : 1)
#define MAX_TERMS 100
#define MAX_INPUT 1024

typedef struct {
    float coef;
    int expon;
}polynomial;

polynomial terms[MAX_TERMS];

int avail = 0;

void checkString(char*, int*, int*);
void padd(int, int, int, int, int*, int*);
void attach(float, int);

void main()
{
    char firstpoly[MAX_INPUT], secondpoly[MAX_INPUT];
    int startA, finishA, startB, finishB;
    int startD, finishD;
    int i;

    printf("첫번째 다항식을 입력하세요.\n");
    gets(firstpoly);
    checkString(firstpoly, &startA, &finishA);

    printf("두번째 다항식을 입력하세요.\n");
    gets(secondpoly);
    checkString(secondpoly, &startB, &finishB);

    padd(startA, finishA, startB, finishB, &startD, &finishD);

    for(i = startD; i <= finishD; i++) {
        if(terms[i].expon){
            printf("%.3fx^%d ", terms[i].coef, terms[i].expon);
        }
        else printf("%.3f", terms[i].coef);
        if(i != finishD) printf("+ ");
    }
}



void checkString(char *input, int *starting, int *finishing){
    *starting = avail;
    int i;
    int termCount = 0;
    char *ptr = strtok(input, " ");
    while(ptr != NULL){
        if(termCount%2 == 0) {      //When the input is a coefficient
            if(!atof(ptr)) {        //If the coefficient is zero, return error
                fprintf(stderr, "Invalid input.\n");
                exit(EXIT_FAILURE);
            }
            terms[*starting + termCount/2].coef = atof(ptr);
        }
        else terms[*starting + termCount/2].expon = atoi(ptr);
        ptr = strtok(NULL, " ");
        termCount++;
    }
    avail = *starting + (termCount + 1) / 2;
    *finishing = avail-1;

    /* Error Conditions */
    if(termCount % 2 == 1) {        //If the number of terms is odd, return error
        fprintf(stderr, "The coefficients and exponents must be sets.\n");
        exit(EXIT_FAILURE);
    }

    for(i = *starting; i <= (*finishing - 1); i++){     //If the exponents are not in descending order, return error
        if(terms[i].expon <= terms[i+1].expon ){
            fprintf(stderr, "Enter the polynomial in descending degree.\n");
            exit(EXIT_FAILURE);
        }
    }
}


void padd(int startA, int finishA, int startB, int finishB, int *startD, int *finishD) {
    float coefficient;
    *startD = avail;
    while( startA <= finishA && startB <= finishB){
        switch(COMPARE(terms[startA].expon, terms[startB].expon)){
            case -1:        //a expon < b expon
                    attach(terms[startB].coef, terms[startB].expon);
                    startB++;
                    break;
            case 0:         //equal exponents
                    coefficient = terms[startA].coef + terms[startB].coef;
                    if(coefficient)
                        attach(coefficient, terms[startA].expon);
                    startA++;
                    startB++;
                    break;
            case 1:         //a expon > b expon
                    attach(terms[startA].coef, terms[startA].expon);
                    startA++;
        }
    }
    //add in remaining terms of A
    for(;startA <= finishA; startA++) {
            attach(terms[startA].coef, terms[startA].expon);
    }
    //add in remaining terms of B
    for(;startB <= finishB; startB++) {
            attach(terms[startB].coef, terms[startB].expon);
    }
    *finishD = avail-1;
}

void attach(float coefficient, int exponent){
    if(avail >= MAX_TERMS){     //If the polynomial array is full, return error
        fprintf(stderr, "Too many terms in the polynomial\n");
        exit(EXIT_FAILURE);
    }
    terms[avail].coef = coefficient;
    terms[avail++].expon = exponent;
}
