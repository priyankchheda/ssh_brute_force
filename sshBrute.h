#ifndef SSHCONN_H
#define SSHCONN_H

#include <libssh/libssh.h>
#include <vector>
#include <string>

class SSHBrute
{
    ssh_session session = NULL;
    int port;
    std::string hostname;

public:
    SSHBrute(std::string _host, int _port=22): hostname(_host), port(_port) {};
    ~SSHBrute();

    void initSession();
    void setUser(std::string username);
    void connect(std::vector<std::string> password);
};

#endif
