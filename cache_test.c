#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <string.h>
#include <unistd.h>

#define SIZE_1KB (1024)
#define SIZE_1MB (1024*1024)

#define NUMBER 64*SIZE_1MB
#define MILLION 1000000

__u64 rdtsc();

__u64 gettime();

int test_cache_linesize(int array[], int len, int K);

int test_cache_capacity(int array[], int cap);

int test_cache_associative(int array[], int size, int K);

int test_cache(int k);

int main(int argc, char **argv)
{	
	int i,k = 0;
	if(2 == argc)
	{
		for(i = 0; i < strlen(argv[1]); i++)
		{
			k *= 10;
			k += (int)(argv[1][i]-'0');
		}
	}
	else
		k = 0;	
	test_cache(k);
	return 0;
}

__u64 rdtsc()
{
	__u32 hi;
	__u32 lo;
	
	__asm__ __volatile__
	(
		"rdtsc":"=a"(lo),"=d"(hi)
	);
	
	return (__u64)hi<<32|lo;
}

__u64 gettime()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return ((__u64)(tv.tv_sec))*MILLION+tv.tv_usec;
}

int test_cache_linesize(int array[], int len, int K)
{
	int i,max = len*K;
	for(i = 0; i<=max; i+=K)
	{
		array[i] *= 3;
	}
	return 0;
}

int test_cache_capacity(int array[], int cap)
{
	int i;
	int lenmod = cap - 1;
	int times = 64*SIZE_1MB;
	for(i = 0; i<times; i++)
	{
		array[(i*32)&(lenmod)]++;/*32 means linesize/sizeof(int) = 32*/
	}
	return 0;
}

int test_cache_associative(int array[], int size, int K)
{
	int i;
	int cur = 0;
	__u64 begin;
	__u64 end;
	begin = gettime();
	for(i = 0; i<SIZE_1MB; i++)
	{
		array[cur]++;
		cur += K;
		if(cur >= size)
			cur = 0;
	}
	end = gettime();
	
	printf("When size = %10d, K = %10d : test_cache_associative cost %14llu us\n",size,K,end-begin);
	return 0;
}

int test_cache(int k)
{
	int *array = NULL;
	array = malloc(NUMBER*sizeof(int));
	__u64 begin;
	__u64 end;
	int i;
	int K = k;

 	int cap;
	int size;
	int temp1,temp2;
	if(array == NULL)
	{
		printf("Malloc space for array failed.\n");
		return -1;
	}

	for(i = 0; i<NUMBER; i++)
	{
		array[i] = i;
	}

	printf("----------test cachae linesize----------\n");

	if( 0 != k)
	{
		begin = rdtsc();		
		test_cache_linesize(array,64*1024,K);
		end = rdtsc();
		printf("when K = %5d multiply %10d times %8llu clicks average cost %llu clicks\n",K,64*1024,end-begin,(end-begin)/(64*1024));
	}
	else
		for(K = 1; K<=512; K *= 2)
		{
			begin = rdtsc();
			test_cache_linesize(array,1024*64,K);
			end = rdtsc();
			printf("when K = %5d multiply %10d times %8llu clicks average cost %llu clicks\n",K,64*1024,end-begin,(end-begin)/(64*1024));
		}

}
