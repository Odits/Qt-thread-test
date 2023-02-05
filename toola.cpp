#include "toola.h"
#include <QDebug>
#include <unistd.h>

ToolA *ToolA::mytoola = nullptr;//静态ToolA类 mytoola
char str[64] = {0};

ToolA::ToolA(QObject *parent) : QObject(parent)
{
    mytoola=this;
    connect(this,&ToolA::SigDeliverMessStatic,this,&ToolA::SlotDeliverMessStatic);//关联内部信号与槽
}

void ToolA::Funcoo(char *msg)
{
    //静态函数需要发出信号
    printf("%s . msg = %s\n", __func__, msg);
    qDebug() << "ToolA sent DeliverMess";
    strcpy(str, msg);
    emit mytoola->SigDeliverMessStatic(str);//发射内部信号
}

void ToolA::SlotDeliverMessStatic(char *msg)
{
    printf("%s . msg = %s\n", __func__, msg);
    usleep(500);
    this->msg = str;
    printf("%s . str = %s\n", __func__, str);
    emit mytoola->SigDeliverMess();//发射出去信号
}
