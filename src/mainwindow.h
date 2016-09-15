/*

  Main window class for the PhotoFiltre LX application.

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class PaintWidget;
class PaintBrushSettingsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage getCurrentTabImage();

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
    void on_actionQuit_triggered();
    void on_actionOil_Paint_triggered();
    void on_actionCharcoal_Drawing_triggered();
    void on_actionClose_triggered();
    void on_actionClose_all_triggered();

    void showError(const QString &message);
    void onPaintBrushSettingsChanged();
    void onPickPrimaryColor(const QPoint&);
    void onPickSecondaryColor(const QPoint&);
    void onFloodFillPrimaryColor(const QPoint&);
    void onFloodFillSecondaryColor(const QPoint&);
    void onCrop(const QRect&);
    void onZoomChanged(const QString&);
    void onEditText(const QString&,const QFont&);

    void on_actionSwirl_triggered();

    void on_actionGrayScale_triggered();

    void on_actionFlip_Vertical_triggered();

    void on_actionFlip_Horizontal_triggered();

    void on_actionRotate_CCW_triggered();

    void on_actionRotate_CW_triggered();

    void on_actionImage_Size_triggered();

protected:
    void closeEvent(QCloseEvent *event);

private:
    PaintWidget* createPaintWidget(const QString &imagePath) const;
    PaintWidget* createPaintWidget(const QSize &imageSize) const;
    void addTab(PaintWidget *widget);
    void saveContent(int tabIndex);
    bool saveImage(int tabIndex, const QString &fileName);
    bool handleCloseTab(int index); // Returns true if cancelled.
    bool handleCloseTabs(); // Returns true if cancelled.
    void saveGeometryState();
    void disableUnimplementedActions();
    Ui::MainWindow *ui;
    QString m_toolSelected;
    PaintBrushSettingsWidget *m_pbSettingsWidget;
};

#endif // MAINWINDOW_H
