#ifndef SSHCONN_H
#define SSHCONN_H

#include <libssh/libssh.h>
#include <vector>
#include <string>
#include "spdlog/spdlog.h"

class SSHBrute
{
    ssh_session session = NULL;
    int port;
    std::string hostname;
    std::shared_ptr<spdlog::logger> _logger;

public:
    SSHBrute(std::string _host, int _port=22);
    ~SSHBrute();

    void initSession();
    void setUser(std::string username);
    void connect(std::vector<std::string> password);
};

#endif
