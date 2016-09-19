#ifndef SCANMANAGER_H
#define SCANMANAGER_H

#include <QObject>
#include <QProcess>

class ScanManager : public QObject
{
    Q_OBJECT
public:
    explicit ScanManager(QObject *parent = 0);
    ~ScanManager();

    void scan();
    void list();

    QStringList& getSourceList(){ return mSourceList; }
    void setCurrentSourceId(int sourceId){ mCurrentSourceId = sourceId; }

    enum Mode { LIST, SCAN };
signals:
    void listFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void scanFinished(int exitCode, QProcess::ExitStatus exitStatus);
private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess *mProc;
    Mode mMode;
    QStringList mSourceList;
    int mCurrentSourceId;
};

#endif // SCANMANAGER_H
