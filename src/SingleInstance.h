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

#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>
#include <QString>
#include <QStringList>

class SingleInstancePrivate;

// Keeps a single running Photoflare per user. The first process becomes the
// primary (isPrimary() == true) and receives messages from later processes.
class SingleInstance : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SingleInstance)

public:
    explicit SingleInstance(const QString &id, QObject *parent = nullptr);
    ~SingleInstance() override;

    bool isPrimary() const;

    // Secondary instances only: forwards the messages to the primary.
    // Returns false if the primary could not be reached.
    bool sendMessages(const QStringList &messages);

signals:
    // Emitted whenever another launch contacts this instance, even without files.
    void activationRequested();
    void messageReceived(const QString &message);

private:
    SingleInstancePrivate *d;
};

#endif // SINGLEINSTANCE_H
