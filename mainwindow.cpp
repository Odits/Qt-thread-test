#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

//#include <pthread.h>
#include <thread>


#define USE_SO 1

#if USE_SO
#include "fake_send&recv/fake_SR.h"
#else
#include <dlfcn.h>

typedef int(*pfake_SR)(char*, char*, int);
static pfake_SR fake_SR = nullptr;

typedef int(*pcancel_process)();
static pcancel_process cancel_process = nullptr;
#endif


MainWindow *MainWindow::st = nullptr;//静态类

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	st = this;

    connect(this, &MainWindow::SigDeliverMessStatic, this, &MainWindow::Print_to_result);//关联内部信号与槽

#if !USE_SO

	void* hDll = nullptr;
	hDll = dlopen("./FSR.so", RTLD_LAZY);
	if(nullptr == hDll)
	{
		qDebug("dlopen %s\n", dlerror());
		exit(0);
	}

	fake_SR = pfake_SR(dlsym(hDll, "fake_SR"));
	if(nullptr == fake_SR)
	{
		dlclose(hDll);
		qDebug("%s\n", dlerror());
		exit(0);
	}

	cancel_process = pcancel_process(dlsym(hDll, "cancel_process"));
	if(nullptr == cancel_process)
	{
		dlclose(hDll);
		qDebug("%s\n", dlerror());
		exit(0);
	}
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}


void* MainWindow::Thread1(void *)
{
    char in[32] = "123456789", out[32] = {0};

	fake_SR(in, out, 2);
    printf("out = %s\n", out);
    emit st->SigDeliverMessStatic(QString(out));//发射内部信号
    qDebug("Thread1 Done");

    return nullptr;
}



void MainWindow::on_pushButton_clicked()
{
#if 0   //pthread
    //创建 mythread1 线程，执行 Thread1() 函数
	pthread_t mythread1;
	int res = pthread_create(&mythread1, nullptr, Thread1, nullptr);
    if (res != 0)
    {
		qDebug("线程创建失败");
        return;
    }
#endif
    t1 = new std::thread(Thread1, nullptr);

	qDebug("主线程执行完毕");

    return;
}


void MainWindow::on_pushButton_2_clicked()
{
    cancel_process();
    if (t1)
    {
        t1->join();
        delete t1;
        ui->result->append("cancel ing");
    }
}


void MainWindow::Print_to_result(QString msg)
{
    qDebug("%s>--msg = %s\n", __func__, msg.toStdString().c_str());
    //收到信号
    ui->result->append("msg = " + msg);
}





