#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "fake_send&recv/fake_SR.h"

#include <pthread.h>

#include <QDebug>

MainWindow *MainWindow::st = nullptr;//静态类
QString MainWindow::msg;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	st = this;

	connect(this,&MainWindow::SigDeliverMessStatic,this,&MainWindow::Print_2_result);//关联内部信号与槽
}

MainWindow::~MainWindow()
{
    delete ui;
}


void* MainWindow::Thread1(void *)
{
    char in[32] = "123456789", out[32] = {0};

    fake_SR(in, out, 5);
    printf("out = %s\n", out);
	msg = out;
	emit st->SigDeliverMessStatic();//发射内部信号
    qDebug("Thread1 Done");

    return nullptr;
}



void MainWindow::on_pushButton_clicked()
{
    //创建 mythread1 线程，执行 Thread1() 函数
	int res = pthread_create(&mythread1, nullptr, Thread1, nullptr);
    if (res != 0)
    {
		qDebug("线程创建失败");
        return;
    }

	qDebug("主线程执行完毕");

    return;
}


void MainWindow::on_pushButton_2_clicked()
{
    cancel_process();
    ui->result->append("cancel ing");
}


void MainWindow::Print_2_result()
{
	qDebug("%s . st->msg = %s\n", __func__, st->msg.toStdString().c_str());
	//收到信号
	ui->result->append("msg = " + QString(st->msg));
}





