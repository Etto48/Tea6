#include "main.h"

Server::Server *globalServer = nullptr;
Peer::Peer *globalPeer = nullptr;

void ctrl_c(int signum)
{
    if(globalServer)
        globalServer->close();
    if(globalPeer)
        globalPeer->close();
}

int serverRole()
{
    Log::event << Log::time() << " Tea6-" << VERSION << "-Server\n";
    Server::Server s{INDEX_PORT};
    globalServer = &s;
    signal(SIGINT, ctrl_c);
    signal(SIGTERM, ctrl_c);
    return s.join() != nullptr;
}

int peerRole()
{
    Log::event << Log::time() << " Tea6-" << VERSION << "-Peer\n";
    std::string username;
    std::string password;
    std::string indexIp;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;
    std::cout << "Index IPv6: ";
    std::cin >> indexIp;
    //std::cout << Tools::ipv6ToString(Tools::stringToIpv6(indexIp)) << std::endl;
    Peer::Peer p{username, password, indexIp, INDEX_PORT, 4824};
    globalPeer = &p;
    signal(SIGINT, ctrl_c);
    signal(SIGTERM, ctrl_c);
    p.join();
    return 0;
}

int main(int argc, char *argv[], char *envp[])
{
    Parser::InputParser args{argc, argv};
    std::string role = "";
    if (!args.cmdOptionExists("--role"))
    {
        do
        {
            std::cout << "Role: ";
            std::cin >> role;
            if (role != "server" && role != "peer")
                std::cout << "Available roles: server, peer" << std::endl;
        } while (role != "server" && role != "peer");
    }
    else
        role = args.getCmdOption("--role");
    if (role == "server")
    {
        return serverRole();
    }
    else if (role == "peer")
    {
        return peerRole();
    }
    else
    {
        std::cout << "Syntax: tea6 [--role server|peer]" << std::endl;
        return 0;
    }
}