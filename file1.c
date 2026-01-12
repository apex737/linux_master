#include <stdio.h>
#include <string.h>

const char* str = "fseek() 100\n";
int write_file(void)
{
	FILE* fp = fopen("file1", "w");
	if(fp == NULL){
		perror("fopen failed");
		return -1;
	}
	fputs("Hello World\n", fp);
	fprintf(fp,"fpos is at %ld\n", ftell(fp));
	fclose(fp);

    // zu: size_t, zd: ssize_t
    printf("%zu\n", strlen(str));
	return 0;
}

int read_file(void)
{
	FILE* fp = fopen("file1", "r+");
	if(fp == NULL){
		perror("fopen failed");
		return -1;
	}
    fseek(fp, 0, SEEK_END); // set fp to tail
	fputs("Hello World\n", fp);
    fseek(fp, -12, SEEK_END);

	fputs(str ,fp);
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
