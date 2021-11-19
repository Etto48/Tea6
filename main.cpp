#include "main.h"

Server::Server* globalServer = nullptr;

void ctrl_c(int signum)
{
    globalServer->close();
}

int main(int argc, char* argv[], char* envp[])
{
    Log::event << Log::time() << " Tea6-"<< VERSION << "\n";
    Server::Server s{4823};
    globalServer = &s;
    signal(SIGINT,ctrl_c);  
    signal(SIGTERM,ctrl_c);    
    s.join();
}