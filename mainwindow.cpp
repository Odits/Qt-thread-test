#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "fake_send&recv/fake_SR.h"

#include <pthread.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_toola(new ToolA)//初始化ToolA对象
{
    ui->setupUi(this);
    connect(m_toola,&ToolA::SigDeliverMess,this,&MainWindow::SlotDeliverMess);//关联ToolA的信号和MainWindow的槽

    //调用ToolA中静态方法
//    m_toola->Funcoo();
}

MainWindow::~MainWindow()
{
    delete ui;
}


static QString info;

void* MainWindow::Thread1(void *)
{
    char in[32] = "123456789", out[32] = {0};

    fake_SR(in, out, 5);
    printf("out = %s\n", out);
    ToolA::Funcoo(out);
    qDebug("Thread1 Done");

    //    ui->result->append(QString("out = %1").arg(out));
    return nullptr;
}



void MainWindow::on_pushButton_clicked()
{
//    char in[32] = "abcdefg", out[32] = {0};

//    fake_SR(in, out, 10);

//    ui->result->append(QString("out = %1").arg(out));
//    ui->result->append("DONE");


    int res;
    //创建两个线程变量
//	pthread_t mythread1, mythread2;
    void* thread_result;
    //创建 mythread1 线程，执行 Thread1() 函数
    res = pthread_create(&mythread1, NULL, Thread1, NULL);
    if (res != 0)
    {
        printf("线程创建失败\n");
        return;
    }

    //阻塞主线程，直至 mythread1 线程执行结束，用 thread_result 指向接收到的返回值，阻塞状态才消除。
//	res = pthread_join(mythread1, nullptr);
    //输出线程执行完毕后返回的数据
//	printf("%s\n", (char*)thread_result);
    printf("主线程执行完毕\n");

    ui->result->setPlainText(info);

    ui->result->append("DONE");

    return;
}


void MainWindow::on_pushButton_2_clicked()
{
    cancel_process();
    ui->result->append("cancel ing");
}






void MainWindow::SlotDeliverMess()
{
    printf("%s . m_toola->msg = %s\n", __func__, m_toola->msg.toStdString().c_str());
    //收到ToolA发出的信号
    qDebug() << "MainWindow received DeliverMess";


    ui->result->append("msg = " + QString(m_toola->msg));
}





