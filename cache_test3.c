#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <string.h>
#include <unistd.h>

#define SIZE_1KB (1024)
#define SIZE_1MB (1024*1024)

#define NUMBER 64*SIZE_1MB
#define MILLION 1000000

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

int main(int argc,char **argv)
{
	__u64 begin,end;	
	int i,j,k,gap=0;
	int *array1,*array2,*res;
	int b = 0;
	array1 = malloc(NUMBER*sizeof(int));
	array2 = malloc(NUMBER*sizeof(int));
	res = malloc(NUMBER*sizeof(int));
	for(i = 0; i<NUMBER; i++)
	{
		array1[i] = 1;
		array2[i] = 2;
	}
	
	if(argc == 2)
	{
		for(i = 0; i < strlen(argv[1]); i++)
		{
			gap *= 10;
			gap += (int)(argv[1][i]-'0');
		}
		begin = rdtsc();
		for(j = 0; j<gap; j++)
			for(k = j; k<NUMBER; k += gap)
				res[k] = array1[k]*array2[k];
		end = rdtsc();
		printf("Gap: %5d Calculate:%16llu clicks average:%5llu clicks\n",gap,end-begin,((end-begin)/(NUMBER)));
	}
	else
	{
		for(i = 1; i <= 131072; i *= 2)
		{
			begin = rdtsc();
			for(j = 0; j<i; j++)
				for(k = j; k<NUMBER; k += i)
					res[k] = array1[k]*array2[k];
			end = rdtsc();
			printf("Gap: %5d Calculate:%16llu clicks average:%5llu clicks\n",i,end-begin,((end-begin)/(NUMBER)));
		}
	}
	return 0;
}
