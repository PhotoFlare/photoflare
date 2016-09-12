#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionText_triggered();

    void on_actionPreferences_triggered();

    void on_toolButtonPointer_clicked();

    void on_toolButtonDropper_clicked();

    void on_toolButtonWand_clicked();

    void on_toolButtonLine_clicked();

    void on_toolButtonPaintBucket_clicked();

    void on_toolButtonSprayCan_clicked();

    void on_toolButtonPaintBrush_clicked();

    void on_toolButtonPaintBrushAdv_clicked();

    void on_toolButtonStamp_clicked();

    void on_toolButtonBlur_clicked();

    void clearToolpalette();

private:
    Ui::MainWindow *ui;
    QString toolSelected;
};

#endif // MAINWINDOW_H
