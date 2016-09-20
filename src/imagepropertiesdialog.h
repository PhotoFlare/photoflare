#ifndef IMAGEPROPERTIESDIALOG_H
#define IMAGEPROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
class imagePropertiesDialog;
}

class imagePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit imagePropertiesDialog(QWidget *parent = 0);
    ~imagePropertiesDialog();

private:
    Ui::imagePropertiesDialog *ui;
};

#endif // IMAGEPROPERTIESDIALOG_H
