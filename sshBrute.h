#ifndef SSHCONN_H
#define SSHCONN_H

#include <libssh/libssh.h>
#include <vector>
#include <string>
using namespace std;

class SSHBrute
{
    ssh_session session = NULL;
    int port;
    string hostname;

public:
    SSHBrute(string _host, int _port=22): hostname(_host), port(_port) {};
    ~SSHBrute();

    void initSession();
    void setUser(string username);
    void connect(vector<string> password);
};

#endif
