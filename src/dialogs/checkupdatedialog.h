#ifndef CHECKUPDATEDIALOG_H
#define CHECKUPDATEDIALOG_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class checkupdateDialog;
}

class checkupdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit checkupdateDialog(QWidget *parent = 0);
    ~checkupdateDialog();

public slots:
    void replyFinished(QNetworkReply *reply);

signals:
    void finished();

private slots:
    void on_closeButton_clicked();

private:
    Ui::checkupdateDialog *ui;
};

#endif // CHECKUPDATEDIALOG_H
