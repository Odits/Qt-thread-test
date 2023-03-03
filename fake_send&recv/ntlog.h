#if !defined  _NTLOG_H_
#define _NTLOG_H_

#include <string.h>
#include <string>
//#include <dlfcn.h>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

#pragma once

#define  NO_LOG     0
#define  LOG_INFO   1
#define  LOG_WARN   2
#define  LOG_ERROR  3

#define  _LOG_LEVEL_  LOG_INFO       //定义日志详细级别，INFO最详细，包括WARN和ERROR。WARN次详细，包括ERROR。ERROR最少，只包含ERROR类日志。NO_LOG为不打印日志

	extern char _LOG_PATH_[64];

	extern char ntlog_buf[5120];

    extern char sourcePath[128];

	int messLog(char * log_path, char const * _sourcefilename, size_t line, int level, int mode, char const * logdata, size_t logdatalen);
	/*
		打印日志,日志文件固定为ntbp8903log.txt,日志允许保存1M的文件，超出1M，日志将会被删除，重新新建文件记录新的日志。
		入参：
			log_path:日志保存路径.
			sourcefilename:源文件名,调用时直接传宏 __FILE__。
			line:打印日志的行，调用时直接传宏  __LINE__。
			level:宏定义级别，取值为LOG_INFO、LOG_WARN和LOG_ERROR。INFO最详细，包括WARN和ERROR。WARN次详细，包括ERROR。ERROR最少，只包含ERROR类日志。
			mode:打印方式，0 ---直接打印，1---每个字符按16进制格式逐一打印
			logdata：日志数据
			logdatalen:日志数据长度
		返回：0 成功，其他失败！
	*/

//#define LogWriteLine(msg)  _LogWriteLine(msg, __FILE__, __LINE__)
//    void _LogWriteLine(const char* msg, char* file, int line);

//#define LogWriteERROR(msg)  _LogWriteERROR(msg, __FILE__, __LINE__)
//    void _LogWriteERROR(const char* msg, char* file, int line);

//#define LogWriteTitleMsgStr(a, b) _LogWriteTitleMsgStr(a, b, __FILE__, __LINE__)
//    void _LogWriteTitleMsgStr(const char *str1, const char *str2, char* file, int line);

//#define LogWriteTitleMsgInt(a, b) _LogWriteTitleMsgInt(a, b, __FILE__, __LINE__)
//	void _LogWriteTitleMsgInt(const char *str, int ret, char* file, int line);

#define LogWriteTitleMsgBcd(a, b, c)  _LogWriteTitleMsgBcd(a, b, c, __FILE__, __LINE__)
	void _LogWriteTitleMsgBcd(char *title, unsigned char* msg, int msglen, char* file, int line);

//    extern char log_buf[512];
#define LogWritef(format, args...) \
	sprintf(ntlog_buf, format, args); \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf))

#define LogWriteERRORf(format, args...) \
	sprintf(ntlog_buf, format, args); \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_ERROR, 0, ntlog_buf, strlen(ntlog_buf))


#define LogWriteLine(msg) \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_INFO, 0, msg, strlen(msg))

#define LogWriteERROR(msg) \
	messLog(_LOG_PATH_, __FILE__, __LINE__, LOG_ERROR, 0, msg, strlen(msg))


#define LOGGER Logger log(__func__, __FILE__, __LINE__)

class Logger
{
    char fun_name[32] = {0};
    char path[256] = {0};
public:
    Logger(const char* msg, char const* file, size_t line)
	{
        strcpy(fun_name, msg);
        strcpy(path, file);
        sprintf(ntlog_buf, "--------------------%s--------------------IN", msg);
        messLog(_LOG_PATH_, file, line, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf));
    }
    ~Logger()
    {
        sprintf(ntlog_buf, "--------------------%s------------------------------OUT\n", fun_name);
        messLog(_LOG_PATH_, path, 0, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf));
    }
};

size_t strRfindN(const char* dest, char c, int n);

void LOG_setSourcePath(int n = 2);

#endif
