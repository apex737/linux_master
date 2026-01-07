#include <stdio.h>

int write_file(void)
{
	FILE* fp = fopen("hi", "w");
	if(fp == NULL){
		perror("fopen failed");
		return -1;
	}
	fputs("Hello World\n", fp);
	fprintf(fp,"fpos is at %ld\n", ftell(fp));
	fclose(fp);
	return 0;
}

int read_file(void)
{
	FILE* fp = fopen("hi", "r+");
	if(fp == NULL){
		perror("fopen failed");
		return -1;
	}
	fputs("Hello World\n", fp);
	fprintf(fp,"fpos is at %ld\n", ftell(fp));
	fprintf(fp, "%ld\n", ftell(fp));
	printf("%d\n", fseek(fp, 12, SEEK_SET));
	fprintf(fp, "fseek() %d\n", 100);
	fclose(fp);
	return 0;

}
int main()
{
	if(write_file()){
		perror("failed write");
	}

	read_file();
	return 0;
}
