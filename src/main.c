#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void help(void);
void add(char *item, char *time);
void get_time(time_t current_time);


int main(int argc, char *argv[])
{
    time_t current_time = time(NULL);
    printf("Current time, master AdminDev:\n");
    get_time(current_time);

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
    printf("usage: ./todo cmd item date time\n");
    printf("cmd: add, all, done, edit\n");
    printf("item: Todo to do\n");
    printf("date: \"today\", \"tomorrow\", \"4 char month two digit date\"\n");
    printf("time: \"noon\", \"3:30 PM\", \"4:45 AM\"\n");

}

void add(char *item, char *time)
{
    puts(item);
    puts(time);
}

void get_time(time_t current_time)
{
    current_time = time(NULL);
    if(current_time != (time_t)(-1))
        printf("%s\n", asctime(localtime(&current_time)));
}
