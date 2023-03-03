#include "fake_SR.h"
#include "ntlog.h"
#include <vector>
#include <future>
#include <iostream>

static int cancel_flag = 0;

static Callback g_callback;

typedef int (*pfunc)(char*, char*, int);

class n_thread
{
    std::string args;
    std::thread t;
    bool isRun = false;
public:
    n_thread()    {    }
    int run_thread(pfunc func, char *_in, char *_out, int timeout)
    {
        if (isRun)  return -69;
        isRun = true;

        args = _in;
        t = std::thread(func, const_cast<char*>(args.c_str()), _out, timeout);
        t.detach();
        return 0;
    }
    void End()
    {

        while (isRun)
            usleep(10);

    }
    void finish()
    {
        isRun = false;
    }
    void callback(std::string msg)
    {
        if (g_callback)
            g_callback(const_cast<char*>(msg.c_str()));
        pthread_cancel(t.native_handle());
    }
};

n_thread *g_nThread = nullptr;

int initDriver(Callback _callback)
{
    g_callback = _callback;

    g_nThread = new n_thread();

    return 0;
}

int deinit()
{
    if (g_nThread)
    {
        g_nThread->End();
        delete g_nThread;
        g_nThread = nullptr;
    }

    g_callback = nullptr;
    return 0;
}


int _fake_SR(char *in, char *out, int timeout)
{
    out = new char[64];
    LOG_setSourcePath(1);
    LOGGER;

    LogWritef("  IN: %s", in);

	cancel_flag = 0;	//执行cancel_flag初始化
    int n = strlen(in);

    std::vector<char> obj;

    for (int i = 0; i < n && cancel_flag != 1; i++)
    {
		if (cancel_flag == 1)
		{
			LogWritef("cancel_flag = %d, return -99", cancel_flag);
			return -99;
		}
        LogWritef("---%c", in[i]);
        obj.push_back(in[i]);
		usleep(1000000 * timeout / n);
    }
	
    for (int i = 0; i < n && obj.size() > 0; i++)
    {
        out[i] = obj[obj.size()-1];
        obj.pop_back();
    }

    LogWritef("  OUT: %s", out);

    g_nThread->callback(out);
    delete [] out;

    g_nThread->finish();
    return 0;
}


int fake_SR(char *in, char *out, int timeout)
{
    if (!g_nThread)
        return -96;
    int retVal = g_nThread->run_thread(_fake_SR, in, out, timeout);
    if (retVal != 0)
        printf("the thread is running\n");

    return retVal;
}


int cancel_process()
{
    LOGGER;
    cancel_flag = 1;
    LogWritef("cancel_flag = %d", cancel_flag);
    return 0;
}

