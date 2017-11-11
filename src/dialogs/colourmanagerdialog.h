#ifndef COLOURMANAGERDIALOG_H
#define COLOURMANAGERDIALOG_H

#include <QDialog>

namespace Ui {
class colourManagerDialog;
}

class colourManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit colourManagerDialog(QWidget *parent = 0);
    ~colourManagerDialog();

private slots:
    void on_addButton_clicked();

    void on_removeButton_clicked();

    QColor getColorFromLabel(QWidget *colorLabel) const;

    void addColourToPalette();

private:
    Ui::colourManagerDialog *ui;
};

#endif // COLOURMANAGERDIALOG_H
