#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void help(void);
int lines_in_file(char *file_name);
void add(char *item);
void get_time(time_t current_time);
char *update_todo(char *item, char *old, char *replacement);

int main(int argc, char *argv[])
{
    //time_t current_time = time(NULL);
    //printf("Current time: ");
    //get_time(current_time);

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
        add(argv[2]);
        
    }

    if(check_delete == 0)
    {
        FILE *fptr;
        FILE *tmp_ptr;
        char *item;
        unsigned long i;
        unsigned long compared_item;


        fptr = fopen("todo.dat", "rb");
        tmp_ptr = fopen("temp.dat", "wb");
        if(fptr == NULL)
        {
            printf("Cannot open file.\n");
            return 1;
        }

        for(i = 0; i < sizeof(fptr); i++)
        {
            item = malloc(16);
            if(fgets(item, 16, fptr) != NULL)
            {
                compared_item = atoi(argv[2]);
                if(compared_item != i + 1)
                    fprintf(tmp_ptr, "%s", item);
            }
            free(item);
        }

        fclose(fptr); 
        fclose(tmp_ptr);
        rename("temp.dat", "todo.dat");
    }


    if(check_list == 0)
    {
        FILE *fptr;
        char item[250];
        unsigned long i;

        fptr = fopen("todo.dat", "rb");
        if(fptr == NULL)
        {
            printf("Cannot open file.\n");
            return 1;
        }

        for(i = 0; i < sizeof(fptr); i++)
        {
            if(fgets(item, 250, fptr) != NULL)
            {
                printf("%ld %s", i + 1, item);
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
    char *file_name = "todo.dat";
    
    if((output = fopen(file_name, "ab")) == NULL)
    {
        printf("Can't open %s for reading.\n", file_name);
        exit(1);
    }
    fprintf(output, "%s\n", item);

    fclose(output);
}

void get_time(time_t current_time)
{
    if(current_time != (time_t)(-1))
        printf("%s\n", asctime(localtime(&current_time)));
}

char *update_todo(char *item, char *old, char *replacement)
{
    char *result;
    int i, count = 0;
    int new_word_length = strlen(replacement);
    int old_word_length = strlen(old);

    for(i = 0; item[i] != '\0'; i++)
    {
        if(strstr(&item[i], old) == &item[i])
        {
            count++;
            i += old_word_length - 1;
        }
    }

    result = (char *)malloc(i + count * (new_word_length - old_word_length) + 1);

    i = 0;
    while(*item)
    {
        if(strstr(item, old) == item)
        {
            strcpy(&result[i], replacement);
            i += new_word_length;
            item += old_word_length;
        }
        else
            result[i++] = *item++;
    }
    result[i] = '\0';
    return result;
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
        if(chr == 'n')
            lines++;
        chr = getc(fptr);
    }
    fclose(fptr);
    return lines;
}
