#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include <pthread.h>
#include <QDebug>

#include "../fake_send&recv/fake_SR.h"

#include <thread>

MainWindow *MainWindow::st = nullptr;//静态类
QString MainWindow::msg;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	st = this;

	connect(this,&MainWindow::SigDeliverMessStatic,this,&MainWindow::Print_to_result);//关联内部信号与槽
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
    t1 = new std::thread(Thread1, nullptr);


	qDebug("主线程执行完毕");

    return;
}


void MainWindow::on_pushButton_2_clicked()
{
    cancel_process();
    t1->join();
    delete t1;
    ui->result->append("cancel ing");
}


void MainWindow::Print_to_result()
{
	qDebug("%s . st->msg = %s\n", __func__, st->msg.toStdString().c_str());
	//收到信号
	ui->pushButton->setFlat(false);
	ui->result->append("msg = " + QString(st->msg));
}





