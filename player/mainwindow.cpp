#include "mainwindow.h"
#include "ui_mainwindow.h"


QString STATUS_CHOISE = "QLabel { color : black; font-size: 25px;}";
QString STATUS_WAIT = "QLabel { color : blue; font-size: 25px;}";
QString STATUS_WIN = "QLabel { color : green; font-size: 25px;}";
QString STATUS_LOSE = "QLabel { color : red; font-size: 25px;}";
QString STATUS_DRAW = "QLabel { color : orange; font-size: 25px;}";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->status->setStyleSheet(STATUS_CHOISE);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);

    connectToServer("127.0.0.1", 1234);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onConnected()
{
    qDebug() << "Connected to server";
}

void MainWindow::onReadyRead()
{
    QDataStream in(socket);
    int winner;
    in >> winner;

    if (winner == 0)
    {
        qDebug() << "It's a draw!";
        ui->status->setStyleSheet(STATUS_DRAW);
        ui->status->setText("Ничья!");
    }
    else if (winner == 1)
    {
        qDebug() << "You win!";
        ui->status->setStyleSheet(STATUS_WIN);
        ui->status->setText("Победа!");
    }
    else if (winner == 2)
    {
        qDebug() << "You lose!";
        ui->status->setStyleSheet(STATUS_LOSE);
        ui->status->setText("Поражение =(");
    }
}

void MainWindow::connectToServer(const QString &ip, quint16 port)
{
    socket->connectToHost(ip, port);
}

void MainWindow::sendChoice(int choice)
{
    ui->status->setStyleSheet(STATUS_WAIT);
    ui->status->setText("Ожидайте хода апонента");
    if (socket->state() == QTcpSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << choice;

        socket->write(data);
    }
}

void MainWindow::on_rock_clicked()
{
    sendChoice(0);
}


void MainWindow::on_paper_clicked()
{
    sendChoice(1);
}


void MainWindow::on_scissors_clicked()
{
    sendChoice(2);
}

