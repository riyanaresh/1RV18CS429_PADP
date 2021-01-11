#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

void check(int **arr1, int **arr2){
	int i,j,k, r=10,c=10;
	int **arr3 = (int **)malloc(r * sizeof(int *));
	printf("\nChecking for correctness of program : \n");
	printf("\n MATRIX I \n");
	
	for(i = 0;i < r; i++){
		for(j = 0;j < c; j++){
			printf("%d ",arr1[i][j]);
		}
		printf("\n");
	}
	printf("\n MATRIX II \n");
	for(i = 0;i < r; i++){
		for(j = 0;j < c; j++){
			printf("%d ",arr2[i][j]);
		}
		printf("\n");
	}
	
	for (i=0; i<r; i++)
		arr3[i] = (int *)malloc(c * sizeof(int));

	double x = omp_get_wtime();
	for(i = 0;i < r; i++)
			for(j = 0;j < c; j++)
				for(k = 0;k < r; k++)
					arr3[i][j] += arr1[i][k] * arr2[k][j];
	double y = omp_get_wtime();

	printf("\nTime for serial %lf\t\t", y-x);
	printf("\n FINAL MATRIX SERIAL \n");
	for(i = 0;i < r; i++){
		for(j = 0;j < c; j++){
			printf("%d ",arr3[i][j]);
		}
		printf("\n");
	}
	
}


int main(){
	int it=1;
	
	while(it<=4){
		int r = 100*it, c = 100*it, i, j, sum =0, k;
		if(it==1)
			r = 10, c = 10;

		int **arr1 = (int **)malloc(r * sizeof(int *));
		for (i=0; i<r; i++)
			arr1[i] = (int *)malloc(c * sizeof(int));
		int **arr2 = (int **)malloc(r * sizeof(int *));
		for (i=0; i<r; i++)
			arr2[i] = (int *)malloc(c * sizeof(int));
		int **arr3 = (int **)malloc(r * sizeof(int *));
		for (i=0; i<r; i++)
			arr3[i] = (int *)malloc(c * sizeof(int));

		for(i = 0;i < r; i++)
			for(j = 0;j < c; j++){
				arr1[i][j] = (rand()/r)%10;
			}

		for(i = 0;i < r; i++)
			for(j = 0;j < c; j++)
				arr2[i][j] = (rand()/r)%10;

		if(it==1){
			double x = omp_get_wtime();
			for(i = 0;i < r; i++)
				for(j = 0;j < c; j++)
					for(k = 0;k < r; k++)
						arr3[i][j] += arr1[i][k] * arr2[k][j];
			double y = omp_get_wtime();
			check(arr1, arr2);
			printf("\nTime for parallel %lf\t\t", y-x);
			printf("\n FINAL MATRIX PARALLEL \n");
			for(i = 0;i < r; i++){
				for(j = 0;j < c; j++){
					printf("%d ",arr3[i][j]);
				}
				printf("\n");
			}
            printf("\nThe execution time are\nSize\t\t1\t\t\t2\t\t\t4\t\t\t8\n");
		}


		printf("%d\t\t",r);
		for(int p=1;p<=8;p=p*2)
		{
			double x = omp_get_wtime();
			omp_set_num_threads(p);
			#pragma omp parallel for private(j, k) shared(i)
			for(int i = 0;i < r; i++)
				for(j = 0;j < c; j++)
				{
					arr3[i][j]=0;
					for(k = 0;k < r; k++)
						arr3[i][j] += arr1[i][k] * arr2[k][j];
				}
			double y = omp_get_wtime();
			printf("%lf\t\t", y-x);
		}
		printf("\n");
		it++;
	}
	return 0;
}
