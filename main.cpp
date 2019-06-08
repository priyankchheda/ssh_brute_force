#include <iostream>
#include <vector>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "sshBrute.h"
#include "utility.h"

int main(int argc, char* argv[])
{
    auto logger = spdlog::basic_logger_mt<spdlog::async_factory>(
            "logger", "sshbrute.log");
    spdlog::flush_every(std::chrono::seconds(2));
    logger->set_level(spdlog::level::debug);
    if (argc == 1) {
        std::cout << "Usage: " << argv[0] << " [options]\n"
            << "Try '" << argv[0] << " --help' for more information.\n";
        exit(1);
    }

    std::string user_filename = "";
    std::string password_filename = "";
    std::string hostname = "";
    int port = 22;

    for (int c = 1; c < argc; c++) {
        std::string options = std::string(argv[c]);
        if (options ==  "-uf" || options == "--userfile") {
            c = c + 1;
            user_filename = std::string(argv[c]);
        } else if (options == "-pf" || options == "--passfile") {
            c = c + 1;
            password_filename = std::string(argv[c]);
        } else if (options == "--host") {
            c = c + 1;
            hostname = std::string(argv[c]);
        } else if (options == "-p" || options == "--post") {
            c = c + 1;
            port = std::atoi(argv[c]);
        } else if (options == "-h" || options == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]\n\n"
                << "Required:\n"
                << " -uf, --userfile <filename>\tusername list\n"
                << " -pf, --passfile <filename>\tpassword list\n"
                << "      --host <hostname>\t\thostname\n\n"
                << "Optional:\n"
                << "  -p, --port <port>\t\tport number [default: 22]\n"
                << "  -h, --help\t\t\tPrint usage\n";
            exit(1);
        }
    }

    if (user_filename == "") {
        std::cerr << "Error: need username list for further processing\n";
        exit(1);
    } else if (password_filename == "") {
        std::cerr << "Error: need password list for further processing\n";
        exit(1);
    } else if (hostname == "") {
        std::cerr << "Error: need hostname for further processing\n";
        exit(1);
    }

    SSHBrute conn(hostname);
    std::vector<std::string> username = readFile(user_filename);
    std::vector<std::string> password = readFile(password_filename);
    for (int i = 0; i < username.size(); i++) {
        logger->info("checking for user: {}", username[i]);
        conn.setUser(username[i]);
        conn.connect(password);
    }
    return 0;
}
