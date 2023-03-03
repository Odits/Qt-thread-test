
extern "C"
{
//默认接口
typedef int(*Callback)(char* message);

int initDriver(Callback);

int deinit();


int fake_SR(char *in, char *out, int timeout);

int cancel_process();

}

