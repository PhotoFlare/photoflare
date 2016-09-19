/*

  Main window class for the PhotoFiltre LX application.

*/

#include "mainwindow.h"
#include "NewDialog.h"
#include "textdialog.h"
#include "prefsdialog.h"
#include "ScanDevicesDialog.h"
#include "aboutdialog.h"
#include "registerdialog.h"
#include "PaintWidget.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QClipboard>
#include <QComboBox>
#include <QSettings>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMdiSubWindow>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QShortcut>

#include "./tools/PaintBrushTool.h"
#include "./tools/PaintBrushAdvTool.h"
#include "./tools/ColourPickerTool.h"
#include "./tools/PaintBucketTool.h"
#include "./tools/PointerTool.h"
#include "./tools/TextTool.h"
#include "./tools/SprayCanTool.h"
#include "./tools/LineTool.h"

#include "PaintBrushSettingsWidget.h"
#include "PaintBrushAdvSettingsWidget.h"
#include "SprayCanSettingsWidget.h"
#include "LineSettingsWidget.h"
#include "ToolManager.h"
#include "Settings.h"
#include "FilterManager.h"

#define PAINT_BRUSH ToolManager::instance()->paintBrush()
#define PAINT_BRUSH_ADV ToolManager::instance()->paintBrushAdv()
#define COLOUR_PICKER ToolManager::instance()->colourPicker()
#define PAINT_BUCKET ToolManager::instance()->paintBucket()
#define MOUSE_POINTER ToolManager::instance()->mousePointer()
#define TEXT_TOOL ToolManager::instance()->textTool()
#define SPRAY_CAN ToolManager::instance()->sprayCanTool()
#define LINE_TOOL ToolManager::instance()->lineTool()

namespace {
const QString UNTITLED_TAB_NAME = QObject::tr("Untitled");
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(SETTINGS->isMultiWindowMode())
    {
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
    } else
    {
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    }

    // Center colorBox in tool palette.
    ui->verticalLayout->setAlignment(ui->colorBoxWidget, Qt::AlignCenter);

    // Disable actions that are not yet implemented.
    disableUnimplementedActions();
    createKeyboardShortcuts();

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
    connect(ui->mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::onSubWindowActivated);

    m_pbSettingsWidget = new PaintBrushSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_pbSettingsWidget);
    connect(m_pbSettingsWidget, &PaintBrushSettingsWidget::settingsChanged, this, &MainWindow::onPaintBrushSettingsChanged);

    m_pbAdvSettingsWidget = new PaintBrushAdvSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_pbAdvSettingsWidget);
    connect(m_pbAdvSettingsWidget, &PaintBrushAdvSettingsWidget::settingsChanged, this, &MainWindow::onPaintBrushAdvSettingsChanged);

    m_scSettingsWidget = new SprayCanSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_scSettingsWidget);
    connect(m_scSettingsWidget, &SprayCanSettingsWidget::settingsChanged, this, &MainWindow::onSprayCanSettingsChanged);

    m_lineSettingsWidget = new LineSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_lineSettingsWidget);
    connect(m_lineSettingsWidget, &LineSettingsWidget::settingsChanged, this, &MainWindow::onLineSettingsChanged);

    on_toolButtonPaintBrush_clicked();

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    PAINT_BRUSH->setPrimaryColor(ui->colorBoxWidget->primaryColor());
    PAINT_BRUSH->setSecondaryColor(ui->colorBoxWidget->secondaryColor());
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::primaryColorChanged, PAINT_BRUSH, &PaintBrushTool::setPrimaryColor);
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::secondaryColorChanged, PAINT_BRUSH, &PaintBrushTool::setSecondaryColor);

    PAINT_BRUSH_ADV->setPrimaryColor(ui->colorBoxWidget->primaryColor());
    PAINT_BRUSH_ADV->setSecondaryColor(ui->colorBoxWidget->secondaryColor());
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::primaryColorChanged, PAINT_BRUSH_ADV, &PaintBrushAdvTool::setPrimaryColor);
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::secondaryColorChanged, PAINT_BRUSH_ADV, &PaintBrushAdvTool::setSecondaryColor);

    SPRAY_CAN->setPrimaryColor(ui->colorBoxWidget->primaryColor());
    SPRAY_CAN->setSecondaryColor(ui->colorBoxWidget->secondaryColor());
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::primaryColorChanged, SPRAY_CAN, &SprayCanTool::setPrimaryColor);
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::secondaryColorChanged, SPRAY_CAN, &SprayCanTool::setSecondaryColor);

    LINE_TOOL->setPrimaryColor(ui->colorBoxWidget->primaryColor());
    LINE_TOOL->setSecondaryColor(ui->colorBoxWidget->secondaryColor());
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::primaryColorChanged, LINE_TOOL, &LineTool::setPrimaryColor);
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::secondaryColorChanged, LINE_TOOL, &LineTool::setSecondaryColor);

    bool maximize = SETTINGS->isMaximizeWindow();
    if (maximize) {
        this->setWindowState(Qt::WindowMaximized);
    } else {
        QRect geometry = SETTINGS->customWindowGeometry();
        if (geometry.isValid())
            this->setGeometry(geometry);
    }

    updateRecents();

    QObject::connect(COLOUR_PICKER, SIGNAL(pickPrimaryColor(const QPoint&)), this, SLOT(onPickPrimaryColor(const QPoint&)));
    QObject::connect(COLOUR_PICKER, SIGNAL(pickSecondaryColor(const QPoint&)), this, SLOT(onPickSecondaryColor(const QPoint&)));

    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillPrimaryColor(const QPoint&)), this, SLOT(onFloodFillPrimaryColor(const QPoint&)));
    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillSecondaryColor(const QPoint&)), this, SLOT(onFloodFillSecondaryColor(const QPoint&)));

    QObject::connect(MOUSE_POINTER, SIGNAL(crop(const QRect&)), this, SLOT(onCrop(const QRect&)));
    QObject::connect(MOUSE_POINTER, SIGNAL(copy()), this, SLOT(onCopy()));
    QObject::connect(MOUSE_POINTER, SIGNAL(paste()), this, SLOT(onPaste()));

    QObject::connect(TEXT_TOOL, SIGNAL(editText(const QString&,const QFont&)), this, SLOT(onEditText(const QString&,const QFont&)));

    QObject::connect(SETTINGS, SIGNAL(multiWindowModeChanged(bool)), this, SLOT(onMultiWindowModeChanged(bool)));

    m_scanManager = new ScanManager();
    QObject::connect(m_scanManager, SIGNAL(listFinished(int,QProcess::ExitStatus)), this, SLOT(onListFnished(int,QProcess::ExitStatus)));
    QObject::connect(m_scanManager, SIGNAL(scanFinished(int,QProcess::ExitStatus)), this, SLOT(onScanFnished(int,QProcess::ExitStatus)));
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
        addPaintWidget(createPaintWidget(dialog.newImageSize()));
    }
}

void MainWindow::on_actionOpen_triggered()
{
    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QString(), tr("Image Files (*.png *.jpg *.jpeg *.gif);;All Files (*)"));
    openFile(fileName);
}

void MainWindow::on_actionSave_triggered()
{
    saveContent();
}

void MainWindow::on_actionSave_As_triggered()
{
    // Load default filter by image file name.
    QString currentFileName;
    currentFileName = ui->mdiArea->currentSubWindow()->windowTitle();

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

    if (saveImage(fileName)) {
        ui->mdiArea->currentSubWindow()->setWindowModified(false);
        ui->mdiArea->currentSubWindow()->setWindowTitle(fileName + " [*]");
        SETTINGS->addRecentFile(fileName);
        updateRecents();
    } else {
        showError(tr("Unable to save image."));
    }
}

void MainWindow::on_actionText_triggered()
{
    textDialog dialog(this);
    if(dialog.exec())
    {
        clearToolpalette();
        m_toolSelected = "text";
        PaintWidget *widget = getCurrentPaintWidget();
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
    PaintWidget *widget = getCurrentPaintWidget();
    if (!widget)
        return;

    NewDialog dialog;
    dialog.setWindowTitle("Resize Image");
    dialog.setImageSize(widget->image().size());
    if (dialog.exec()) {
        widget->setImage(widget->image().scaled(dialog.newImageSize()));
    }
}

void MainWindow::openFile(const QString& fileName)
{
    if (!fileName.isEmpty()) {
        addPaintWidget(createPaintWidget(fileName));
        SETTINGS->addRecentFile(fileName);
        updateRecents();
    }
}

void MainWindow::updateRecents()
{
    ui->menuRecent_Files->clear();

    QList<QVariant> recentFiles = SETTINGS->getRecentFiles();
    QList<QVariant>::iterator i;
    for(i = recentFiles.begin(); i != recentFiles.end(); i++) {
        const QString& fileName = (*i).toString();
        QAction* action = ui->menuRecent_Files->addAction(fileName);
        connect(action, &QAction::triggered, [this, fileName] () {
            addPaintWidget(createPaintWidget(fileName));
        });
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

    m_pbSettingsWidget->setVisible(false);
    m_pbAdvSettingsWidget->setVisible(false);
    m_scSettingsWidget->setVisible(false);
    m_lineSettingsWidget->setVisible(false);
}

PaintWidget *MainWindow::createPaintWidget(const QString &imagePath) const
{
    return new PaintWidget(imagePath);
}

PaintWidget *MainWindow::createPaintWidget(const QSize &imageSize) const
{
    return new PaintWidget(imageSize);
}

void MainWindow::addPaintWidget(PaintWidget *widget)
{
    widget->autoScale();

    connect(widget, &PaintWidget::zoomChanged, [this] (float scale) {
        this->zoomCombo->setItemText(0, QString::number((int)(scale*100)).append("%"));
        this->zoomCombo->setCurrentIndex(0);
    });

    addChildWindow(widget);

    on_toolButtonPaintBrush_clicked();
}

void MainWindow::addChildWindow(PaintWidget *widget)
{
    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QMdiSubWindow *mdiSubWindow = ui->mdiArea->addSubWindow(widget);
    QString title = widget->imagePath().isEmpty() ? UNTITLED_TAB_NAME : widget->imagePath();
    title = title + " [*]";
    mdiSubWindow->setWindowTitle(title);
    mdiSubWindow->installEventFilter(this);
    mdiSubWindow->show();

    connect(widget, &PaintWidget::contentChanged, [widget, mdiSubWindow, title, this] () {
        if (!mdiSubWindow->isWindowModified()) {
            mdiSubWindow->setWindowModified(true);
        }
        ui->actionUndo->setEnabled(widget->isUndoEnabled());
        ui->actionRedo->setEnabled(widget->isRedoEnabled());
    });
}

void MainWindow::saveContent()
{
    QString currentFileName = ui->mdiArea->currentSubWindow()->windowTitle();

    if(currentFileName.contains(UNTITLED_TAB_NAME + " [*]"))
    {
        on_actionSave_As_triggered();
    } else
    {
        saveImage(currentFileName.mid(0,currentFileName.length() - 4));
        ui->mdiArea->currentSubWindow()->setWindowModified(false);
    }
}

bool MainWindow::saveImage(const QString &fileName)
{
    if (fileName.isEmpty())
        return false;

    PaintWidget *widget;
    widget = getCurrentPaintWidget();

    return widget ? widget->image().save(fileName) : false;
}

bool MainWindow::handleCloseChildWindow(QMdiSubWindow *subWindow)
{
    if (!subWindow)
        return false;

    ui->mdiArea->setActiveSubWindow(subWindow);

    if (subWindow->isWindowModified()) {
        int buttonCode = QMessageBox::question(this, tr("Unsaved Changes"), tr("Save changes before leaving?"),
                                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

        if (buttonCode == QMessageBox::Cancel) {
            return true;
        } else if (buttonCode == QMessageBox::Yes) {
            saveContent();
        }
    }

    subWindow->setWindowModified(false);
    subWindow->close();

    return false;
}

bool MainWindow::handleCloseTabs()
{
    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();

    for (int i = 0; i < windows.size(); ++i) {
       QMdiSubWindow *subWindow = windows.at(i);
       if(subWindow->isWindowModified())
       {
           if (handleCloseChildWindow(subWindow)) {
               return true;
           } else {

           }
       } else {
           subWindow->close();
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

bool MainWindow::eventFilter(QObject * obj, QEvent * e)
{
    switch (e->type())
    {
        case QEvent::Close:
        {
            QMdiSubWindow * subWindow = dynamic_cast<QMdiSubWindow*>(obj);
            Q_ASSERT (subWindow != NULL);
            if(handleCloseChildWindow(subWindow))
            {
                e->ignore();
                return true;
            }

            subWindow->widget()->deleteLater();
            break;
        }
        default:
            qt_noop();
    }

    return QObject::eventFilter(obj, e);
}

void MainWindow::on_toolButtonPointer_clicked()
{
    clearToolpalette();
    m_toolSelected = "pointer";
    ui->toolButtonPointer->setChecked(true);
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(MOUSE_POINTER);
}

void MainWindow::on_toolButtonDropper_clicked()
{
    clearToolpalette();
    m_toolSelected = "dropper";
    ui->toolButtonDropper->setChecked(true);
    PaintWidget *widget = getCurrentPaintWidget();
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
    m_lineSettingsWidget->setVisible(true);
    onLineSettingsChanged();
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(LINE_TOOL);
}

void MainWindow::on_toolButtonPaintBucket_clicked()
{
    clearToolpalette();
    m_toolSelected = "paintBucket";
    ui->toolButtonPaintBucket->setChecked(true);
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(PAINT_BUCKET);
}

void MainWindow::on_toolButtonSprayCan_clicked()
{
    clearToolpalette();
    m_toolSelected = "sprayCan";
    ui->toolButtonSprayCan->setChecked(true);
    m_scSettingsWidget->setVisible(true);
    onSprayCanSettingsChanged();

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(SPRAY_CAN);
}

void MainWindow::on_toolButtonPaintBrush_clicked()
{
    clearToolpalette();
    m_toolSelected = "paintBrush";
    ui->toolButtonPaintBrush->setChecked(true);
    m_pbSettingsWidget->setVisible(true);

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(PAINT_BRUSH);
}

void MainWindow::on_toolButtonPaintBrushAdv_clicked()
{
    clearToolpalette();
    m_toolSelected = "paintBrushAdv";
    ui->toolButtonPaintBrushAdv->setChecked(true);
    m_pbAdvSettingsWidget->setVisible(true);
    onPaintBrushAdvSettingsChanged();

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(PAINT_BRUSH_ADV);

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

PaintWidget* MainWindow::getCurrentPaintWidget()
{
    PaintWidget *widget = 0;
    if(ui->mdiArea->currentSubWindow())
    {
        widget = static_cast<PaintWidget *>(ui->mdiArea->currentSubWindow()->widget());
    }

    return widget;
}

void MainWindow::on_actionGrayScale_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->grayscale(widget->image()));
    }
}

void MainWindow::on_actionFlip_Horizontal_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->flipHorz(widget->image()));
    }
}

void MainWindow::on_actionFlip_Vertical_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->flipVert(widget->image()));
    }
}

void MainWindow::on_actionRotate_CCW_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->rotateCCW(widget->image()));
    }
}

void MainWindow::on_actionRotate_CW_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->rotateCW(widget->image()));
    }
}

void MainWindow::on_actionOil_Paint_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->oilPaint(widget->image()));
    }
}

void MainWindow::on_actionCharcoal_Drawing_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->charcoal(widget->image()));
    }
}

void MainWindow::on_actionSwirl_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->swirl(widget->image()));
    }
}

void MainWindow::on_actionClose_triggered()
{
    handleCloseChildWindow(ui->mdiArea->currentSubWindow());
}

void MainWindow::on_actionClose_all_triggered()
{
    handleCloseTabs();
}

void MainWindow::on_actionFilterbar_triggered()
{
    if(ui->toolBar->isVisible())
    {
        ui->toolBar->hide();
    }
    else
        ui->toolBar->show();
}

void MainWindow::on_actionToolpalette_triggered()
{
    if(ui->dockWidget_palette->isVisible())
    {
        ui->dockWidget_palette->hide();
    }
    else
        ui->dockWidget_palette->show();
}

void MainWindow::on_actionZoom_in_triggered()
{

}

void MainWindow::on_actionZoom_out_triggered()
{

}

void MainWindow::on_actionAuto_zoom_triggered()
{

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

void MainWindow::onPaintBrushAdvSettingsChanged()
{
    PAINT_BRUSH_ADV->setWidth(m_pbAdvSettingsWidget->brushWidth());
    PAINT_BRUSH_ADV->setBrushPixmap(m_pbAdvSettingsWidget->brushPixmap());
    PAINT_BRUSH_ADV->setPreassure(m_pbAdvSettingsWidget->preassure());
    PAINT_BRUSH_ADV->setFade(m_pbAdvSettingsWidget->fade());
    PAINT_BRUSH_ADV->setStep(m_pbAdvSettingsWidget->step());
}

void MainWindow::onSprayCanSettingsChanged()
{
    SPRAY_CAN->setRadius(m_scSettingsWidget->radius());
    SPRAY_CAN->setPressure(m_scSettingsWidget->pressure());
    SPRAY_CAN->setDispersion(m_scSettingsWidget->dispersion());
    SPRAY_CAN->setRainbow(m_scSettingsWidget->rainbow());
}

void MainWindow::onLineSettingsChanged()
{
    LINE_TOOL->setWidth(m_lineSettingsWidget->width());
    LINE_TOOL->setOpacity(m_lineSettingsWidget->opacity());
    LINE_TOOL->setAntialias(m_lineSettingsWidget->antialias());
    LINE_TOOL->setStyle(m_lineSettingsWidget->style());
    LINE_TOOL->setArrowStyle(m_lineSettingsWidget->arrowStyle());
}

void MainWindow::onPickPrimaryColor(const QPoint& pos)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    {
        const QImage& image = widget->image();
        const QColor& color = image.pixel(pos);

        ui->colorBoxWidget->setPrimaryColor(color);
    }
}

void MainWindow::onPickSecondaryColor(const QPoint& pos)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    {
        const QImage& image = widget->image();
        const QColor& color = image.pixel(pos);

        ui->colorBoxWidget->setSecondaryColor(color);
    }
}

void MainWindow::onFloodFillPrimaryColor(const QPoint& pos)
{
    const QColor& color = ui->colorBoxWidget->primaryColor();
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->floodFill(widget->image(), pos, color));
    }
}

void MainWindow::onFloodFillSecondaryColor(const QPoint& pos)
{
    const QColor& color = ui->colorBoxWidget->secondaryColor();
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->setImage(FilterManager::instance()->floodFill(widget->image(), pos, color));
    }
}

void MainWindow::onCrop(const QRect& rect)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        QRect crop = widget->image().rect().intersected(rect);
        widget->setImage(widget->image().copy(crop));
    }
}

void MainWindow::onCopy()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setImage(widget->image());
    }
}

void MainWindow::onPaste()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        QClipboard *clipboard = QApplication::clipboard();
        MOUSE_POINTER->setOverlayImage(clipboard->image());
    }
}

void MainWindow::onZoomChanged(const QString& scale)
{
    PaintWidget *widget = getCurrentPaintWidget();
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

void MainWindow::onMultiWindowModeChanged(bool multiWindowMode)
{
    if(multiWindowMode)
    {
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
    } else
    {
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    }
}

void MainWindow::onSubWindowActivated(QMdiSubWindow *window)
{
    Q_UNUSED(window)
    PaintWidget *widget = getCurrentPaintWidget();
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

void MainWindow::on_actionRegister_triggered()
{
    registerDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionUndo_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->undo();
    }
}

void MainWindow::on_actionRedo_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->redo();
    }
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer);
    if (dialog->exec() == QDialog::Accepted)
    {
        QPainter painter;
        if (! painter.begin(&printer)) {
            qWarning("Failed to init printer");
            return;
        }
        PaintWidget *widget = getCurrentPaintWidget();
        if (widget) {
            painter.drawImage(0,0,widget->image());
        }
        painter.end();
    }
    delete dialog;
}

void MainWindow::on_actionAcquire_image_triggered()
{
    addPaintWidget(createPaintWidget(QSize(480,640)));
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget) {
        widget->showProgressIndicator(true);
    }
    m_scanManager->scan();
}

void MainWindow::on_actionScan_triggered()
{
   this->on_actionAcquire_image_triggered();
}

void MainWindow::on_actionSelect_device_triggered()
{
    m_scanManager->list();
}

void MainWindow::onListFnished(int,QProcess::ExitStatus status)
{
    if(status == QProcess::NormalExit)
    {
        ScanDevicesDialog dlg(this);
        dlg.setSourceList(m_scanManager->getSourceList());
        if (dlg.exec() == QDialog::Accepted)
        {
            m_scanManager->setCurrentSourceId(dlg.getCurrentSourceId());
        }
    }
}

void MainWindow::onScanFnished(int,QProcess::ExitStatus status)
{
    if(status == QProcess::NormalExit)
    {
        QFile file("FROM_SCANNER.bmp");

        PaintWidget *widget = getCurrentPaintWidget();
        if (widget) {
            widget->showProgressIndicator(false);
            widget->setImage(QImage(file.fileName()));
            widget->autoScale();
        }

        file.remove();
    }
}

void MainWindow::createKeyboardShortcuts() {
    //File Menu
    ui->actionNew->setShortcut(QString("Ctrl+N"));
    ui->actionOpen->setShortcut(QString("Ctrl+O"));
    ui->actionSave->setShortcut(QString("Ctrl+S"));
    ui->actionSave_As->setShortcut(QString("Ctrl+Shift+S")); //broken
    ui->actionPrint->setShortcut(QString("Ctrl+P"));
    ui->actionClose->setShortcut(QString("Ctrl+W"));
    ui->actionQuit->setShortcut(QString("Ctrl+Q"));
    //Edit Menu
    ui->actionCut->setShortcut(QString("Ctrl+X")); //not implemented
    ui->actionCopy->setShortcut(QString("Ctrl+C")); //broken
    ui->actionPaste->setShortcut(QString("Ctrl+V")); //broken
    ui->actionUndo->setShortcut(QString("Ctrl+Z"));
    ui->actionRedo->setShortcut(QString("Ctrl+Y"));
    ui->actionImage_properties->setShortcut(QString("Ctrl+J")); //not implemented
    //Image Menu
    ui->actionImage_Size->setShortcut(QString("Ctrl+H"));
    ui->actionCrop->setShortcut(QString("Ctrl+Shift+H")); //broken
    //View Menu
    ui->actionToolpalette->setShortcut(QString("Ctrl+L")); //broken

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
    //ui->actionCopy->setEnabled(false);
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
    //ui->actionFilterbar->setEnabled(false);
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
    //ui->actionImage_Size->setEnabled(false);
    ui->actionImage_properties->setEnabled(false);
    ui->actionAcquire_image->setEnabled(true);
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
    //ui->actionPaste->setEnabled(false);
    ui->actionPaste_and_text_bounding_box->setEnabled(false);
    ui->actionPaste_as_new_image->setEnabled(false);
    ui->actionPaste_shape->setEnabled(false);
    ui->actionPaste_special->setEnabled(false);
    ui->actionPosterize->setEnabled(false);
    ui->actionPrint->setEnabled(true);
    ui->actionPurge->setEnabled(false);
    ui->actionRGB_Mode->setEnabled(false);
    //ui->actionRecent_files->setEnabled(false);
    //ui->actionRedo->setEnabled(false);
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
    //ui->actionScan->setEnabled(false);
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
    //ui->actionText->setEnabled(false);
    ui->actionTexture->setEnabled(false);
    //ui->actionToolpalette->setEnabled(false);
    ui->actionTransform->setEnabled(false);
    ui->actionTransform_2->setEnabled(false);
    ui->actionTransparency_mask->setEnabled(false);
    ui->actionTransparent_colour->setEnabled(false);
    //ui->actionUndo->setEnabled(false);
    ui->actionValidate->setEnabled(false);
    ui->actionVisual_effect->setEnabled(false);
    ui->actionZoom_in->setEnabled(false);
    ui->actionZoom_out->setEnabled(false);

    ui->toolButtonWand->setEnabled(false);
    ui->toolButtonStamp->setEnabled(false);
    //ui->toolButtonSprayCan->setEnabled(false);
    //ui->toolButtonPaintBrushAdv->setEnabled(false);
    ui->toolButtonLine->setEnabled(true);
    ui->toolButtonBlur->setEnabled(false);
}
