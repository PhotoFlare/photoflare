/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "SingleInstance.h"

#include <QDir>
#include <QLocalServer>
#include <QLocalSocket>
#include <QLockFile>
#include <QElapsedTimer>
#include <QThread>

namespace {
    // Messages are newline separated, so a path containing a newline can't be sent.
    constexpr char kSeparator = '\n';
    constexpr int kConnectTimeoutMs = 5000;

    // Scopes the lock and socket to the current user on shared machines.
    QString userSuffix()
    {
        QString user = qEnvironmentVariable("USERNAME");
        if (user.isEmpty()) {
            user = qEnvironmentVariable("USER");
        }
        return user.isEmpty() ? QString() : QStringLiteral("-") + user;
    }
}

class SingleInstancePrivate
{
public:
    QString serverName;
    QLockFile *lockFile = nullptr;
    QLocalServer *server = nullptr;
};

SingleInstance::SingleInstance(const QString &id, QObject *parent)
    : QObject(parent), d(new SingleInstancePrivate)
{
    const QString name = id + userSuffix();
    d->serverName = name;
    d->lockFile = new QLockFile(QDir::tempPath() + QStringLiteral("/") + name + QStringLiteral(".lock"));
    d->lockFile->setStaleLockTime(0); // only a dead owner process makes the lock stale

    // The lock is what decides who is primary, so two simultaneous launches can't both win.
    if (!d->lockFile->tryLock(0)) {
        return;
    }

    // Safe to remove any socket left behind by a crashed primary: we hold the lock.
    QLocalServer::removeServer(d->serverName);
    d->server = new QLocalServer(this);
    if (!d->server->listen(d->serverName)) {
        delete d->server;
        d->server = nullptr;
        return;
    }

    connect(d->server, &QLocalServer::newConnection, this, [this]() {
        while (QLocalSocket *socket = d->server->nextPendingConnection()) {
            emit activationRequested();
            connect(socket, &QLocalSocket::readyRead, this, [this, socket]() {
                while (socket->canReadLine()) {
                    const QString message = QString::fromUtf8(socket->readLine()).trimmed();
                    if (!message.isEmpty()) {
                        emit messageReceived(message);
                    }
                }
            });
            connect(socket, &QLocalSocket::disconnected, socket, &QLocalSocket::deleteLater);
        }
    });
}

SingleInstance::~SingleInstance()
{
    if (d->lockFile) {
        d->lockFile->unlock();
        delete d->lockFile;
    }
    delete d;
}

bool SingleInstance::isPrimary() const
{
    return d->server != nullptr;
}

bool SingleInstance::sendMessages(const QStringList &messages)
{
    if (isPrimary()) {
        return false;
    }

    // The primary may still be starting up and not listening yet, so retry for a while.
    QLocalSocket socket;
    QElapsedTimer timer;
    timer.start();
    while (true) {
        socket.connectToServer(d->serverName, QIODevice::WriteOnly);
        if (socket.waitForConnected(500)) {
            break;
        }
        if (timer.elapsed() >= kConnectTimeoutMs) {
            return false;
        }
        QThread::msleep(100);
    }

    for (const QString &message : messages) {
        socket.write(message.toUtf8());
        socket.write(&kSeparator, 1);
    }
    const bool written = messages.isEmpty() || socket.waitForBytesWritten(kConnectTimeoutMs);
    socket.disconnectFromServer();
    if (socket.state() != QLocalSocket::UnconnectedState) {
        socket.waitForDisconnected(1000);
    }
    return written;
}
