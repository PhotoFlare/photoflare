#ifndef TRANSPARENTDIALOG_H
#define TRANSPARENTDIALOG_H

/*

  Transparent dialog class.

*/

#include <QDialog>

namespace Ui {
class TransparentDialog;
}

class TransparentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransparentDialog(QWidget *parent = 0);
    ~TransparentDialog();

    void setColor(QRgb color);
    QColor color() {return m_color;}
    int tolerance();
private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void readSettings(QWidget* window);
    void writeSettings(QWidget* window);

signals:
    void previewTransparent(QColor,int);
    void dialogAccepted();
    void dialogRejected();
    void dialogFinished(int);

protected:
    void closeEvent (QCloseEvent *event);

private:
    Ui::TransparentDialog *ui;
    QColor m_color;
};

#endif // TRANSPARENTDIALOG_H
