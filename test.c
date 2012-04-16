#include <stdio.h>
#include <stdlib.h>


int main(int aegc,char **argv){
	FILE *memory;
	char temp[10000];
	if((memory=fopen("/proc/memory","r")))
	{
			fgets(temp,10000,memory);
			printf("%s\n",temp);
		fclose(memory);
		return 0;
	}
	printf("fail\n");
	return 0;
}
