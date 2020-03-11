#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 50
#define MALLOC(p, s) \
    if(!((p) = malloc(s))) {\
        fprintf(stderr, "Insufficient memory");\
        exit(EXIT_FAILURE);\
    }
#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))
#define COMPARE(x, y) (((x) > (y)) ? (1) : ((x) == (y)) ? (0) : (-1))

typedef enum {head, entry} tagfield;
typedef struct matrixNode *matrixPointer;
typedef struct
{
    int row;
    int col;
    int value;
} entryNode;
typedef struct matrixNode
{
    matrixPointer down;
    matrixPointer right;
    tagfield tag;
    union
    {
        matrixPointer next;
        entryNode entry;
    } u;
};
matrixPointer hdnode[MAX_SIZE];
int endmatrix = 0;  //the last index number of hdnode

matrixPointer newNode()
{
    matrixPointer temp;
    MALLOC(temp, sizeof(*temp));
    return temp;
}

void attach(int row, int col, int value, matrixPointer *ptr)
{
    matrixPointer temp;
    MALLOC(temp, sizeof(*temp));
    temp->tag = entry;
    temp->u.entry.row = row;
    temp->u.entry.col = col;
    temp->u.entry.value = value;
    (*ptr)->right = temp;
    *ptr = temp;
}

matrixPointer mread(void)
{
    int numRows, numCols, numTerms, numHeads, i;
    int row, col, value, currentRow;
    matrixPointer temp, last, node;
    printf(" Please enter the number of rows, columns, and nonzero values\n");
    printf(" The input rows, columns and nonzero values must be divided by spaces : ");
    scanf("%d%d%d", &numRows, &numCols, &numTerms);
    while(numTerms > numRows*numCols || numRows <= 0 || numCols <= 0)
    {
        if(numRows <= 0) printf(" The number of rows cannot be less than 0 : ");
        else if(numCols <= 0) printf(" The number of columns cannot be less than 0 : ");
        else if(numTerms > numRows*numCols) printf(" The number of nonzero values cannot be greater than the matrix size : ");
        scanf("%d%d%d", &numRows, &numCols, &numTerms);
    }
    numHeads = (numCols > numRows) ? numCols : numRows;
    node = newNode(); node->tag = entry;
    node->u.entry.row = numRows;
    node->u.entry.col = numCols;
    node->u.entry.value = numTerms;

    if(!numHeads) node->right = node;
    else
    {
        for(i = endmatrix; i < endmatrix + numHeads; i++)
        {
            temp = newNode();
            hdnode[i] = temp; hdnode[i]->tag = head;
            hdnode[i]->right = temp; hdnode[i]->u.next = temp;
        }
        currentRow = 0;
        last = hdnode[endmatrix];
        for(i = 0; i < numTerms; i++)
        {
            printf(" Please input row, column and value : ");
            scanf("%d%d%d", &row, &col, &value);
            while(row >= numRows || col >= numCols)
            {
                printf(" The input cannot be larger than the matrix size\n Please input again : ");
                scanf("%d%d%d", &row, &col, &value);
            }
            if(row > currentRow)
            {
                last->right = hdnode[endmatrix + currentRow];
                currentRow = row; last = hdnode[endmatrix + row];
            }
            MALLOC(temp, sizeof(*temp));
            temp->tag = entry; temp->u.entry.row = row; temp->u.entry.col = col; temp->u.entry.value = value;
            last->right = temp;
            last = temp;
            hdnode[endmatrix + col]->u.next->down = temp;
            hdnode[endmatrix + col]->u.next = temp;
        }
        last->right = hdnode[endmatrix + currentRow];
        for(i = endmatrix; i < endmatrix + numCols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        for(i = endmatrix; i< endmatrix + numHeads-1; i++)
            hdnode[i]->u.next = hdnode[i+1];
        hdnode[endmatrix + numHeads-1]->u.next = node;
        node->right = hdnode[endmatrix];
        endmatrix = endmatrix + numHeads;
    }
    return node;
}

void mwrite(matrixPointer node)
{
    int i, countnode = 1;
    matrixPointer head = node->right; matrixPointer temp;
    printf(" \n # of ROWS = %d, # of COLS = %d \n", node->u.entry.row, node->u.entry.col);
    printf(" The input rows, columns, and value are : \n");
    for(i = 0; i < node->u.entry.row; i++)
    {
        for(temp = head->right; temp != head; temp = temp->right){
            printf(" [%2d] value of matrix : ", countnode);
            printf("%5d%5d%5d \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
            countnode++;
        }
        head = head->u.next;
    }
}

void merase(matrixPointer *node)
{
    matrixPointer x, y, head = (*node)->right;
    int i;
    int numHeads = ((*node)->u.entry.row > (*node)->u.entry.col) ? (*node)->u.entry.row : (*node)->u.entry.col;
    for(i = 0; i<(*node)->u.entry.row; i++)
    {
        y = head->right;
        while (y != head)
        {
            x = y; y = y->right; free(x);
        }
        x = head; head = head->u.next; free(x);
    }
    y = head;
    while(y != *node)
    {
        x = y; y = y->u.next; free(x);
    }
    free(*node); *node = NULL;
    endmatrix = endmatrix - numHeads;
}

void mtranspose(matrixPointer *node)
{
    int i, temp, numHeads;
    numHeads = ((*node)->u.entry.col > (*node)->u.entry.row) ? (*node)->u.entry.col : (*node)->u.entry.row;
    matrixPointer temp_swap, temp_count, head = (*node)->right;
    for(i = 0; i < numHeads; i++)
    {
        SWAP(head->down, head->right, temp_swap);
        head = head->u.next;
    }
    head = head->right;
    for(i = 0; i<(*node)->u.entry.row; i++)
    {
        for(temp_count = head->down; temp_count != head; temp_count = temp_count->down)
        {
            SWAP(temp_count->down, temp_count->right, temp_swap);
            SWAP(temp_count->u.entry.row, temp_count->u.entry.col, temp);
        }
        head = head->u.next;
    }
    SWAP((*node)->u.entry.row, (*node)->u.entry.col, temp);
}

matrixPointer madd(matrixPointer first, matrixPointer second)
{
    if(!(first->u.entry.row == second->u.entry.row && first->u.entry.col == second->u.entry.col))
    {
        fprintf(stderr, " The size of the two matrices has to be the same in order to proceed addition \n Please start the program again \n");
        merase(&first);
        merase(&second);
        exit(EXIT_FAILURE);
    }
    matrixPointer node, temp, last, headfirst = first->right, headsecond = second->right;
    matrixPointer nodefirst = headfirst->right, nodesecond = headsecond->right;
    int i, sum, numHeads = (first->u.entry.row > first->u.entry.col) ? first->u.entry.row : first->u.entry.col;
    int count_add = 0;
    node = newNode(); node->tag = entry;
    node->u.entry.row = first->u.entry.row;
    node->u.entry.col = second->u.entry.row;
    for(i = endmatrix; i < endmatrix + numHeads; i++)
    {
        temp = newNode();
        hdnode[i] = temp; hdnode[i]->tag = head;
        hdnode[i]->right = temp; hdnode[i]->u.next = temp;
    }
    for(i = endmatrix; i < endmatrix + numHeads; i++)
    {
        last = hdnode[i];
        while((nodefirst != headfirst) && (nodesecond != headsecond))
        {
            switch(COMPARE(nodefirst->u.entry.col, nodesecond->u.entry.col))
            {
            case -1: //first col < second col
                attach(nodefirst->u.entry.row, nodefirst->u.entry.col, nodefirst->u.entry.value, &last);
                hdnode[endmatrix + nodefirst->u.entry.col]->u.next->down = last;
                hdnode[endmatrix + nodefirst->u.entry.col]->u.next = last;
                nodefirst = nodefirst->right;
                break;
            case 0: //first col = second col
                count_add++;
                sum = nodefirst->u.entry.value + nodesecond->u.entry.value;
                if(sum)
                {
                    attach(nodefirst->u.entry.row, nodefirst->u.entry.col, sum, &last);
                    hdnode[endmatrix + nodefirst->u.entry.col]->u.next->down = last;
                    hdnode[endmatrix + nodefirst->u.entry.col]->u.next = last;
                }
                nodefirst = nodefirst->right;
                nodesecond = nodesecond->right;
                break;
            case 1: //first col > second col
                attach(nodesecond->u.entry.row, nodesecond->u.entry.col, nodesecond->u.entry.value, &last);
                hdnode[endmatrix + nodesecond->u.entry.col]->u.next->down = last;
                hdnode[endmatrix + nodesecond->u.entry.col]->u.next = last;
                nodesecond = nodesecond->right;
            }
        }
        for(;nodefirst != headfirst; nodefirst = nodefirst->right)
        {
            attach(nodefirst->u.entry.row, nodefirst->u.entry.col, nodefirst->u.entry.value, &last);
            hdnode[endmatrix + nodefirst->u.entry.col]->u.next->down = last;
            hdnode[endmatrix + nodefirst->u.entry.col]->u.next = last;
        }
        for(;nodesecond != headsecond; nodesecond = nodesecond->right)
        {
            attach(nodesecond->u.entry.row, nodesecond->u.entry.col, nodesecond->u.entry.value, &last);
            hdnode[endmatrix + nodesecond->u.entry.col]->u.next->down = last;
            hdnode[endmatrix + nodesecond->u.entry.col]->u.next = last;
        }
        last->right = hdnode[i];
        headfirst = headfirst->u.next; nodefirst = headfirst->right;
        headsecond = headsecond->u.next; nodesecond = headsecond->right;
    }
    for(i = endmatrix; i < endmatrix + node->u.entry.col; i++)
        hdnode[i]->u.next->down = hdnode[i];
    for(i = endmatrix; i< endmatrix + numHeads-1; i++)
        hdnode[i]->u.next = hdnode[i+1];
    hdnode[endmatrix + numHeads-1]->u.next = node;
    node->right = hdnode[endmatrix];
    node->u.entry.value = first->u.entry.value + second->u.entry.value - count_add;
    endmatrix = endmatrix + numHeads;
    return node;
}

matrixPointer mmult(matrixPointer first, matrixPointer second)
{
    if(!(first->u.entry.col == second->u.entry.row))
    {
        fprintf(stderr, " Number of columns in first matrix and number of rows in second matrix has to be the same in order to proceed multiplication \n Please start the program again\n");
        merase(&first);
        merase(&second);
        exit(EXIT_FAILURE);
    }
    matrixPointer node, temp, last, headfirst = first->right, headsecond = second->right;
    matrixPointer nodefirst = headfirst->right, nodesecond = headsecond->down;
    int i, j, sum, currentSum, numHeads = (first->u.entry.row > second->u.entry.col) ? first->u.entry.row : second->u.entry.col;
    int currentRow = 0;
    int countVal = 0;
    node = newNode(); node->tag = entry;
    node->u.entry.row = first->u.entry.row;
    node->u.entry.col = second->u.entry.col;
    for(i = endmatrix; i < endmatrix + numHeads; i++)
    {
        temp = newNode();
        hdnode[i] = temp; hdnode[i]->tag = head;
        hdnode[i]->right = temp; hdnode[i]->u.next = temp;
    }
    for(i = 0; i < first->u.entry.row; i++)
    {
        last = hdnode[endmatrix + i];
        for(j = 0; j < second->u.entry.col; j++)
        {
            currentSum = 0;
            while((nodefirst != headfirst) && (nodesecond != headsecond))
            {
                switch(COMPARE(nodefirst->u.entry.col, nodesecond->u.entry.row))
                {
                case -1:    //first col < second row
                    nodefirst = nodefirst->right;
                    break;
                case 0:
                    currentSum = currentSum + (nodefirst->u.entry.value)*(nodesecond->u.entry.value);
                    nodefirst = nodefirst->right;
                    nodesecond = nodesecond->down;
                    break;
                case 1:     //first col > second row
                    nodesecond = nodesecond->down;
                }
            }
            if(currentSum)
            {
                countVal++;
                attach(i, j, currentSum, &last);
                hdnode[endmatrix + j]->u.next->down = last;
                hdnode[endmatrix + j]->u.next = last;
            }
            headsecond = headsecond->u.next; nodesecond = headsecond->down;
            nodefirst = headfirst->right;
        }
        last->right = hdnode[endmatrix + i];
        headfirst = headfirst->u.next; nodefirst = headfirst->right;
        headsecond = second->right; nodesecond = headsecond->down;
    }
    for(i = endmatrix; i < endmatrix + second->u.entry.col; i++)
        hdnode[i]->u.next->down = hdnode[i];
    for(i = endmatrix; i< endmatrix + numHeads-1; i++)
        hdnode[i]->u.next = hdnode[i+1];
    hdnode[endmatrix + numHeads-1]->u.next = node;
    node->right = hdnode[endmatrix];
    node->u.entry.value = countVal;
    endmatrix = endmatrix + numHeads;
    return node;
}

void main()
{
    int userinput;
    matrixPointer matrix1, matrix2, mresult;
    printf(" __________________________________________________________________________________\n\n\n");
    printf("                        COSE213 Assignment#2 - Sparse Matrices -\n\n");
    printf(" __________________________________________________________________________________\n");
    printf("                                                               2016320213 김형민\n");
    printf("    ___________________________________________________________________________    \n\n");
    printf("                                 What will you do?\n\n");
    printf("             [1] Input New Matrix                  [2] Exit Program  \n");
    printf("    ___________________________________________________________________________    \n    >> ");
    scanf("%d", &userinput);
    if(userinput != 1) {printf("\n Exiting Program \n\n"); exit(0);}
    system("cls");
    do{
        printf("\n [ Input First Matrix ]\n");
        matrix1 = mread();
        system("cls");
        printf(" ___________________________________________________________________________    \n\n");
        printf("                              What will you do?\n\n");
        printf("           [1] Print Matrix                  [2] Input New Matrix\n           [3] Transpose Matrix              [4] Erase Matrix\n");
        printf(" ___________________________________________________________________________    \n >> ");
        scanf("%d", &userinput);
        while(userinput == 1)
        {
            printf(" The input matrix is as follows : \n");
            mwrite(matrix1);
            printf("\n >> ");
            scanf("%d", &userinput);
        }
        if(userinput == 4)  //erase matrix
        {
            merase(&matrix1);
            system("cls");
            printf("\n The matrix has been erased \n Please input a new matrix \n\n");
        }
    }while(userinput == 4);
    if(userinput == 3) //transpose matrix
    {
        system("cls");
        printf(" The input matrix is as follows : \n");
        mwrite(matrix1);
        printf("\n The transposed matrix is as follows : \n");
        mtranspose(&matrix1);
        mwrite(matrix1);
        printf("\n The matrix has been transposed  \n Exiting Program \n\n");
        merase(&matrix1);
        exit(EXIT_SUCCESS);
    }
    else if(userinput == 2)
    {
        system("cls");
        do{
            printf("\n [ Input Second Matrix ]\n");
            matrix2 = mread();
            system("cls");
            printf(" ___________________________________________________________________________    \n\n");
            printf("                             What will you do?\n\n");
            printf("           [1] Print Matrix                       [2] Add Matrices \n           [3] Multiply Matrices                  [4] Erase Matrix\n");
            printf(" ___________________________________________________________________________    \n >> ");
            scanf("%d", &userinput);
            while(userinput == 1)
            {
                printf(" The input matrix is as follows : \n");
                mwrite(matrix1);
                mwrite(matrix2);
                printf("\n >> ");
                scanf("%d", &userinput);
            }
            if(userinput == 4)
            {
                merase(&matrix2);
                system("cls");
                printf("\n The matrix has been erased \n Please input a new matrix \n\n");
            }
        }while(userinput == 4);
        if(userinput == 2) //matrix addition
        {
            system("cls");
            printf(" The input matrices are : \n");
            mwrite(matrix1);
            mwrite(matrix2);
            printf(" \n\n");
            mresult = madd(matrix1, matrix2);
            printf(" The result matrix is : \n");
            mwrite(mresult);
            printf("\n The matrices have been added \n Exiting Program \n\n");
            merase(&mresult);
            merase(&matrix2);
            merase(&matrix1);
            exit(EXIT_SUCCESS);
        }
        else if(userinput == 3) //matrix multiplication
        {
            system("cls");
            printf(" The input matrices are : \n");
            mwrite(matrix1);
            mwrite(matrix2);
            printf(" \n\n");
            mresult = mmult(matrix1, matrix2);
            printf(" The result matrix is : \n");
            mwrite(mresult);
            printf("\n The matrices have been multiplied \n Exiting Program \n\n");
            merase(&mresult);
            merase(&matrix2);
            merase(&matrix1);
            exit(EXIT_SUCCESS);
        }
    }
}
