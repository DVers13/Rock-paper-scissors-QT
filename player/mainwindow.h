#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnected();
    void onReadyRead();

    void on_rock_clicked();

    void on_paper_clicked();

    void on_scissors_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    void connectToServer(const QString &ip, quint16 port);
    void sendChoice(int choice);

};
#endif // MAINWINDOW_H
