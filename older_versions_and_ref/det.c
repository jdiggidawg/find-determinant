#include <stdio.h>
#include <stdlib.h>

// https://stackoverflow.com/questions/41384020/c-program-to-calculate-the-determinant-of-a-nxn-matrix
/*

Compile 
gcc -Wall det.c -o det 

*/

int det(int A[100][100], int n);

int main()
{
    int A[100][100];
    int i,j,n,res;
    printf("Enter the order of the matrix: \n");
    scanf("%d",&n);
    printf("\nEnter the elements of the matrix one by one: \n");
    for(i = 0 ; i < n ; i++)
    {
        for(j = 0 ; j < n ; j++)
        {
            scanf("%d",&A[i][j]);
        }
    }
    for(i = 0 ; i < n ; i++)
    {
        for(j = 0 ; j < n ; j++)
        {
            printf("%5d",A[i][j]);
        }
        printf("\n");
    }
    res = det(A,n);
    printf("Det: %d\n",res);

    return 0;
}
int det(int A[100][100], int n)
{
    int Minor[100][100];
    int i,j,k,c1,c2;
    int determinant;
    
    int sign =1;
    determinant = 0;

    if(n == 2)
    {
        
        determinant = A[0][0]*A[1][1]-A[0][1]*A[1][0];
        return determinant;
    }
    else
    {   
        for(i = 0 ; i < n ; i++)
        {
            c1 = 0, c2 = 0;

            for(j = 0 ; j < n ; j++)
            {
                for(k = 0 ; k < n ; k++)
                {
                    if(j != 0 && k != i)
                    {
                        Minor[c1][c2] = A[j][k];
                        c2++;
                        if(c2>n-2)
                        {
                            c1++;
                            c2=0;
                        }
                    }
                }
            }
            determinant = determinant + sign *(A[0][i]*det(Minor,n-1));
            sign = -1 * sign;
        }
    }
    return determinant;
}