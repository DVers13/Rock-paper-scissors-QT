#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent){}

void Server::startServer()
{
    if (!this->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Could not start server";
    } else {
        qDebug() << "Listening to port 1234...";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    // Сохраняем сокеты игроков
    if (players.size() < 2) {
        players.append(socket);
        connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    }
    else
    {
        socket->deleteLater();
    }
}

void Server::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QDataStream in(socket);
    int choice;
    in >> choice;

    if (socket == players[0])
    {
        player1Choice = choice;
    }
    else if (socket == players[1])
    {
        player2Choice = choice;
    }

    // Если оба игрока сделали выбор, определяем победителя
    if (player1Choice != -1 && player2Choice != -1)
    {
        int winner = determineWinner(player1Choice, player2Choice);

        if (winner == 1)
        {
            sendResult(players[0], 1);
            sendResult(players[1], 2);
        }
        else if (winner == 0)
        {
            sendResult(players[0], 0);
            sendResult(players[1], 0);
        }
        else
        {
            sendResult(players[0], 2);
            sendResult(players[1], 1);
        }

        // Сбрасываем выборы для следующего раунда
        player1Choice = -1;
        player2Choice = -1;
    }
}

void Server::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    players.removeAll(socket);
    socket->deleteLater();

    qDebug() << "Client disconnected";
}

int Server::determineWinner(int choice1, int choice2)
{
    qDebug() << choice1 << choice2;
    if (choice1 == choice2) return 0; // Ничья

    if ((choice1 == 0 && choice2 == 2) || // Камень > Ножницы
        (choice1 == 1 && choice2 == 0) || // Бумага > Камень
        (choice1 == 2 && choice2 == 1))   // Ножницы > Бумага
    {
        return 1; // Игрок 1 выиграл
    }
    else
    {
        return 2; // Игрок 2 выиграл
    }
}

void Server::sendResult(QTcpSocket *socket, int winner)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << winner;

    socket->write(data);
}
