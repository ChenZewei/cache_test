#include <stdio.h>
#include <stdlib.h>
#include <linux/types.h>
#include <string.h>
#include <unistd.h>

#define SIZE 1024
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
	int array[SIZE][SIZE];
	int i,j,begin,end;

	if(!strcmp(argv[1],"0"))//Friendly
	{
		printf("Case1\n");
		begin = rdtsc();
		for(i = 0; i<SIZE; i++)
			for(j = 0; j<SIZE; j++)
				array[i][j] = i+j;
		end = rdtsc();
		printf("Clicks:%d\t average:%d\n",end-begin,(end-begin)/(1024*1024));
	}
	else
	{
		printf("Case2\n");
		begin = rdtsc();
		for(j = 0; j<SIZE; j++)
			for(i = 0; i<SIZE; i++)
				array[i][j] = i+j;
		end = rdtsc();
		printf("Clicks:%d\t average:%d\n",end-begin,(end-begin)/(1024*1024));
	}
	return 0;
}
