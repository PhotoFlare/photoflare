#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H

#include <QDialog>

namespace Ui {
class PluginDialog;
}

class PluginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginDialog(QWidget *parent = 0);
    ~PluginDialog();

private:
    Ui::PluginDialog *ui;
};

#endif // PLUGINDIALOG_H
