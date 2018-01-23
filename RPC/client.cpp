#include <iostream>
#include <rpc/rpc.h>
#include <unistd.h>
#include "prog.h"

using namespace std;

int main(int argc, char *argv[])
{
    CLIENT *Client;
    char host[256];

    string cmd = "";
    for(int i=1; i<argc; i++) {
        cmd += argv[i];
        cmd += " ";
    }

    cmd += " 2>&1";

    if (argc < 2) {
        fprintf(stderr, "usage: ./client finger <args> \n");
        exit(1);
    }
    gethostname(host, 256);

    Client = clnt_create(host, COMMANDPROG, COMMANDVERS, "udp"); //Create a handle for Client
    
    if(Client == NULL)
    {
        cout << "Error: Client not formed" << endl;
        exit(-1);
    }

    char *c = (char *)cmd.c_str();

    String *print = execcommand_1(&c, Client);

    cout << print->String_val;

    return 0;
}
