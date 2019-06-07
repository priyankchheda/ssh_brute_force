#include <iostream>
#include <vector>
#include "sshBrute.h"
#include "utility.h"

int main()
{
    SSHBrute conn("localhost");
    std::vector<std::string> username = readFile("username.txt");
    std::vector<std::string> password = readFile("password.txt");
    for (int i = 0; i < username.size(); i++) {
        conn.setUser(username[i]);
        conn.connect(password);
    }
    return 0;
}
