/*
Find determinants of NxN matrix using multithreading
Name: Jessie Tran
Student ID: 301262354
pmtran@sfu.ca
Course: CMPT300
File: find_det.c

Compile:

gcc --std=c99 -Wall -g -Werror -lpthread find_det.c -o find_det

*/

/*
Helpful sources:
https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
https://www.geeksforgeeks.org/determinant-of-a-matrix/
https://stackoverflow.com/questions/16869687/assigning-one-2d-array-to-another
https://stackoverflow.com/questions/14285230/assigning-values-to-additional-rows-in-2d-array-in-c
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>



// Function declarations
long runFindDeterminant(long **mat, int n); 
void *determinantOfMatrix(void *arg);
void print2DSquareMatrix( long **M, int N);
void getSubMatrix(long **orig, long **submatrix, int p, int q, int N);
long ** newRandomMatrix(int n);
void free2DArray(long** arr, int n);
void runSamples ();



typedef struct {
    int size;
    long ** matrix; 
    long det_ans;
    pthread_t tid;
} find_det_t;

//--------------------------------------------------------------------------------
// MAIN
//--------------------------------------------------------------------------------

int main() {
    
    runSamples(); 

    int n; // Size of n X n matrix

    while(1) {
        printf("\n\nEnter size of a random NxN matrix:\n");

        scanf ("%d",&n);
        
        long **new = newRandomMatrix(n);

        printf("Here's your matrix:\n");
        
        print2DSquareMatrix(new,n);
        
        printf("Calculating the determinant...\n");
        
        long det = runFindDeterminant(new,n); 
        
        printf("Determinant: %ld\n", det);  

    }
    
    return 0;
}
//--------------------------------------------------------------------------------
// DETERMINANT FUNCTIONS
//--------------------------------------------------------------------------------

// Matrix passed into this funciton will be freed in the end.
// Trying to access will result in segmentation fault
long runFindDeterminant(long **mat, int n) {

    find_det_t *find_d = malloc(sizeof(find_det_t));
        
    find_d->size = n;
    find_d->matrix = mat;
    find_d->det_ans = 0;

    pthread_create(&find_d->tid, NULL, determinantOfMatrix, (void * )find_d);
    
    // Wait for thread termination
    pthread_join(find_d->tid, NULL);

    long res = find_d->det_ans;
    
    //Clean up;
    free2DArray(find_d->matrix, find_d->size);
    free(find_d);

    return res;
}



// Replace recursive function for finding determinant of matrix using threads
// n is current dimension of mat[][].
void *determinantOfMatrix(void *arg)
{
    find_det_t *find_d = (find_det_t *) arg;
    
    int n = find_d->size;
    
    long **mat = find_d->matrix;
   
    //  Base case : if matrix contains single element
    if (n == 1) {
        find_d->det_ans = mat[0][0];
        pthread_exit(0);
    }
    
    int sign = 1;  // To store sign multiplier
 
    // Spawn sub threads to help calculate submatrices' determinants
    
    find_det_t sub_find_d[n]; 

     // Iterate according to each col in the first row of the original matrix
    for (int i = 0; i < n; i++)
    {
        sub_find_d[i].size = n-1;
        sub_find_d[i].det_ans = 0;
        
        // Getting Submatrix of mat[0][col]
        long **submatrix = (long**) malloc (n * sizeof(long*));    
        for (int i = 0; i < n; i++) {
            submatrix[i] = (long *) malloc (n * sizeof(long));
        }
        
        getSubMatrix(mat, submatrix, 0, i, n);
        
        sub_find_d[i].matrix = submatrix;   

        //Replace recursive part with thread create
        pthread_create(&sub_find_d[i].tid, NULL, determinantOfMatrix, (void *) &sub_find_d[i]);
        
    }

    // Wait for sub threads to terminate
    // Calculations are saved in find_det_t structs
    for (int i=0; i < n; i++) {
        
        pthread_join(sub_find_d[i].tid, NULL);
    
        // Add or minus det to parent's det
        find_d->det_ans += sign * mat[0][i] * sub_find_d[i].det_ans;
        
        //terms are to be added with alternate sign
        sign = -sign;

        // Free submatrix after its calculation is done
        free2DArray(sub_find_d[i].matrix, n-1);
    }

    
    pthread_exit(0);
    
}



void getSubMatrix(long **orig, long **submatrix, int p, int q, int N) {
    int row,col;
    int i = 0, j = 0;

    for(row = 0; row < N; row++) {
        for(col = 0; col < N; col++) {
            // To create submatrix ignore pth row and jth column            
            if ( (row != p) && (col != q) ) {
                // j starts at 0 and iterates, just like j++ in a for loop
                submatrix[i][j++] = orig[row][col]; 
                if (j == N - 1) { 
                // Row of submatrix is filled, so increase row index
                    i++;
                // And  reset col index
                    j = 0;
                }
            }
        }
    }
}



//--------------------------------------------------------------------------------
// HELPER FUNCTIONS
//--------------------------------------------------------------------------------
// limit entries inside the matrix to int <= 1000

long ** newRandomMatrix(int n) {
    
    // Dynamically allocate a 2D array structure
    long **mat = (long **)malloc(n * sizeof(long *));        
    
    for (int i=0; i<n; i++)
        mat[i] = (long *) malloc(n* sizeof(long));

    // Fill with random numbers
    srand((signed)time(NULL));    
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            mat[r][c] = rand() % 2001 + (-1000);            
        }
    }
    return mat;
}


void free2DArray(long** arr, int n) {
    
    for (int i = 0; i < n; i++) {
        long* currentPtr = arr[i];
        free(currentPtr);
    }
    free(arr);
}

void print2DSquareMatrix( long **M, int N ) {   
    for (int i = 0; i <  N; i++) {
      for (int j = 0; j < N; j++) {
         printf("%5ld ", M[i][j]);
      }
      printf("\n");
    }
}

void runSamples () {
    int n = 4;
    
    //Determinant = -250 
    long sample1[4][4] = {{0, 4, 0, -3},
                      {1, 1, 5, 2},
                      {1, -2, 0, 6},
                      {3, 0, 0, 1}};
    //Determinant = 30
    
    long sample2[4][4] = {{1, 0, 2, -1},
                     {3, 0, 0, 5},
                     {2, 1, 4, -3},
                     {1, 0, 5, 0} };
    
    // Dynamically allocate the 2D arrays to pass into runFindDeterminant()
    long **mat1 = (long**) malloc (n * sizeof(long*));
    long **mat2 = (long**) malloc (n * sizeof(long*));      
    
    for (int i = 0; i < n; i ++) {
        mat1[i] = (long *) malloc (n * sizeof(long));
        mat2[i] = (long *) malloc (n * sizeof(long));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat1[i][j] = sample1[i][j];
            mat2[i][j] = sample2[i][j];
        }
    }
                
    printf("Sample 1:\n");    
    print2DSquareMatrix(mat1,n);
    long det1 = runFindDeterminant(mat1, n);
    printf("Determinant:  %ld\n", det1);
    
    printf("\n");

    printf("Sample 2:\n");
    print2DSquareMatrix(mat2,n);
    long det2 = runFindDeterminant(mat2, n);
    printf("Determinant:  %ld\n", det2);
            
}


