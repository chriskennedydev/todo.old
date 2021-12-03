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
void add(int todo_length, char **todo, char *filename);
void update(int todo_length, char **todo, char *filename, char *tempfile);
void del(char **todo, char *filename, char *tempfile);
void list(char *filename);
void check_dir(const char *homedir, const char *workdir);


int main(int argc, char **argv)
{
    const char *homedir = getenv("HOME");
    const char *workdir = "/.todo";

    char filedir[2048];
    char tmpfile[2048];

    strncpy(filedir, homedir, sizeof(filedir) - 1);
    strncat(filedir, workdir, sizeof(filedir) - 1);
    strncat(filedir, "/todo", sizeof(filedir) - 1);
    filedir[2047] = '\0';

    strncpy(tmpfile, homedir, sizeof(tmpfile) - 1);
    strncat(tmpfile, workdir, sizeof(tmpfile) - 1);
    strncat(tmpfile, "/temp", sizeof(tmpfile) - 1);
    tmpfile[2047] = '\0';

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

    else if(strncmp(argv[1], "update", 4) == 0)
    {
        update(argc, argv, filedir, tmpfile);
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

void add(int todo_length, char **todo, char *filename)
{
    FILE *output;
    int strsize = 0;
    for(int i = 1; i < todo_length; i++) 
    {
        strsize += strlen(todo[i]);
        if(todo_length > i + 1)
        strsize++;
    }

    char *item = malloc(strsize);

    if(item == NULL) 
    {
        perror("Error with malloc()");
        exit(1);
    }

    item[0] = '\0';


    for(int i = 2; i < todo_length; i++) 
    {
        strcat(item, todo[i]);
        if(todo_length > i + 1)
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

void update(int todo_length, char **todo, char *filename, char *tempfile)
{
    FILE *fptr = fopen(filename, "rb");
    FILE *tmp_ptr = fopen(tempfile, "wb");
    int cmp_item = atoi(todo[2]);
    char *item = malloc(2048);
    int strsize = 0;

    for(int i = 1; i < todo_length; i++) 
    {
        strsize += strlen(todo[i]);
        if(todo_length > i + 1)
        strsize++;
    }

    char *updated_todo = malloc(strsize);
    if(item == NULL) 
    {
        perror("Error with malloc()");
        exit(1);
    }

    updated_todo[0] = '\0';


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

    for(int i = 3; i < todo_length; i++) 
    {
        strcat(updated_todo, todo[i]);
        if(todo_length > i + 1)
            strcat(updated_todo, " ");
    }

    for(int i = 0; i < lines; i++) 
    {
        memset(item, 0, 2048);
        if(fgets(item, 2048, fptr) != NULL)
        {
            if(cmp_item == i + 1)
            {
                fprintf(tmp_ptr, "%s\n", updated_todo);
                continue;
            }
            fprintf(tmp_ptr, "%s", item);
        }
    }
    fclose(fptr); 
    fclose(tmp_ptr);
    rename(tempfile, filename);

    free(item);
    free(updated_todo);
}

void del(char **todo, char *filename, char *tempfile) 
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
                cmp_item = atoi(todo[2]);
                if(cmp_item != i + 1)
                fprintf(tmp_ptr, "%s", item);
        }
    }

    fclose(fptr); 
    fclose(tmp_ptr);
    rename(tempfile, filename);
    free(item);
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
    FILE *fptr = fopen(file_name, "rb");
    int lines = 0;
    char chr;
    if(fptr == NULL)
    {
        perror("Cannot open file ~/.todo/todo");
        exit(1);
    }


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
