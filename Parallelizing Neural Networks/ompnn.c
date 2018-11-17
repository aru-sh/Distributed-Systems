#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#include<math.h>



#define size 4000

float sigmoid(float x)
{
     float exp_value;
     float return_value;
     exp_value = exp((float) -x);
     return_value = 1 / (1 + exp_value);
     return return_value;

}


int main()
{
	float f;
	int i;
	int j;
//	int size;
//	printf("Enter the number of neurons: ");
//	scanf("%d",&size);

	float weight[size][size];
	float layer[size][size];
	float output[size][size];
/*	float **weight = (float **)malloc(size * sizeof(float *)); 

    for (i=0; i<size; i++) 
         weight[i] = (float *)malloc(size * sizeof(float)); 


	float **layer = (float **)malloc(size * sizeof(float *)); 

    for (i=0; i<size; i++) 
         layer[i] = (float *)malloc(size * sizeof(float)); 
	
	float **output = (float **)malloc(size * sizeof(float *)); 

    for (i=0; i<size; i++) 
         output[i] = (float *)malloc(size * sizeof(float)); */

    // Initialize buffers.
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
			f=rand()%1000+1;
			f=f/1000;           
			weight[i][j] = f;
            output[i][j] = 0.0f;
        }
    }
	
	for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
			if(j==0){
			f=rand()%1000+1;
			f=f/1000;
			layer[i][j]=f;
			}
			else{
			layer[i][j]=layer[i][j-1];
			}
		}
	}

	

	double time_spent=0.0;

	clock_t begin=clock();


//    #pragma omp parallel for default(none) shared(weight,layer,output,size)
	#pragma omp parallel for default(none) shared(weight,layer,output)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                output[i][j] += weight[i][k] * layer[k][j];
            }
        }
    }
//	#pragma omp parallel for default(none) shared(output,size)
	#pragma omp parallel for default(none) shared(output)
	for (int i = 0; i < size; ++i) {
        output[i][0]=sigmoid(output[i][0]);
	}
	
	clock_t end=clock();

	time_spent+=(double)(end-begin)/CLOCKS_PER_SEC;

	//printf("Time elapsed is %f seconds", time_spent);
	
	//free(layer);
	//free(weight);
	//free(output);

    return 0;
}
