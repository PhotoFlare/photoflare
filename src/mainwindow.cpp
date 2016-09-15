/*

  Main window class for the PhotoFiltre LX application.

*/

#include "mainwindow.h"
#include "NewDialog.h"
#include "textdialog.h"
#include "prefsdialog.h"
#include "aboutdialog.h"
#include "PaintWidget.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QSettings>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QMouseEvent>

#include "./tools/PaintBrushTool.h"
#include "./tools/ColourPickerTool.h"
#include "./tools/PaintBucketTool.h"
#include "./tools/PointerTool.h"
#include "./tools/TextTool.h"

#include "PaintBrushSettingsWidget.h"
#include "ToolManager.h"
#include "Settings.h"
#include "FilterManager.h"

#define PAINT_BRUSH ToolManager::instance()->paintBrush()
#define COLOUR_PICKER ToolManager::instance()->colourPicker()
#define PAINT_BUCKET ToolManager::instance()->paintBucket()
#define MOUSE_POINTER ToolManager::instance()->mousePointer()
#define TEXT_TOOL ToolManager::instance()->textTool()

namespace {
const QString UNTITLED_TAB_NAME = QObject::tr("Untitled");
// Temporary const variable.
const QString MODIFIED_UNTITLED_TAB_NAME = QObject::tr("Untitled *");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Center colorBox in tool palette.
    ui->verticalLayout->setAlignment(ui->colorBoxWidget, Qt::AlignCenter);

    // Disable actions that are not yet implemented.
    //disableUnimplementedActions();

    zoomCombo = new QComboBox;
    zoomCombo->setFocusPolicy( Qt::NoFocus );
    // FIXME: String below should probably be localized.
    QStringList list(QStringList() << "" << "10%" << "25%" << "33%" << "50%" << "66%" << "75%" << "100%" << "200%" << "300%" << "400%" << "500%" << "600%" << "800%");
    zoomCombo->addItems(list);
    zoomCombo->setEnabled(true);
    zoomCombo->insertSeparator(1);
    zoomCombo->insertSeparator(8);
    zoomCombo->insertSeparator(10);
    ui->mainToolBar->addWidget(zoomCombo);
    connect(zoomCombo, SIGNAL(activated(const QString&)), this, SLOT(onZoomChanged(const QString&)));
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    m_toolSelected = "paintBrush";
    ui->toolButtonPaintBrush->setChecked(true);
    m_pbSettingsWidget = new PaintBrushSettingsWidget;
    ui->dockWidgetContentsToolpalette->layout()->addWidget(m_pbSettingsWidget);
    connect(m_pbSettingsWidget, &PaintBrushSettingsWidget::settingsChanged, this, &MainWindow::onPaintBrushSettingsChanged);

    PAINT_BRUSH->setPrimaryColor(ui->colorBoxWidget->primaryColor());
    PAINT_BRUSH->setSecondaryColor(ui->colorBoxWidget->secondaryColor());
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::primaryColorChanged, PAINT_BRUSH, &PaintBrushTool::setPrimaryColor);
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::secondaryColorChanged, PAINT_BRUSH, &PaintBrushTool::setSecondaryColor);

    bool maximize = SETTINGS->isMaximizeWindow();
    if (maximize) {
        this->setWindowState(Qt::WindowMaximized);
    } else {
        QRect geometry = SETTINGS->customWindowGeometry();
        if (geometry.isValid())
            this->setGeometry(geometry);
    }

    QObject::connect(COLOUR_PICKER, SIGNAL(pickPrimaryColor(const QPoint&)), this, SLOT(onPickPrimaryColor(const QPoint&)));
    QObject::connect(COLOUR_PICKER, SIGNAL(pickSecondaryColor(const QPoint&)), this, SLOT(onPickSecondaryColor(const QPoint&)));

    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillPrimaryColor(const QPoint&)), this, SLOT(onFloodFillPrimaryColor(const QPoint&)));
    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillSecondaryColor(const QPoint&)), this, SLOT(onFloodFillSecondaryColor(const QPoint&)));

    QObject::connect(MOUSE_POINTER, SIGNAL(crop(const QRect&)), this, SLOT(onCrop(const QRect&)));

    QObject::connect(TEXT_TOOL, SIGNAL(editText(const QString&,const QFont&)), this, SLOT(onEditText(const QString&,const QFont&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    aboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionNew_triggered()
{
    NewDialog dialog;
    if (dialog.exec()) {
        addTab(createPaintWidget(dialog.newImageSize()));
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QString(), tr("Image Files (*.png *.jpg *.jpeg *.gif);;All Files (*)"));
    if (!fileName.isEmpty())
        addTab(createPaintWidget(fileName));
}

void MainWindow::on_actionSave_triggered()
{
    saveContent(ui->tabWidget->currentIndex());
}

void MainWindow::on_actionSave_As_triggered()
{
    // Load default filter by image file name.
    QString currentFileName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    QString suffix = QFileInfo(currentFileName).suffix();

    QStringList filters;
    filters << tr(".png (*.png)");
    filters << tr(".jpg (*.jpg *.jpeg)");
    filters << tr(".bmp (*.bmp)");
    filters << tr(".pbm (*.pbm)");
    filters << tr(".pgm (*.pgm)");
    filters << tr(".ppm (*.ppm)");
    filters << tr("All Files (*)");

    QString defaultFilter;
    if (!suffix.isEmpty()) {
        foreach (const QString &filter, filters) {
            if (filter.contains(suffix, Qt::CaseInsensitive)) {
                defaultFilter = filter;
                break;
            }
        }
    }
    if (defaultFilter.isEmpty())
        defaultFilter = filters.last();

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    QString(), filters.join(";;"), &defaultFilter);


    if (fileName.isEmpty())
        return;

    // WORKAROUND: Add the extension to the file name manually.
    QString fileNameSuffix = QFileInfo(fileName).suffix();

    if (fileNameSuffix.isEmpty()) {
        if (defaultFilter == filters.last()) {
            fileName += ".png";// NOTE: Default file extension.
        } else {
            QStringList list = defaultFilter.split(" (");
            if (list.count() == 2) {
                fileName += list.at(0);
            }
        }
    }

    if (saveImage(ui->tabWidget->currentIndex(), fileName)) {
        ui->tabWidget->tabBar()->tabButton(ui->tabWidget->currentIndex(), QTabBar::RightSide)->setWindowModified(false);
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), fileName);
    } else {
        showError(tr("Unable to save image."));
    }

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    handleCloseTab(index);
}

void MainWindow::on_actionText_triggered()
{
    textDialog dialog(this);
    if(dialog.exec())
    {
        clearToolpalette();
        m_toolSelected = "text";
        PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
        if (widget) {
            widget->setPaintTool(TEXT_TOOL);
            TEXT_TOOL->setText(dialog.text(), dialog.font());
        }
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    prefsDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionImage_Size_triggered()
{
    NewDialog dialog;
    if (dialog.exec()) {

        PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
        if (widget) {
            widget->setImage(widget->image().scaled(dialog.newImageSize()));
        }
    }
}

void MainWindow::clearToolpalette()
{
    ui->toolButtonPointer->setChecked(false);
    ui->toolButtonDropper->setChecked(false);
    ui->toolButtonWand->setChecked(false);
    ui->toolButtonLine->setChecked(false);
    ui->toolButtonPaintBucket->setChecked(false);
    ui->toolButtonSprayCan->setChecked(false);
    ui->toolButtonPaintBrush->setChecked(false);
    ui->toolButtonPaintBrushAdv->setChecked(false);
    ui->toolButtonStamp->setChecked(false);
    ui->toolButtonBlur->setChecked(false);
}

PaintWidget *MainWindow::createPaintWidget(const QString &imagePath) const
{
    return new PaintWidget(imagePath);
}

PaintWidget *MainWindow::createPaintWidget(const QSize &imageSize) const
{
    return new PaintWidget(imageSize);
}

void MainWindow::addTab(PaintWidget *widget)
{
    if (m_toolSelected == "paintBrush")
        widget->setPaintTool(PAINT_BRUSH);

    int tabIndex = ui->tabWidget->addTab(widget, widget->imagePath().isEmpty() ? UNTITLED_TAB_NAME : widget->imagePath());
    ui->tabWidget->setCurrentIndex(tabIndex);

    QWidget *closeButton = ui->tabWidget->tabBar()->tabButton(tabIndex, QTabBar::RightSide);
    // Remove our paint widget after tab closing.
    connect(closeButton, &QWidget::destroyed, widget, &PaintWidget::deleteLater);

    connect(widget, &PaintWidget::contentChanged, [closeButton, tabIndex, this] () {
        if (!closeButton->isWindowModified()) {
            closeButton->setWindowModified(true);
            QString modifiedText = ui->tabWidget->tabText(tabIndex) + " *";
            ui->tabWidget->setTabText(tabIndex, modifiedText);
        }
    });

    connect(widget, &PaintWidget::zoomChanged, [this] (float scale) {
        this->zoomCombo->setItemText(0, QString::number((int)(scale*100)).append("%"));
        this->zoomCombo->setCurrentIndex(0);
    });
    widget->autoScale();
}

void MainWindow::saveContent(int tabIndex)
{
    QString tabText = ui->tabWidget->tabText(tabIndex);

    if (tabText == UNTITLED_TAB_NAME || tabText == MODIFIED_UNTITLED_TAB_NAME) {
        return on_actionSave_As_triggered();
    } else {
        tabText.remove(" *");
        if (saveImage(tabIndex, tabText)) {
            ui->tabWidget->tabBar()->tabButton(tabIndex, QTabBar::RightSide)->setWindowModified(false);
            ui->tabWidget->setTabText(tabIndex, tabText);
        }
    }
}

bool MainWindow::saveImage(int tabIndex, const QString &fileName)
{
    if (fileName.isEmpty())
        return false;

    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->widget(tabIndex));
    return widget ? widget->image().save(fileName) : false;
}

bool MainWindow::handleCloseTab(int index)
{
    QWidget *tab = ui->tabWidget->tabBar()->tabButton(index, QTabBar::RightSide);
    if (!tab)
        return false;

    if (tab->isWindowModified()) {
        int buttonCode = QMessageBox::question(this, tr("Unsaved Changes"), tr("Do you want to save changes before leaving?"),
                                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

        if (buttonCode == QMessageBox::Cancel) {
            return true;
        } else if (buttonCode == QMessageBox::Yes) {
            saveContent(index);
        }
    }

    ui->tabWidget->removeTab(index);

    return false;
}

bool MainWindow::handleCloseTabs()
{
    for (int i = ui->tabWidget->count() - 1; i >=0; --i) {
        QWidget *tab = ui->tabWidget->tabBar()->tabButton(i, QTabBar::RightSide);
        if (tab->isWindowModified()) {
            ui->tabWidget->setCurrentIndex(i);
            if (handleCloseTab(i))
                return true;
        } else {
            ui->tabWidget->removeTab(i);
        }
    }

    return false;
}

void MainWindow::saveGeometryState()
{
    // Save maximized window state if user maximizes the window manually.
    if (this->isMaximized() && !SETTINGS->isMaximizeWindow()) {
        SETTINGS->setMaximizeWindow(true);
    } else if (!SETTINGS->isMaximizeWindow()) { // Save custom window geometry.
        SETTINGS->setCustomWindowGeometry(this->geometry());
    }
}

void MainWindow::on_toolButtonPointer_clicked()
{
    clearToolpalette();
    m_toolSelected = "pointer";
    ui->toolButtonPointer->setChecked(true);
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget)
        widget->setPaintTool(MOUSE_POINTER);
}

void MainWindow::on_toolButtonDropper_clicked()
{
    clearToolpalette();
    m_toolSelected = "dropper";
    ui->toolButtonDropper->setChecked(true);
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget)
        widget->setPaintTool(COLOUR_PICKER);
}

void MainWindow::on_toolButtonWand_clicked()
{
    clearToolpalette();
    m_toolSelected = "wand";
    ui->toolButtonWand->setChecked(true);
}

void MainWindow::on_toolButtonLine_clicked()
{
    clearToolpalette();
    m_toolSelected = "line";
    ui->toolButtonLine->setChecked(true);
}

void MainWindow::on_toolButtonPaintBucket_clicked()
{
    clearToolpalette();
    m_toolSelected = "paintBucket";
    ui->toolButtonPaintBucket->setChecked(true);
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget)
        widget->setPaintTool(PAINT_BUCKET);
}

void MainWindow::on_toolButtonSprayCan_clicked()
{
    clearToolpalette();
    m_toolSelected = "sprayCan";
    ui->toolButtonSprayCan->setChecked(true);
}

void MainWindow::on_toolButtonPaintBrush_clicked()
{
    clearToolpalette();
    m_toolSelected = "paintBrush";
    ui->toolButtonPaintBrush->setChecked(true);

    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget)
        widget->setPaintTool(PAINT_BRUSH);
}

void MainWindow::on_toolButtonPaintBrushAdv_clicked()
{
    clearToolpalette();
    m_toolSelected = "paintBrushAdv";
    ui->toolButtonPaintBrushAdv->setChecked(true);
}

void MainWindow::on_toolButtonStamp_clicked()
{
    clearToolpalette();
    m_toolSelected = "stamp";
    ui->toolButtonStamp->setChecked(true);
}

void MainWindow::on_toolButtonBlur_clicked()
{
    clearToolpalette();
    m_toolSelected = "blur";
    ui->toolButtonBlur->setChecked(true);
}

void MainWindow::on_actionQuit_triggered()
{
    if (!handleCloseTabs()) {
        saveGeometryState();
        qApp->quit();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!handleCloseTabs()) {
        saveGeometryState();
    } else {
        event->ignore();
    }
}

QImage MainWindow::getCurrentTabImage()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    return widget->image();
}

void MainWindow::on_actionGrayScale_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->grayscale(widget->image()));
    }
}

void MainWindow::on_actionFlip_Horizontal_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->flipHorz(widget->image()));
    }
}

void MainWindow::on_actionFlip_Vertical_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->flipVert(widget->image()));
    }
}

void MainWindow::on_actionRotate_CCW_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->rotateCCW(widget->image()));
    }
}

void MainWindow::on_actionRotate_CW_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->rotateCW(widget->image()));
    }
}

void MainWindow::on_actionOil_Paint_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->oilPaint(widget->image()));
    }
}

void MainWindow::on_actionCharcoal_Drawing_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->charcoal(widget->image()));
    }
}

void MainWindow::on_actionSwirl_triggered()
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->swirl(widget->image()));
    }
}

void MainWindow::on_actionClose_triggered()
{
    int index = ui->tabWidget->currentIndex();
    if (index != -1) {
        handleCloseTab(index);
    }
}

void MainWindow::on_actionClose_all_triggered()
{
    handleCloseTabs();
}

void MainWindow::showError(const QString &message)
{
    QMessageBox::critical(this, tr("Error"), message);
}

void MainWindow::onPaintBrushSettingsChanged()
{
    PAINT_BRUSH->setWidth(m_pbSettingsWidget->brushWidth());
    PAINT_BRUSH->setAntialiasing(m_pbSettingsWidget->antialiasing());
    PAINT_BRUSH->setCapStyle(m_pbSettingsWidget->brushCapStyle());
}

void MainWindow::onPickPrimaryColor(const QPoint& pos)
{
    const QImage& image = this->getCurrentTabImage();
    const QColor& color = image.pixel(pos);

    ui->colorBoxWidget->setPrimaryColor(color);
}

void MainWindow::onPickSecondaryColor(const QPoint& pos)
{
    const QImage& image = this->getCurrentTabImage();
    const QColor& color = image.pixel(pos);

    ui->colorBoxWidget->setSecondaryColor(color);
}

void MainWindow::onFloodFillPrimaryColor(const QPoint& pos)
{
    const QColor& color = ui->colorBoxWidget->primaryColor();
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->floodFill(widget->image(), pos, color));
    }
}

void MainWindow::onFloodFillSecondaryColor(const QPoint& pos)
{
    const QColor& color = ui->colorBoxWidget->secondaryColor();
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setImage(FilterManager::instance()->floodFill(widget->image(), pos, color));
    }
}

void MainWindow::onCrop(const QRect& rect)
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        QRect crop = widget->image().rect().intersected(rect);
        widget->setImage(widget->image().copy(crop));
    }
}

void MainWindow::onZoomChanged(const QString& scale)
{
    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        widget->setScale(scale);
    }
}

void MainWindow::onEditText(const QString& text,const QFont& font)
{
    textDialog dialog(this);
    dialog.editText(text, font);
    if(dialog.exec())
    {
        TEXT_TOOL->setText(dialog.text(), dialog.font());
    }
}

void MainWindow::onTabChanged(int index)
{
    Q_UNUSED(index)

    PaintWidget *widget = static_cast<PaintWidget *>(ui->tabWidget->currentWidget());
    if (widget) {
        QString scale = QString::number((int)(widget->getScale()*100)).append("%");
        int index = this->zoomCombo->findText(scale);
        if(index < 0) {
            this->zoomCombo->setItemText(0, scale);
            this->zoomCombo->setCurrentIndex(0);
        } else {
            this->zoomCombo->setCurrentIndex(index);
        }
    }
}

// This method disables actions that are not yet implemented.
// They still appear in menus, but are greyed out.
void MainWindow::disableUnimplementedActions()
{
    ui->actionAged_effect->setEnabled(false);
    ui->actionAntialiasing->setEnabled(false);
    //ui->actionArtistic->setEnabled(false);
    ui->actionAuto_contrast->setEnabled(false);
    ui->actionAuto_levels->setEnabled(false);
    ui->actionAuto_zoom->setEnabled(false);
    ui->actionAutomate_Batch->setEnabled(false);
    ui->actionAutomatic_Crop->setEnabled(false);
    ui->actionAutomatic_transparency->setEnabled(false);
    ui->actionBlur->setEnabled(false);
    ui->actionBounding_box->setEnabled(false);
    ui->actionBright_Contrast->setEnabled(false);
    ui->actionBrightminus->setEnabled(false);
    ui->actionBrightplus->setEnabled(false);
    ui->actionCanvas_Size->setEnabled(false);
    ui->actionCenter->setEnabled(false);
    ui->actionClear->setEnabled(false);
    ui->actionColour->setEnabled(false);
    ui->actionColour_balance->setEnabled(false);
    ui->actionContract->setEnabled(false);
    ui->actionContrastminus->setEnabled(false);
    ui->actionContrastplus->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCopy_shape->setEnabled(false);
    ui->actionCopyright->setEnabled(false);
    ui->actionCrop->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionDefine_pattern->setEnabled(false);
    //ui->actionDeform->setEnabled(false);
    ui->actionDithering->setEnabled(false);
    ui->actionDuotone->setEnabled(false);
    ui->actionDuplicate->setEnabled(false);
    ui->actionDustReduction->setEnabled(false);
    ui->actionEdges->setEnabled(false);
    ui->actionExpand->setEnabled(false);
    ui->actionExport_as_icon->setEnabled(false);
    ui->actionFade->setEnabled(false);
    ui->actionFill_with_pattern->setEnabled(false);
    ui->actionFilterbar->setEnabled(false);
    ui->actionFit_Image->setEnabled(false);
    ui->actionFit_ratio->setEnabled(false);
    ui->actionFlatten->setEnabled(false);
    //ui->actionFlip_Horizontal->setEnabled(false);
    //ui->actionFlip_Vertical->setEnabled(false);
    ui->actionFrame->setEnabled(false);
    ui->actionFull_screen->setEnabled(false);
    ui->actionGammaCorrectminus->setEnabled(false);
    ui->actionGammaCorrectplus->setEnabled(false);
    ui->actionGamma_correct->setEnabled(false);
    ui->actionGradient->setEnabled(false);
    //ui->actionGrayScale->setEnabled(false);
    ui->actionHue_Saturation->setEnabled(false);
    ui->actionHue_variation->setEnabled(false);
    ui->actionImage_Size->setEnabled(false);
    ui->actionImage_properties->setEnabled(false);
    ui->actionImport_twain->setEnabled(false);
    ui->actionIndexed_Mode->setEnabled(false);
    ui->actionInformation->setEnabled(false);
    ui->actionInvert->setEnabled(false);
    ui->actionLevels->setEnabled(false);
    ui->actionLoad_shape->setEnabled(false);
    ui->actionManual_settings->setEnabled(false);
    ui->actionMore_highlights->setEnabled(false);
    ui->actionMore_shadows->setEnabled(false);
    ui->actionNegative->setEnabled(false);
    ui->actionNoise->setEnabled(false);
    ui->actionOffset->setEnabled(false);
    ui->actionOldPhoto->setEnabled(false);
    ui->actionOptimized_Clipping->setEnabled(false);
    ui->actionOriginal_size->setEnabled(false);
    ui->actionOther->setEnabled(false);
    ui->actionOutside_drop_shadow->setEnabled(false);
    ui->actionOutside_frame->setEnabled(false);
    ui->actionOptions->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionPaste_and_text_bounding_box->setEnabled(false);
    ui->actionPaste_as_new_image->setEnabled(false);
    ui->actionPaste_shape->setEnabled(false);
    ui->actionPaste_special->setEnabled(false);
    ui->actionPosterize->setEnabled(false);
    ui->actionPrint->setEnabled(false);
    ui->actionPurge->setEnabled(false);
    ui->actionRGB_Mode->setEnabled(false);
    ui->actionRecent_files->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionReinforce->setEnabled(false);
    ui->actionRelief->setEnabled(false);
    ui->actionReplace_colour->setEnabled(false);
    ui->actionReplace_colour_range->setEnabled(false);
    ui->actionRevert->setEnabled(false);
    //ui->actionRotate_CCW->setEnabled(false);
    //ui->actionRotate_CW->setEnabled(false);
    ui->actionSaturationminus->setEnabled(false);
    ui->actionSaturationplus->setEnabled(false);
    ui->actionSave_shape->setEnabled(false);
    ui->actionScan->setEnabled(false);
    ui->actionSelect_all->setEnabled(false);
    ui->actionSet_shape->setEnabled(false);
    ui->actionSet_wallpaper->setEnabled(false);
    ui->actionSharpen->setEnabled(false);
    ui->actionShow_grid->setEnabled(false);
    ui->actionShow_selection->setEnabled(false);
    ui->actionSkew->setEnabled(false);
    ui->actionSnap_to_grid->setEnabled(false);
    ui->actionSoften->setEnabled(false);
    ui->actionStroke_and_fill->setEnabled(false);
    ui->actionStylize->setEnabled(false);
    ui->actionSwap_RGB_channel->setEnabled(false);
    ui->actionText->setEnabled(false);
    ui->actionTexture->setEnabled(false);
    ui->actionToolpalette->setEnabled(false);
    ui->actionTransform->setEnabled(false);
    ui->actionTransform_2->setEnabled(false);
    ui->actionTransparency_mask->setEnabled(false);
    ui->actionTransparent_colour->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionValidate->setEnabled(false);
    ui->actionVisual_effect->setEnabled(false);
    ui->actionZoom_in->setEnabled(false);
    ui->actionZoom_out->setEnabled(false);
}
