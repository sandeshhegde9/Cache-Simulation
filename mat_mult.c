#include<stdio.h>
int main()
{
int N=100;
int A[N][N],B[N][N],C[N][N];
int i, j, k;
for (i = 0; i < N; i++)
{
	for (j = 0; j < N; j++)
	{
		//C[i][j] = 0;
		printf("%p\n",&C[i][j]);
		for (k = 0; k < N; k++)
		//C[i][j] += A[i][k]*B[k][j];
		printf("%p\n%p\n%p\n",&A[i][k],&B[k][i],&C[i][j]);
		//printf("%d\n",sizeof(int));
	}
}return 0;
}
