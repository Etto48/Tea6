#include "main.h"

int main(int argc, char* argv[], char* envp[])
{
    Server::Server s{4823};
    s.join();
}