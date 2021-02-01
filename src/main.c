#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h> 
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>

void help(void);
int lines_in_file(char *file_name);
void examples(void);
void add(int argc, char **argv, char *filename);
void del(char **argv, char *filename, char *tempfile);
void list(char *filename);
void check_dir(const char *homedir, const char *workdir);


int main(int argc, char **argv)
{
    const char *homedir = getenv("HOME");
    const char *workdir = "/.todo";

    char filedir[2048];
    char tmpfile[2048];

    strcpy(filedir, homedir);
    strcat(filedir, workdir);
    strcat(filedir, "/todo.dat");

    strcpy(tmpfile, homedir);
    strcat(tmpfile, workdir);
    strcat(tmpfile, "/temp.dat");

    if(argc == 1)
    {
        printf("Need at least one argument.\n");
        help();
        exit(0);
    }

    check_dir(homedir, workdir);

    if(strncmp(argv[1], "help", 4) == 0)
    {
        help();
    }

    else if(strncmp(argv[1], "add", 4) == 0)
    {
        add(argc, argv, filedir);
    }

    else if(strncmp(argv[1], "del", 4) == 0)
    {
        del(argv, filedir, tmpfile);
    }

    else if(strncmp(argv[1], "list", 4) == 0)
    {
        list(filedir);
    }

    else if(strncmp(argv[1], "examples", 4) == 0)
    {
        examples();
    }

    else
    {
        help();
    }

    return 0;
}

void help(void)
{
    printf("usage: todo cmd arg\n");
    printf("cmd: add | del | list\n");
    printf("add -- arg: todo to do\n");
    printf("del -- arg: number of todo to delete\n");
    printf("list: list current todos\n");
    printf("for examples use todo examples\n");
}

void examples(void)
{
    printf("add a todo:\n");
    printf("todo add refactor app\n");
    printf("-----\n");
    printf("delete a todo:\n");
    printf("todo del 2\n");
    printf("-----\n");
    printf("list todos:\n");
    printf("todo list\n");
}

void add(int argc, char **argv, char *filename)
{
    FILE *output;
    int strsize = 0;
    for(int i = 1; i < argc; i++) 
    {
        strsize += strlen(argv[i]);
        if(argc > i + 1)
        strsize++;
    }

    char *item = malloc(strsize);

    if(item == NULL) 
    {
        perror("Error with malloc()");
        exit(1);
    }

    item[0] = '\0';


    for(int i = 2; i < argc; i++) 
    {
        strcat(item, argv[i]);
        if(argc > i + 1)
        strcat(item, " ");
    }

    if((output = fopen(filename, "ab")) == NULL)
    {
        perror("Can't open file for reading.");
        exit(1);
    }

    fprintf(output, "%s\n", item);

    fclose(output);
    free(item);
}

void del(char **argv, char *filename, char *tempfile) 
{
	    FILE *fptr = fopen(filename, "rb");
	    FILE *tmp_ptr = fopen(tempfile, "wb");
        int cmp_item; 
        char *item = malloc(2048);

        if(fptr == NULL)
        {
            perror("Cannot open file.");
            exit(1);
        }
        
        if(tmp_ptr == NULL)
        {
            perror("Cannot open file.");
            exit(1);
        }

        int lines = lines_in_file(filename);

        for(int i = 0; i < lines; i++)
        {
            memset(item, 0, 2048);
            if(fgets(item, 2048, fptr) != NULL)
            {
                    cmp_item = atoi(argv[2]);
                    if(cmp_item != i + 1)
                    fprintf(tmp_ptr, "%s", item);
            }
        }

        fclose(fptr); 
        fclose(tmp_ptr);
        rename(tempfile, filename);
}

void list(char *filename)
{
        char *item = malloc(2048);
        if(item == NULL) 
        {
            perror("Error with malloc()");
            exit(1);
        }

        FILE *fptr = fopen(filename, "rb");
        if(fptr == NULL)
        {
            perror("Cannot open file ~/.todo/todo");
            exit(1);
        }

        int lines = lines_in_file(filename);

        printf("Todo List\n");
        printf("----------\n");

        for(int i = 0; i < lines; i++) 
        {
            if(fgets(item, 2048, fptr) != NULL)
                printf("%d %s", i + 1, item);
            else
                perror("Shenanigans");

        }
        fclose(fptr);        
        free(item);
}

int lines_in_file(char *file_name)
{
        FILE *fptr;
        int lines = 0;
        char chr;
        if(fptr == NULL)
        {
            perror("Cannot open file ~/.todo/todo");
            exit(1);
        }

        fptr = fopen(file_name, "rb");

        chr = getc(fptr);
        while(chr != EOF)
        {
            if(chr == '\n')
                    lines++;
            chr = getc(fptr);
        }
        fclose(fptr);
        return lines;
}

void check_dir(const char *homedir, const char *workdir)
{
        char *filedir = malloc(2048);
        strcpy(filedir, homedir);
        strcat(filedir, workdir);
        DIR *todo = opendir(filedir);

        if(todo)
        {
            closedir(todo);
        }
        else if(ENOENT == errno) 
        {
            mkdir(filedir, 0775);
        }
        else 
        {
            perror("Could not create ~/.todo dir");
            exit(1);
        }

        free(filedir);
}
