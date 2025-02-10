#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startServer(quint16 port);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QList<QTcpSocket*> players;
    int player1Choice = -1;
    int player2Choice = -1;

    int determineWinner(int choice1, int choice2);
    void sendResult(QTcpSocket *socket, int winner);
};
#endif // SERVER_H
