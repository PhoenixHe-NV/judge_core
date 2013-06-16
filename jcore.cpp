#include "common.hpp"
#include "log.hpp"
#include "native_runner.hpp"

DEFINE_OPTIONAL_ARG(string, result, "/dev/stderr", "Judge result output file");

int main(int argc, const char* argv[])
{
    cerr.sync_with_stdio(0);
    if (ParseArguments(argc, argv) < 0)
        return 1;
    auto R = new native_runner();
    auto res = R->run();
    LOG("----FINAL----");
    LOG("time:      "<<res.timeCost);
    LOG("memory:    "<<res.memoryCost);
    LOG("retCode:   "<<res.returnCode);
    LOG("result:    "<<res.result);
    auto resFile = fopen(ARG_result.c_str(), "w");
    fprintf(resFile, "%d %d %d %d", 
            res.result, res.returnCode, res.timeCost, res.memoryCost);
    return 0;
}
