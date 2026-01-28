#include "../../def.h"
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct _file {
	int fdr;
	int fdw;
} RWFILE;

int basic_copy(RWFILE* rw)
{
	char buf[1024];
	ssize_t readCnt;
	while((readCnt = read(rw->fdr, buf, sizeof(buf))) > 0)
	{
		if (write(rw->fdw, buf, readCnt) != readCnt){
			return -1;
		}
	}
	lseek(rw->fdr, 0, SEEK_SET);
	return 0;
}

void sendfile_copy(RWFILE* rw)
{
	struct stat sb;
	fstat(rw->fdr, &sb);
	sendfile(rw->fdw, rw->fdr, NULL, sb.st_size);
	lseek(rw->fdr, 0, SEEK_SET);
}

int mmap_copy(RWFILE* rw)
{
	// read, write size 싱크
	struct stat sb;
	fstat(rw->fdr, &sb);
	if(ftruncate(rw->fdw, sb.st_size) == -1){
		perror("ftruncate()");
		return -1;
	}
	char* rptr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, rw->fdr, 0);
	if(rptr == MAP_FAILED){
		perror("rptr map failed");
		return -1;
	}
	char* wptr = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, rw->fdw, 0);
	if(wptr == MAP_FAILED){
		perror("wptr map failed");
		return -1;
	}

	memcpy(wptr, rptr, sb.st_size);
	munmap(wptr, sb.st_size);
    munmap(rptr, sb.st_size);
	return 0;
}


int main()
{
	int fdw1 = open("dst1.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	int fdw2 = open("dst2.dat", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	// MAP_SHARED를 위해 O_RDWR 요구
	int fdw3 = open("dst3.dat", O_CREAT | O_TRUNC | O_RDWR, 0644);
	int fdr = open("src.dat", O_RDONLY);
	if(fdr == -1 || fdw1 == -1 || fdw2 == -1 || fdw3 == -1){
		printf("open(failed)\n");
		return -1;

	}
	RWFILE rw1 = {fdr, fdw1};
	RWFILE rw2 = {fdr, fdw2};
	RWFILE rw3 = {fdr, fdw3};
	basic_copy(&rw1); 
	sendfile_copy(&rw2);
	mmap_copy(&rw3);
	close(fdw1); close(fdw2); close(fdw3);
	close(fdr);

	return 0;
}
