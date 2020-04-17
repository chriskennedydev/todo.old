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
void add(char *item);
void check_dir(void);
void menu(void);


int main(int argc, char *argv[])
{
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    const char *workdir = "/.todo";

    char filedir[80];

    strcpy(filedir, homedir);
    strcat(filedir, workdir);
    strcat(filedir, "/todo.dat");

    if(argc == 1)
    {
        printf("Need at least one argument.\n");
        printf("Use './todo help' for usage\n");
        printf("\n");
        exit(0);
    }

    
    int check_usage = strncmp(argv[1], "help", 4);
    int check_add = strncmp(argv[1], "add", 4);
    int check_list = strncmp(argv[1], "list", 4);
    int check_delete = strncmp(argv[1], "delete", 4);


    if(check_usage == 0)
    {
        help();
        exit(0);
    }

    if(check_add == 0)
    {
        check_dir();
        add(argv[2]);
    }

    if(check_delete == 0)
    {
        FILE *fptr;
        FILE *tmp_ptr;
        char *item;
        int i;
        int compared_item;


        fptr = fopen(filedir, "rb");
        tmp_ptr = fopen("/home/chris/.todo/temp.dat", "wb");
        if(fptr == NULL)
        {
            printf("Cannot open file.\n");
            return 1;
        }

        int lines = lines_in_file(filedir);

        for(i = 0; i < lines; i++)
        {
            item = malloc(250);
            if(fgets(item, 250, fptr) != NULL)
            {
                compared_item = atoi(argv[2]);
                if(compared_item != i + 1)
                    fprintf(tmp_ptr, "%s", item);
            }
            free(item);
        }

        fclose(fptr); 
        fclose(tmp_ptr);
        rename("/home/chris/.todo/temp.dat", filedir);
    }


    if(check_list == 0)
    {
        menu();
        FILE *fptr;
        char item[250];
        int i;

        fptr = fopen(filedir, "rb");
        if(fptr == NULL)
        {
            printf("Cannot open file.\n");
            return 1;
        }

        int lines = lines_in_file(filedir);

        for(i = 0; i < lines; i++)
        {
            if(fgets(item, 250, fptr) != NULL)
            {
                printf("%d %s", i + 1, item);
            }
        }

        fclose(fptr); 
    }
    return 0;
}

void help(void)
{
    printf("usage: ./todo cmd item | list\n");
    printf("cmd: add, delete\n");
    printf("item: Todo to do\n");
    printf("list: list current todos\n");
}

void add(char *item)
{
    FILE *output;
    char *file_name = "/home/chris/.todo/todo.dat";
    
    if((output = fopen(file_name, "ab")) == NULL)
    {
        printf("Can't open %s for reading.\n", file_name);
        exit(1);
    }
    fprintf(output, "%s\n", item);

    fclose(output);
}

int lines_in_file(char *file_name)
{
    FILE *fptr;
    int lines = 0;
    char chr;

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

void check_dir(void)
{
    DIR* todo = opendir("/home/chris/.todo");
    if(todo)
    {
        closedir(todo);
    }
    else if(ENOENT == errno) 
    {
        mkdir("/home/chris/.todo", 0775);
    }
    else 
    {
        printf("Could not create ~/.todo dir\n");
        exit(1);
    }
}

void menu(void)
{
    printf("Todo List\n");
    printf("----------\n");
}
