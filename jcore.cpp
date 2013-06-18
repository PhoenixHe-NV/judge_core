#include "common.hpp"
#include "log.hpp"
#include "c_runner.hpp"
#include "pas_runner.hpp"

DEFINE_OPTIONAL_ARG(string, result, "/dev/stderr", "Judge result output file");
DEFINE_OPTIONAL_ARG(string, type, "c", "c/pas, syscall limit type");

int main(int argc, const char* argv[])
{
    cerr.sync_with_stdio(0);
    if (ParseArguments(argc, argv) < 0)
        return 1;
    runner_base* R;
    if (ARG_type.compare("c") == 0)
    {
        R = new c_runner();
    }
    else if (ARG_type.compare("pas") == 0)
    {
        R = new pas_runner();
    }
    else
    {
        LOG("type not supported yet");
        return 1;
    }
    auto res = R->run();
    LOG("----FINAL----");
    LOG("time:      "<< res->timeCost);
    LOG("memory:    "<< res->memoryCost);
    LOG("retCode:   "<< res->returnCode);
    LOG("result:    "<< res->result);
    auto resFile = fopen(ARG_result.c_str(), "w");
    fprintf(resFile, "%d %d %d %d %s", 
            res->result, res->returnCode, res->timeCost, res->memoryCost, res->commit);
    fclose(resFile);
    return 0;
}
