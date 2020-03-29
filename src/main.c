#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void help(void);
void add(char *item, char *time);


int main(int argc, char *argv[])
{

    if(argc == 1)
    {
        printf("Need at least one argument.\n");
        printf("Use './todo help' for usage\n");
        printf("\n");
        exit(0);
    }

    int check_usage = strncmp(argv[1], "help", 4);
    int check_add = strncmp(argv[1], "add", 4);

    if(check_usage == 0)
    {
        help();
        exit(0);
    }

    if(check_add == 0)
    {
        add(argv[2], argv[3]);
        
    }

    return 0;

}

void help(void)
{
    printf("usage: ./todo cmd item time fmt\n");
}

void add(char *item, char *time)
{
    puts(item);
    puts(time);
}
