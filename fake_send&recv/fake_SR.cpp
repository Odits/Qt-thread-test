#include "fake_SR.h"
#include "ntlog.h"
#include <vector>

static int cancel_flag = 0;

int fake_SR(char *in, char *out, int timeout)
{
    setSourcePath(1);
    LOGGER;

    LogWritef("  IN: %s", in);

    int n = strlen(in);

    std::vector<char> obj;

    for (int i = 0; i < n && cancel_flag != 1; i++)
    {
        LogWritef("---%c", in[i]);
        obj.push_back(in[i]);
        usleep((timeout*1.0 / n) * 1000000);
    }
	
    for (int i = 0; i < n && obj.size() > 0; i++)
    {
        out[i] = obj[obj.size()-1];
        obj.pop_back();
    }

    LogWritef("  OUT: %s", out);

    return 0;
}


int cancel_process()
{
    LOGGER;
    cancel_flag = 1;
    LogWritef("cancel_flag = %d", cancel_flag);
    return 0;
}

