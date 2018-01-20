#include <iostream>
#include <rpc/rpc.h>
#include "prog.h"

using namespace std;

String *execcommand_1_svc(char **cmd, struct svc_req *X)
{
    String *tosend;
    tosend = (String *)malloc(sizeof(String));

    string result = "";
    char buffer[128];

    FILE* pipe = popen(*cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);

    size_t sz = result.size() + 1;

    tosend->String_val = (char *)malloc(sizeof(char) * sz);
    strncpy(tosend->String_val, result.c_str(), sizeof(char) * sz);
    tosend->String_len = sz * sizeof(char);

    return tosend;
}
