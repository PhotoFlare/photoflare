#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*

  Main window class for the PhotoFlare application.

*/

#include <QMainWindow>
#include <QSignalMapper>
#include <QMdiArea>
#include <QProcess>
#include <QLabel>

#include "managers/ScanManager.h"
#include "dialogs/batchdialog.h"

namespace Ui {
class MainWindow;
}

class LayerManager;
class PaintWidget;
class PointerSettingsWidget;
class PaintBrushSettingsWidget;
class PaintBrushAdvSettingsWidget;
class SprayCanSettingsWidget;
class LineSettingsWidget;
class MagicWandSettingsWidget;
class StampSettingsWidget;
class BlurSettingsWidget;
class EraserSettingsWidget;
class SmudgeSettingsWidget;

class QComboBox;
class PaintWidget;
class TransparentDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    PaintWidget* getCurrentPaintWidget();
    void openFile(const QString& fileName);

public slots:
    void handleMessage(const QString& message);
    void on_image_filtered(QImage image);

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
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
    void on_toolButtonEraser_clicked();
    void on_toolButtonSmudge_clicked();

    void clearToolpalette();
    void on_actionQuit_triggered();
    void on_actionOil_Paint_triggered();
    void on_actionCharcoal_Drawing_triggered();
    void on_actionClose_triggered();
    void on_actionClose_all_triggered();
    void refreshTools();
    void on_actionAbout_triggered();
    void setWindowSize();
    void updateRecentFilesMenu();
    void showError(const QString &message);

    void onPointerToolSettingsChanged();
    void onPaintBrushSettingsChanged();
    void onPaintBrushAdvSettingsChanged();
    void onSprayCanSettingsChanged();
    void onLineSettingsChanged();
    void onMagicWandSettingsChanged();
    void onStampSettingsChanged();
    void onBlurSettingsChanged();
    void onEraserSettingsChanged();
    void onSmudgeSettingsChanged();

    void onPickPrimaryColor(const QPoint&);
    void onPickSecondaryColor(const QPoint&);
    void onFloodFillPrimaryColor(const QPoint&);
    void onFloodFillSecondaryColor(const QPoint&);
    void onSelectPrimaryColor(const QPoint&,int,bool);
    void onCrop(const QRect&);
    void onCopy();
    void onPaste();
    void on_actionPaste_as_new_image_triggered();
    void onZoomChanged(const QString&);
    void onSubWindowActivated(QMdiSubWindow*);
    void onEditText(const QString&,const QFont&, const QColor&);
    void on_TextTool_finished();
    void onMultiWindowModeChanged(bool);
    void on_actionSwirl_triggered();
    void on_actionGrayScale_triggered();
    void on_actionFlip_Vertical_triggered();
    void on_actionFlip_Horizontal_triggered();
    void on_actionRotate_CCW_triggered();
    void on_actionRotate_CW_triggered();
    void on_actionImage_Size_triggered();
    void on_actionFilterbar_triggered();
    void on_actionToolpalette_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionPrint_triggered();
    void on_actionAcquire_image_triggered();
    void onScanFnished(int,QProcess::ExitStatus);
    void onListFnished(int,QProcess::ExitStatus);
    void on_actionSelect_device_triggered();
    void on_actionScan_triggered();
    void on_actionFull_screen_triggered();
    void on_actionOriginal_size_triggered();
    void on_actionSolarize_triggered();
    void on_actionWave_triggered();
    void on_actionImplode_triggered();
    void on_actionSoften_triggered();
    void on_actionBlur_triggered();
    void on_actionSharpen_triggered();
    void on_actionReinforce_triggered();
    void on_actionImage_properties_triggered();
    void on_actionAutomate_Batch_triggered();
    void on_actionShow_selection_triggered(bool checked);
    void on_actionCanvas_Size_triggered();
    void on_actionRevert_triggered();
    void on_actionBrightplus_triggered();
    void on_actionBrightminus_triggered();
    void on_actionContrastplus_triggered();
    void on_actionContrastminus_triggered();
    void on_actionGammaCorrectplus_triggered();
    void on_actionGammaCorrectminus_triggered();
    void on_actionSaturationplus_triggered();
    void on_actionSaturationminus_triggered();
    void on_actionOldPhoto_triggered();
    void on_actionDustReduction_triggered();
    void on_actionOutside_frame_triggered();
    void on_actionEmboss_triggered();
    void on_actionGaussian_triggered();
    void on_actionImpulse_triggered();
    void on_actionLaplacian_triggered();
    void on_actionPoisson_triggered();
    void on_actionMonoChromatic_triggered();
    void on_actionEqualize_triggered();
    void on_actionCrop_To_Center_triggered();
    void on_actionAdd_Simple_Frame_triggered();
    void on_actionMotion_blur_triggered();
    void on_actionNormalize_triggered();
    void on_action3D_frame_triggered();
    void on_actionExplode_triggered();
    void on_actionDespeckle_triggered();
    void on_actionSepia_triggered();
    void on_actionAuto_levels_triggered();
    void on_actionAuto_contrast_triggered();
    void batchProcess_fileProcessFinished(QString file, QImage image);
    void batchProcess_batchProgress(int index,int total);
    void on_actionHue_variation_triggered();
    void onHuePreviewChanged(QImage image, bool colorize, QColor color, int degrees);
    void onPreviewTransparent(QColor color, int tolerance);
    void onTransparentFinished(int);
    void onTransparentAccepted();
    void onTransparentRejected();
    void on_actionGradient_triggered();
    void on_actionCrop_triggered();
    void onSelectionChanged(bool visible);
    void on_actionPaste_triggered();
    void on_actionCopy_triggered();
    void on_actionTransparent_colour_triggered();
    void on_actionIndexed_Mode_triggered();
    void on_actionRGB_Mode_triggered();
    void on_actionDonate_triggered();
    void on_actionAutomatic_Crop_triggered();

    void on_actionNegative_triggered();
    void on_actionBlack_and_white_triggered();
    bool fileExists(QString path);

    void on_actionOutside_drop_shadow_triggered();
    void on_actionOpacity_triggered();
    void on_actionPlugins_triggered();
    void on_actionCheck_for_updates_triggered();

    void on_show_hotspots_triggered();

    void on_actionSelect_all_triggered();

    void on_actionColour_Threshold_triggered();

    void on_actionNew_layer_triggered();
    void on_actionRemove_layer_triggered();

    void on_actionClearHistory_triggered();

    void on_actionClearClipboard_triggered();

    void on_actionDuplicate_triggered();

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject * obj, QEvent * e);

private:
    PaintWidget* createPaintWidget(const QString &imagePath) const;
    PaintWidget* createPaintWidget(const QSize &imageSize, const QColor &color) const;
    void addPaintWidget(PaintWidget *widget);
    void addTab(PaintWidget *widget);
    void addChildWindow(PaintWidget *widget);
    void on_actionAuto_zoom_triggered(PaintWidget *widget);
    void saveContent();
    bool saveImage(const QString &fileName, int quality);
    bool handleCloseChildWindow(QMdiSubWindow *subWindow);
    bool handleCloseTabs();
    void saveGeometryState();
    void createKeyboardShortcuts();
    void disableUnimplementedActions(bool hide);
    void updateStatusArea(int width, int height);
    void clearStatusArea();

    void applyThreadedFilter(QString filterName);

    Ui::MainWindow *ui;
    QString m_toolSelected;
    QString m_previousToolSelected;
    LayerManager *m_layerListWidget;
    PointerSettingsWidget *m_ptSettingsWidget;
    PaintBrushSettingsWidget *m_pbSettingsWidget;
    PaintBrushAdvSettingsWidget *m_pbAdvSettingsWidget;
    SprayCanSettingsWidget *m_scSettingsWidget;
    LineSettingsWidget *m_lineSettingsWidget;
    MagicWandSettingsWidget *m_magicWandSettingsWidget;
    StampSettingsWidget *m_stampSettingsWidget;
    BlurSettingsWidget *m_blurSettingsWidget;
    EraserSettingsWidget *m_eraserSettingsWidget;
    SmudgeSettingsWidget *m_smudgeSettingsWidget;
    QComboBox *zoomCombo;
    ScanManager *m_scanManager;
    QImage origImage;
    TransparentDialog *transparentDialog;
    QLabel *batchLbl;
    QLabel *imagesizeLbl;
};

#endif // MAINWINDOW_H
