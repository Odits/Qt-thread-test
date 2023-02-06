#include "ntlog.h"

#define    Error_Succeed  0
#define    Error_FileOpen       -0x204          //文件打开失败

char _LOG_PATH_[64] = "./LOG.txt";

char ntlog_buf[5120];

char sourcePath[128];

using namespace std;

static int _npbp_checklogflag = 0;

static bool first = true;

int messLog(char * log_path, char const * _sourcefilename, size_t line, int level, int mode, char const * logdata, size_t logdatalen)
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
{
//    int pathLen = strlen(sourcePath);
//    char* sourcefilename = _sourcefilename;
//    if (pathLen != 0)   //有设置sourcePath的情况
//    {
//        sourcefilename = strstr(_sourcefilename, sourcePath);
//        if (sourcefilename != nullptr)  //设置的sourcePath正确
//        {
//            sourcefilename = _sourcefilename + pathLen - 3; //跳到sourcePath之后
////            strncpy(sourcefilename, "...", 3);
//        }
//        else
//        {
//            sourcefilename = _sourcefilename;
//        }
//    }
	char sourcefilename[256] = {0};
	strcpy(sourcefilename, _sourcefilename);

	int pathLen = int(strlen(sourcePath));
	if (pathLen != 0)   //有设置sourcePath的情况
	{
		if (strstr(_sourcefilename, sourcePath) != nullptr)  //设置的sourcePath正确
		{
			strcpy(sourcefilename, sourcefilename + pathLen - 3);//跳到sourcePath之后
			memset(sourcefilename, '.', 3);
		}
	}

	char tbuf[512] = {0};
	int iloop = 0;
	long log_size = 0;

	if (NO_LOG == _LOG_LEVEL_)
		return Error_Succeed;


	if (log_path == nullptr)
	{
		strcpy(tbuf, "./nt.log");
	}
	else
	{
		int path_len = int(strlen(log_path));
		if (log_path[path_len] == '/')					//log_path like "./log/" or "/tmp/"
		{
			for (iloop = 0; iloop < path_len; iloop++)
			{
				if (log_path[path_len - iloop] == '/')
					log_path[path_len - iloop] = 0;
			}
			sprintf(tbuf, "%s/nt.log", log_path);
		}
		else											//log_path like "./nt.log" or "./log/nt.txt"
		{
			strcpy(tbuf, log_path);
		}
	}

	FILE *fp = fopen(tbuf, "a+");
	if (fp == nullptr)
		return Error_FileOpen;

	if (_npbp_checklogflag == 0)
	{
		_npbp_checklogflag = 1;
		fseek(fp, 0L, SEEK_END);
		log_size = ftell(fp);    //判断日志大小，大于1M就删除重写
		if (log_size>0x100000)
		{
			fclose(fp);
			remove(tbuf);
			fp = fopen(tbuf, "a+");
			if (fp == nullptr)
				return Error_FileOpen;
		}
		fseek(fp, 0L, SEEK_SET);
	}

	time_t t;  //秒时间
	tm* local; //本地时间
	char buf[128] = { 0 };

	t = time(nullptr); //或者time(&t);//获取目前秒时间
	local = localtime(&t); //转为本地时间
	strftime(buf, 64, "%m-%d/%H:%M:%S", local);        //"%Y-%m-%d %H:%M:%S", local);
	// https://blog.csdn.net/tantion/article/details/86135961
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	sprintf(buf, "%s.%d", buf, tv.tv_usec);

	switch (_LOG_LEVEL_)
	{
	case LOG_INFO:   //所有级别的日志都记录
		if (level == LOG_INFO)
		{
			sprintf(tbuf, "time:%s file:%s  line:%d  INFO:", buf, sourcefilename, line);
		}
		else if (level == LOG_WARN)
		{
			sprintf(tbuf, "time:%s file:%s  line:%d  WARN:", buf, sourcefilename, line);
		}
		else
		{
			sprintf(tbuf, "time:%s file:%s  line:%d  ERROR:", buf, sourcefilename, line);
		}
		fwrite(tbuf, 1, strlen(tbuf), fp);
		if (mode == 0)
		{
			fwrite(logdata, 1, logdatalen, fp);
			fflush(fp);
		}
		else
		{
			for (iloop = 0; iloop<logdatalen; iloop++)
			{
				sprintf(tbuf, "%02X ", (unsigned char)logdata[iloop]);
				fwrite(tbuf, 1, sizeof(tbuf), fp);
			}
			fflush(fp);
		}
		break;
	case LOG_WARN:
		if (level == LOG_INFO)    //LOG_INFO 级日志不写
		{
		}
		else
		{
			if (level == LOG_WARN)
			{
				sprintf(tbuf, "time:%s file:%s  line:%d  WARN:", buf, sourcefilename, line);
			}
			else
			{
				sprintf(tbuf, "time:%s file:%s  line:%d  ERROR:", buf, sourcefilename, line);
			}
			fwrite(tbuf, 1, strlen(tbuf), fp);
			if (mode == 0)
			{
				fwrite(logdata, 1, logdatalen, fp);
				fflush(fp);
			}
			else
			{
				for (iloop = 0; iloop < logdatalen; iloop++)
				{
					sprintf(tbuf, "%02X ", (unsigned char)logdata[iloop]);
					fwrite(tbuf, 1, sizeof(tbuf), fp);
				}
				fflush(fp);
			}
		}
		break;
	case LOG_ERROR:
		if (level == LOG_ERROR)     //只有LOG_ERROR 级日志才写
		{
			sprintf(tbuf, "time:%s file:%s  line:%d  ERROR:", buf, sourcefilename, line);
			fwrite(tbuf, 1, strlen(tbuf), fp);
			if (mode == 0)
			{
				fwrite(logdata, 1, logdatalen, fp);
				fflush(fp);
			}
			else
			{
				for (iloop = 0; iloop<logdatalen; iloop++)
				{
					sprintf(tbuf, "%02X ", (unsigned char)logdata[iloop]);
					fwrite(tbuf, 1, sizeof(tbuf), fp);
				}
				fflush(fp);
			}
		}
		else
		{
		}
		break;
	}
	fwrite("\n", 1, sizeof("\n"), fp);
	fclose(fp);
	return Error_Succeed;
}


void _LogWriteLine(char const * msg, char* file, size_t line)
{
	sprintf(ntlog_buf, msg);
    messLog(_LOG_PATH_, file, line, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf));
}

void _LogWriteERROR(const char* msg, char* file, size_t line)
{
    sprintf(ntlog_buf, msg);
    messLog(_LOG_PATH_, file, line, LOG_ERROR, 0, ntlog_buf, strlen(ntlog_buf));
}

void _LogWriteTitleMsgStr(const char *str1, const char *str2, char* file, size_t line)
{
    sprintf(ntlog_buf, str1, str2);
    messLog(_LOG_PATH_, file, line, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf));
}

void _LogWriteTitleMsgInt(const char *str, int ret, char* file, size_t line)
{
	sprintf(ntlog_buf, str, ret);
	messLog(_LOG_PATH_, file, line, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf));
}

void _LogWriteTitleMsgBcd(char *title, unsigned char* msg, int msglen, char* file, size_t line)
{
    char log_bug[512] = {0};
    sprintf(ntlog_buf, "%s(HEX): ", title);
	if (msglen > 32)
        strcat(ntlog_buf, " \n ");
	for (int i = 0; i < msglen; ++i)
	{
        sprintf(log_bug, "%02X ", (unsigned char)msg[i] & 0xff);
        strcat(ntlog_buf, log_bug);
		int j = 0;
		if (++j == 32)
		{
            strcat(ntlog_buf, " \n ");
			j = 0;
		}
    }
	messLog(_LOG_PATH_, file, line, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf));
}

size_t strRfindN(const char* dest, char c, int n)
{
	size_t ret = strlen(dest);
	for (int i = 0; i < n; i++)
	{
		char tmp[128] = {0};
		strncpy(tmp, dest, ret);
		const char* get = strrchr(tmp, c);
        if (get == nullptr) return ret;
		ret -= strlen(get);
	}
	return ret;
}

void setSourcePath(int n)
{
	if (first)
	{
		char path[128] = __FILE__;

#ifdef __linux__
        char gap = '/';
#else
        char gap = '\\';
#endif
        size_t ret = strRfindN(path, gap, n);
		path[ret] = 0;

        sprintf(path, "%s%c...", path, gap);
		sprintf(ntlog_buf, "This is whole Path");
		messLog(_LOG_PATH_, path, 0, LOG_INFO, 0, ntlog_buf, strlen(ntlog_buf));

		path[ret] = 0;
		strcpy(sourcePath, path);
		first = false;
	}
}
