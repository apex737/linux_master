#include <stdio.h>

struct person {
    char name[20];
    int age;
};

int write_to_file()
{
    FILE *fp = fopen("write_bin", "w"); 
    if(fp == NULL)
    {
        perror("fopen failed");
        return -1;
    }
    struct person kim = {
        .name = "kim",
        .age = 20
    };

    struct person lee = {
        .name = "lee",
        .age = 30
    }; 

   if(fwrite(&lee, sizeof(struct person), 1, fp) != 1)
   {
        goto err;
   }
   if(fwrite(&kim, sizeof(struct person), 1, fp) != 1)
   {
        goto err;
   }

    fclose(fp);
    return 0;

err:
    perror("fwrite failed");
    fclose(fp);
    return -1;
}

int read_file()
{
    FILE *fp = fopen("write_bin", "r"); 
    if(fp == NULL)
    {
        perror("fopen failed");
        return -1;
    }
    struct person persons[2]; 
    if(fread(persons, sizeof(struct person), 2, fp) != 2)
    {
        perror("fread failed");
        return -1;
    }

    fclose(fp);
	return 0;
}

int main()
{

    write_to_file();
    read_file();
    

	return 0;
}
