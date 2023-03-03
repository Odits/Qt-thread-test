#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static int callback(char *);

    static MainWindow *st;//定义一个静态类

signals:
    void SigDeliverMessStatic(QString);//内部信号 用于静态函数调用

private slots:
    void on_pushButton_clicked();

	void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

private slots:
    void Print_to_result(QString);

};

#endif // MAINWINDOW_H
