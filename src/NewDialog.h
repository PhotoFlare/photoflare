#ifndef NEWDIALOG_H
#define NEWDIALOG_H

/*

  Dialog for creating a new image.

*/

#include <QDialog>

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = 0);
    ~NewDialog();

    /*!
     * This method returns the size of new image that was chosen by
     * user. Usually it should be called after the dialog was
     * accepted, otherwise you will get an invalid size.
     */
    QSize newImageSize() const;
    void setImageSize(QSize size);
private slots:
    void on_buttonBox_accepted();

    void on_imagePresetCombo_currentIndexChanged(int index);

    void on_imageResCombo_currentIndexChanged(int index);

    void on_imageWHcombo_currentIndexChanged(int index);

    void on_imageRvalue_valueChanged(double arg1);

    void on_imageHvalue_valueChanged(double arg1);

    void on_imageWvalue_valueChanged(double arg1);

private:
    Ui::NewDialog *ui;

    QSize m_chosenSize;

    double imageRatio;
    float width_px;
    float height_px;
    int currentUnit;
};

#endif // NEWDIALOG_H
