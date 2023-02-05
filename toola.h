#ifndef TOOLA_H
#define TOOLA_H

#include <QObject>

class ToolA : public QObject
{
    Q_OBJECT
public:
    explicit ToolA(QObject *parent = nullptr);
    static void Funcoo(char *msg);//静态函数 将从此函数中发射出信号

    QString msg;
private:
    static ToolA *mytoola;//定义一个静态ToolA类

signals:
    void SigDeliverMess();//真正发出去的信号
    void SigDeliverMessStatic(char *msg);//内部信号 用于静态函数调用

private slots:
    void SlotDeliverMessStatic(char *msg);//内部槽 用于响应内部信号

};

#endif // TOOLA_H
