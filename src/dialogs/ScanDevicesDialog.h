#ifndef SCANDEVICESDIALOG_H
#define SCANDEVICESDIALOG_H

/*

  Scan devices dialog class.

*/

#include <QDialog>

namespace Ui {
class ScanDevicesDialog;
}

class ScanDevicesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScanDevicesDialog(QWidget *parent = 0);
    ~ScanDevicesDialog();

    void setSourceList(QStringList sourceList);
    int getCurrentSourceId(){ return mCurrentSourceId; }
private slots:

    void on_listWidget_currentRowChanged(int currentRow);

private:
    Ui::ScanDevicesDialog *ui;
    int mCurrentSourceId;
};

#endif // SCANDEVICESDIALOG_H
