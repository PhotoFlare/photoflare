/*

  Main window class for the PhotoFlare application.

*/

#include <QDebug>

#include <QClipboard>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMdiSubWindow>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QShortcut>
#include <QDateTime>
#include <QThread>
#include <QMimeData>
#include <QDesktopServices>
#include <QTimer>
#include <QStandardPaths>
#include <QImageReader>

#include "./tools/PaintBrushTool.h"
#include "./tools/PaintBrushAdvTool.h"
#include "./tools/ColourPickerTool.h"
#include "./tools/PaintBucketTool.h"
#include "./tools/PointerTool.h"
#include "./tools/TextTool.h"
#include "./tools/SprayCanTool.h"
#include "./tools/LineTool.h"
#include "./tools/MagicWandTool.h"
#include "./tools/StampTool.h"
#include "./tools/BlurTool.h"
#include "./tools/EraserTool.h"
#include "./tools/SmudgeTool.h"

#include "./toolSettings/pointersettingswidget.h"
#include "./toolSettings/PaintBrushSettingsWidget.h"
#include "./toolSettings/PaintBrushAdvSettingsWidget.h"
#include "./toolSettings/SprayCanSettingsWidget.h"
#include "./toolSettings/LineSettingsWidget.h"
#include "./toolSettings/MagicWandSettingsWidget.h"
#include "./toolSettings/StampSettingsWidget.h"
#include "./toolSettings/BlurSettingsWidget.h"
#include "./toolSettings/erasersettingswidget.h"
#include "./toolSettings/SmudgeSettingsWidget.h"

#include "managers/ToolManager.h"
#include "Settings.h"
#include "managers/FilterManager.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgets/PaintWidget.h"
#include "dialogs/NewDialog.h"
#include "dialogs/aboutdialog.h"
#include "dialogs/textdialog.h"
#include "dialogs/prefsdialog.h"
#include "dialogs/gradientdialog.h"
#include "dialogs/ScanDevicesDialog.h"
#include "dialogs/imagepropertiesdialog.h"
#include "dialogs/outerframedialog.h"
#include "dialogs/dropshadowdialog.h"
#include "dialogs/huedialog.h"
#include "dialogs/compressiondialog.h"
#include "dialogs/transparentdialog.h"
#include "dialogs/batchdialog.h"
#include "dialogs/plugindialog.h"
#include "dialogs/checkupdatedialog.h"

#include "workers/BatchProcessWorker.h"
#include "progress/batchprogress.h"
#include "workers/filterworker.h"

#define PAINT_BRUSH ToolManager::instance()->paintBrush()
#define PAINT_BRUSH_ADV ToolManager::instance()->paintBrushAdv()
#define COLOUR_PICKER ToolManager::instance()->colourPicker()
#define PAINT_BUCKET ToolManager::instance()->paintBucket()
#define MOUSE_POINTER ToolManager::instance()->mousePointer()
#define TEXT_TOOL ToolManager::instance()->textTool()
#define SPRAY_CAN ToolManager::instance()->sprayCanTool()
#define LINE_TOOL ToolManager::instance()->lineTool()
#define MAGIC_WAND ToolManager::instance()->magicWandTool()
#define STAMP_TOOL ToolManager::instance()->stampTool()
#define BLUR_TOOL ToolManager::instance()->blurTool()
#define ERASER_TOOL ToolManager::instance()->eraserTool()
#define SMUDGE_TOOL ToolManager::instance()->smudgeTool()

namespace
{
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
    }
    else
    {
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    }

    if(SETTINGS->getDockLayout() == "Left")
    {
        addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidget_palette);
        addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetSettings);
    }

    // Center colorBox in tool palette.
    ui->verticalLayout->setAlignment(ui->colorBoxWidget, Qt::AlignCenter);

    // Disable actions that are not yet implemented. True = hidden, False = Disabled
    disableUnimplementedActions(true);

    // Create the keyboard shortcut bindings
    createKeyboardShortcuts();

    // Add zoom ComboBox
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

    // Add Settings Widgets to the Dock
    m_ptSettingsWidget = new PointerSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_ptSettingsWidget);
    connect(m_ptSettingsWidget, &PointerSettingsWidget::settingsChanged, this, &MainWindow::onPointerToolSettingsChanged);

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

    m_magicWandSettingsWidget = new MagicWandSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_magicWandSettingsWidget);
    connect(m_magicWandSettingsWidget, &MagicWandSettingsWidget::settingsChanged, this, &MainWindow::onMagicWandSettingsChanged);

    m_stampSettingsWidget = new StampSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_stampSettingsWidget);
    connect(m_stampSettingsWidget, &StampSettingsWidget::settingsChanged, this, &MainWindow::onStampSettingsChanged);

    m_blurSettingsWidget = new BlurSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_blurSettingsWidget);
    connect(m_blurSettingsWidget, &BlurSettingsWidget::settingsChanged, this, &MainWindow::onBlurSettingsChanged);

    m_eraserSettingsWidget = new EraserSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_eraserSettingsWidget);
    connect(m_eraserSettingsWidget, &EraserSettingsWidget::settingsChanged, this, &MainWindow::onEraserSettingsChanged);

    m_smudgeSettingsWidget = new SmudgeSettingsWidget;
    ui->dockWidgetSettings->layout()->addWidget(m_smudgeSettingsWidget);
    connect(m_smudgeSettingsWidget, &SmudgeSettingsWidget::settingsChanged, this, &MainWindow::onSmudgeSettingsChanged);

    // Disable undo/redo buttons on startup
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    // Setup Tool Initial Colours and link signals to the colorChanged functions
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

    // Setup signals for more Tools
    QObject::connect(COLOUR_PICKER, SIGNAL(pickPrimaryColor(const QPoint&)), this, SLOT(onPickPrimaryColor(const QPoint&)));
    QObject::connect(COLOUR_PICKER, SIGNAL(pickSecondaryColor(const QPoint&)), this, SLOT(onPickSecondaryColor(const QPoint&)));

    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillPrimaryColor(const QPoint&)), this, SLOT(onFloodFillPrimaryColor(const QPoint&)));
    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillSecondaryColor(const QPoint&)), this, SLOT(onFloodFillSecondaryColor(const QPoint&)));

    QObject::connect(MOUSE_POINTER, SIGNAL(crop(const QRect&)), this, SLOT(onCrop(const QRect&)));
    QObject::connect(MOUSE_POINTER, SIGNAL(save()), this, SLOT(on_actionSave_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(saveAs()), this, SLOT(on_actionSave_As_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(copy()), this, SLOT(onCopy()));
    QObject::connect(MOUSE_POINTER, SIGNAL(paste()), this, SLOT(onPaste()));

    QObject::connect(TEXT_TOOL, SIGNAL(editTextFinished()), this, SLOT(on_TextTool_finished()));
    QObject::connect(TEXT_TOOL, SIGNAL(editText(const QString&,const QFont&, const QColor&)), this, SLOT(onEditText(const QString&,const QFont&, const QColor&)));

    QObject::connect(SETTINGS, SIGNAL(multiWindowModeChanged(bool)), this, SLOT(onMultiWindowModeChanged(bool)));

    QObject::connect(MAGIC_WAND, SIGNAL(selectPrimaryColor(const QPoint&,int,bool)), this, SLOT(onSelectPrimaryColor(const QPoint&,int,bool)));

    // Setup some other defaults on startup
    setWindowSize();
    updateRecents();

    // Initialize ScanManager for scanning images
    m_scanManager = new ScanManager();
    QObject::connect(m_scanManager, SIGNAL(listFinished(int,QProcess::ExitStatus)), this, SLOT(onListFnished(int,QProcess::ExitStatus)));
    QObject::connect(m_scanManager, SIGNAL(scanFinished(int,QProcess::ExitStatus)), this, SLOT(onScanFnished(int,QProcess::ExitStatus)));

    transparentDialog = 0;

    on_toolButtonPointer_clicked();

    // Remove Qt contextmenu from the toolbars
    setContextMenuPolicy(Qt::NoContextMenu);

    batchLbl = new QLabel(tr("Ready"));
    batchLbl->setStyleSheet("margin:0 100 0 0");
    ui->statusBar->addWidget(batchLbl);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*

    | FILTER WORKER |

*/

void MainWindow::on_image_filtered(QImage image)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(image);

    batchLbl->setText(tr("Ready"));
}

void MainWindow::applyThreadedFilter(QString filterName)
{
    QThread *thread = new QThread();
    FilterWorker *worker = new FilterWorker();
    worker->setFilter(filterName);

    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    worker->setImage(widget->image());

    worker->setParent(this);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(filterProcessFinished(QImage)), this, SLOT(on_image_filtered(QImage)));

    thread->start();

    batchLbl->setText(tr("Working..."));
}

/*

    | FILE MENU |

*/

void MainWindow::on_actionNew_triggered()
{
    NewDialog dialog;
    dialog.setMode(NewDialog::NewImage);
    if (dialog.exec())
    {
        addPaintWidget(createPaintWidget(dialog.newImageSize(),dialog.newImageBackgroundColor()));
    }
}

void MainWindow::on_actionOpen_triggered()
{
    const QString& fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
    SETTINGS->getOpenFolder(), tr("All Files (*);;"
                                "Image Files (*.png *.jpg *.jpeg *.gif);;"
                                "PNG(*.png);;"
                                "JPEG(*.jpg *.jpeg);;"
                                "GIF(*.gif);;"
                                "TIFF(*.tif *.tiff);;"
                                "BMP(*.bmp);;"
                                "ICO(*.ico)"));
/*
                                "ARW (*.arw *.srf *.sr2);;"
                                "BAY (*.bay);;"
                                "CR2 (*.crw *.cr2);;"
                                "DCS (*.dcs *.dcr *.drf *.k25 *.kdc);;"
                                "MOS (*.mos);;"
                                "NEF (*.nef *.nrw);;"
                                "RAW (*.raw *.rw2)"));
*/
    openFile(fileName);

    if(SETTINGS->getPreviouslyOpened() == true)
    {
        QDir d = QFileInfo(fileName).absoluteDir();
        SETTINGS->setOpenFolder(d.absolutePath());
    }
}

void MainWindow::openFile(const QString& fileName)
{
    QImageReader reader(fileName);
    if (!fileName.isEmpty() && reader.format() != "")
    {
        addPaintWidget(createPaintWidget(fileName));
        SETTINGS->addRecentFile(fileName);
        updateRecents();
    }
    if(reader.format() == "")
    {
        showError(tr("Please open a valid image file"));
    }
}

void MainWindow::handleMessage(const QString& message)
{
    QFileInfo fileInfo(message);
    if(fileInfo.exists())
        openFile(QString(fileInfo.absoluteFilePath()));
}

void MainWindow::on_actionRevert_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->revert();
    }
}

bool MainWindow::fileExists(QString path) {
    QFileInfo check_file(path);

    return (check_file.exists() && check_file.isFile());
}

void MainWindow::updateRecents()
{
    ui->menuRecent_Files->clear();

    QList<QVariant> recentFiles = SETTINGS->getRecentFiles();
    QList<QVariant>::iterator i;

    for(i = recentFiles.begin(); i != recentFiles.end(); i++)
    {
        const QString& fileName = (*i).toString();
        if(fileExists(fileName))
        {
            QAction* action = ui->menuRecent_Files->addAction(fileName);
            connect(action, &QAction::triggered, [this, fileName] () {
                addPaintWidget(createPaintWidget(fileName));
            });
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        saveContent();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        // Load default filter by image file name.
        QString currentFileName;
        currentFileName = ui->mdiArea->currentSubWindow()->windowTitle();

        QString suffix = QFileInfo(currentFileName).suffix();

        QStringList filters;
        //filters << tr("All Files (*)");
        filters << tr("png (*.png)");
        filters << tr("jpg (*.jpg *.jpeg)");
        filters << tr("bmp (*.bmp)");
        filters << tr("pbm (*.pbm)");
        filters << tr("pgm (*.pgm)");
        filters << tr("ppm (*.ppm)");
        filters << tr("ico (*.ico)");

        QString defaultFilter;
        if (!suffix.isEmpty())
        {
            foreach (const QString &filter, filters) {
                if (filter.contains(suffix, Qt::CaseInsensitive))
                {
                    defaultFilter = filter;
                    break;
                }
            }
        }
        if (defaultFilter.isEmpty() && SETTINGS->getSaveFormatEnabled())
            defaultFilter = filters.at(SETTINGS->getSaveFormat().toInt());

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        SETTINGS->getSaveFolder(), filters.join(";;"), &defaultFilter);

        if(SETTINGS->getPreviouslyOpenedSave() == true)
        {
            QDir d = QFileInfo(fileName).absoluteDir();
            SETTINGS->setSaveFolder(d.absolutePath());
        }


        if (fileName.isEmpty())
            return;

        // WORKAROUND: Add the extension to the file name manually.
        QString fileNameSuffix = QFileInfo(fileName).suffix();

        if (fileNameSuffix.isEmpty())
        {
                QStringList list = defaultFilter.split(" (");
                if (list.count() == 2)
                {
                    fileNameSuffix = list.at(0);
                    fileName += "." + fileNameSuffix;
                }
        }

        int quality = -1;

        if(SETTINGS->getCompressionDialogEnabled() && fileNameSuffix == "jpg" || fileNameSuffix == "jpeg")
        {
            CompressionDialog dlg;
            dlg.exec();
            quality = dlg.quality();

            //If dialog Accepted
            if(dlg.enableSaveImage)
            {
                if (saveImage(fileName,quality))
                {
                    PaintWidget *widget = getCurrentPaintWidget();
                    if (widget)
                        widget->setImage(QImage(fileName));

                    //Update tab label
                    ui->mdiArea->currentSubWindow()->setWindowModified(false);
                    ui->mdiArea->currentSubWindow()->setWindowTitle(fileName + " [*]");

                    //Update image path so the file properties dialog will show all file details
                    QDir d = QFileInfo(fileName).absoluteDir();
                    widget->setImagePath(d.absolutePath());

                    SETTINGS->addRecentFile(fileName);
                    updateRecents();
                }
                else
                    showError(tr("Unable to save image."));

            }
        }
        //Other file formats
        else
        {
            if (saveImage(fileName,quality))
            {
                PaintWidget *widget = getCurrentPaintWidget();
                if (widget)
                    widget->setImage(QImage(fileName));

                //Update tab label
                ui->mdiArea->currentSubWindow()->setWindowModified(false);
                ui->mdiArea->currentSubWindow()->setWindowTitle(fileName + " [*]");

                //Update image path so the file properties dialog will show all file details
                QDir d = QFileInfo(fileName).absoluteDir();
                widget->setImagePath(d.absolutePath());

                SETTINGS->addRecentFile(fileName);
                updateRecents();
            }
            else
                showError(tr("Unable to save image."));
        }
    }
}

void MainWindow::saveContent()
{
    QString currentFileName = ui->mdiArea->currentSubWindow()->windowTitle();

    if(currentFileName.contains(UNTITLED_TAB_NAME + " [*]"))
    {
        on_actionSave_As_triggered();
    }
    else
    {
        saveImage(currentFileName.mid(0,currentFileName.length() - 4),-1);
        ui->mdiArea->currentSubWindow()->setWindowModified(false);
    }
}

bool MainWindow::saveImage(const QString &fileName, int quality)
{
    if (fileName.isEmpty())
        return false;

    PaintWidget *widget;
    widget = getCurrentPaintWidget();

    return widget ? widget->image().save(fileName,0,quality) : false;
}

void MainWindow::on_actionClose_triggered()
{
    handleCloseChildWindow(ui->mdiArea->currentSubWindow());
}

void MainWindow::on_actionClose_all_triggered()
{
    handleCloseTabs();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer);
    if (dialog->exec() == QDialog::Accepted)
    {
        QPainter painter;
        if (! painter.begin(&printer))
        {
            qWarning("Failed to init printer");
            return;
        }
        PaintWidget *widget = getCurrentPaintWidget();
        if (widget)
        {
            painter.drawImage(0,0,widget->image());
        }
        painter.end();
    }
    delete dialog;
}

void MainWindow::on_actionAcquire_image_triggered()
{
    addPaintWidget(createPaintWidget(QSize(480,640),Qt::white));
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
        if (widget)
        {
            widget->showProgressIndicator(false);
            widget->setImage(QImage(file.fileName()));
            widget->autoScale();
        }

        file.remove();
    }
}

void MainWindow::on_actionImage_properties_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
    imagePropertiesDialog dialog(this);

        if(!widget->imagePath().isEmpty())
        {
            QFileInfo fileInfo(widget->imagePath());

            dialog.setImageName(fileInfo.fileName());
            dialog.setFolder(fileInfo.path());
            dialog.setFileSize(fileInfo.size());
            dialog.setDate(QString("%1").arg(fileInfo.lastModified().toString("dd/MM/yyyy hh:mm")));
        }
        dialog.setColorCount(widget->image());
        dialog.setSize(widget->image().size());
        int size = widget->image().width() * widget->image().height() * 3;
        dialog.setMemorySize(size);
        dialog.setTotalSize((widget->undoCount()+1)*size);
    dialog.exec();
    }
}

void MainWindow::on_actionQuit_triggered()
{
    if (!handleCloseTabs())
    {
        saveGeometryState();
        qApp->quit();
    }
}

/*

    | EDIT MENU |

*/

void MainWindow::on_actionUndo_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->undo();
        updateStatusArea(widget->image().width(),widget->image().height());
    }
}

void MainWindow::on_actionRedo_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->redo();
        updateStatusArea(widget->image().width(),widget->image().height());
    }
}

void MainWindow::onCopy()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        QClipboard *clipboard = QApplication::clipboard();
        clipboard->setImage(widget->image());
    }
}

void MainWindow::onPaste()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        QClipboard *clipboard = QApplication::clipboard();
        widget->setPaintTool(MOUSE_POINTER);
        MOUSE_POINTER->setOverlayImage(clipboard->image());
    }
    else
    {
        QClipboard *clipboard = QApplication::clipboard();
        if(clipboard->mimeData()->hasImage())
        {
            addPaintWidget(createPaintWidget(clipboard->image().size(),Qt::white));
            widget = getCurrentPaintWidget();
            if (widget)
            {
                widget->setImage(clipboard->image());
                widget->setPaintTool(MOUSE_POINTER);
            }
        }
    }
}

void MainWindow::on_actionCopy_triggered()
{
    onCopy();
}

void MainWindow::on_actionPaste_triggered()
{
    onPaste();
}

/*

    | IMAGE MENU |

*/

void MainWindow::on_actionRGB_Mode_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(widget->image().convertToFormat(QImage::Format_ARGB32_Premultiplied));
}

void MainWindow::on_actionIndexed_Mode_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(widget->image().convertToFormat(QImage::Format_Indexed8));
}

void MainWindow::on_actionFlip_Horizontal_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("flipHorz");
}

void MainWindow::on_actionFlip_Vertical_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("flipVert");
}

void MainWindow::on_actionRotate_CCW_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("rotateCCW");
}

void MainWindow::on_actionRotate_CW_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("rotateCW");
}

void MainWindow::on_actionImage_Size_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (!widget)
        return;

    NewDialog dialog;
    dialog.setWindowTitle("Resize Image");
    dialog.setMode(NewDialog::ResizeImage);
    dialog.setImageSize(widget->image().size());
    if (dialog.exec())
        widget->setImage(widget->image().scaled(dialog.newImageSize()));

    updateStatusArea(dialog.newImageSize().width(),dialog.newImageSize().height());
}

void MainWindow::on_actionCanvas_Size_triggered()
{
    NewDialog dialog;
    dialog.setWindowTitle("Resize Canvas");
    dialog.setMode(NewDialog::ResizeCanvas);
    if (dialog.exec())
    {
        QImage canvas (dialog.newImageSize(), QImage::Format_ARGB32_Premultiplied);
        canvas.fill(dialog.backgroundColor());
        PaintWidget *widget = getCurrentPaintWidget();
        QPoint pos;
        if (widget)
        {
            QPainter painter(&canvas);
            switch(dialog.imagePosition())
            {
                case LeftTop:
                    pos = QPoint(0,0);
                    break;
                case CenterTop:
                    pos = QPoint(canvas.width()/2 - widget->image().width()/2, 0);
                    break;
                case RightTop:
                    pos = QPoint(canvas.width() - widget->image().width(), 0);
                    break;
                case LeftCenter:
                    pos = QPoint(0, canvas.height()/2 - widget->image().height()/2);
                    break;
                case CenterCenter:
                    pos = QPoint(canvas.width()/2 - widget->image().width()/2, canvas.height()/2 - widget->image().height()/2);
                    break;
                case RightCenter:
                    pos = QPoint(canvas.width() - widget->image().width(), canvas.height()/2 - widget->image().height()/2);
                    break;
                case LeftBottom:
                    pos = QPoint(0, canvas.height() - widget->image().height());
                    break;
                case CenterBottom:
                    pos = QPoint(canvas.width()/2 - widget->image().width()/2, canvas.height() - widget->image().height());
                    break;
                case RightBottom:
                    pos = QPoint(canvas.width() - widget->image().width(), canvas.height() - widget->image().height());
                    break;
            }
            painter.drawImage(pos, widget->image());
            painter.end();
            widget->setImage(canvas);
        }
    }
}

void MainWindow::on_actionAutomatic_Crop_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("trim");
}

void MainWindow::on_actionOutside_frame_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        OuterFrameDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted)
        {
            widget->setImage(FilterManager::instance()->outsideFrame(widget->image(),dialog.width(),dialog.color()));
        }
    }
}

void MainWindow::on_actionOutside_drop_shadow_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        dropshadowDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted)
        {
            widget->setImage(FilterManager::instance()->dropShadow(widget->image(),dialog.radius(),dialog.padding(),dialog.color(),dialog.offsetx(),dialog.offsety()));
            dialog.on_buttonBox_accepted();
        }
    }
}

void MainWindow::on_actionTransparent_colour_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        origImage = widget->image();
        emit ui->toolButtonDropper->clicked();
        transparentDialog = new TransparentDialog();
        QObject::connect(transparentDialog, SIGNAL(previewTransparent(QColor,int)), this, SLOT(onPreviewTransparent(QColor,int)));
        QObject::connect(transparentDialog, SIGNAL(dialogFinished(int)), this, SLOT(onTransparentFinished(int)));
        QObject::connect(transparentDialog, SIGNAL(dialogAccepted()), this, SLOT(onTransparentAccepted()));
        QObject::connect(transparentDialog, SIGNAL(dialogRejected()), this, SLOT(onTransparentRejected()));
        transparentDialog->setColor(widget->image().pixel(0,0));
        transparentDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        transparentDialog->show();
    }
}

void MainWindow::onTransparentFinished(int)
{
    delete transparentDialog;
    transparentDialog = 0;
}

void MainWindow::onTransparentAccepted()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(FilterManager::instance()->floodFillOpacity(widget->image(), transparentDialog->color(), transparentDialog->tolerance()));
}

void MainWindow::onTransparentRejected()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(origImage);
}

void MainWindow::onPreviewTransparent(QColor color, int tolerance)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(FilterManager::instance()->floodFillOpacity(origImage, color, tolerance));
}

void MainWindow::on_TextTool_finished()
{
    m_toolSelected = m_previousToolSelected;
    QTimer::singleShot(1000, this, SLOT(refreshTools()));
}

void MainWindow::on_actionText_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        textDialog dialog(this);
        if(dialog.exec())
        {
            // Set the previous tool so we can revert back to it after text placement
            m_previousToolSelected = m_toolSelected;

            clearToolpalette();
            m_toolSelected = "text";
            PaintWidget *widget = getCurrentPaintWidget();
            if (widget)
            {
                widget->setPaintTool(TEXT_TOOL);
                TEXT_TOOL->setText(dialog.text(), dialog.font(), dialog.color(), dialog.antialias(), dialog.position());
            }
        }
    }
}

void MainWindow::onEditText(const QString& text,const QFont& font, const QColor& color)
{
    textDialog dialog(this);
    dialog.editText(text, font, color);
    if(dialog.exec())
        TEXT_TOOL->setText(dialog.text(), dialog.font(), dialog.color(), dialog.antialias(), dialog.position());
}

/*

    | SELECTION MENU |

*/

void MainWindow::on_actionShow_selection_triggered(bool checked)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->setSelectionVisible(checked);
        ui->actionCrop->setEnabled(checked && widget->isSelectionVisible());
    }
}

void MainWindow::onSelectionChanged(bool visible)
{
    ui->actionCrop->setEnabled(ui->actionShow_selection->isChecked() && visible);
}

/*

    | ADJUST MENU |

*/

void MainWindow::on_actionHue_variation_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        HueDialog dlg(this, widget->image());
        QObject::connect(&dlg, SIGNAL(huePreviewChanged(QImage,bool,QColor,int)), this, SLOT(onHuePreviewChanged(QImage,bool,QColor,int)));
        if (dlg.exec())
        {
            if(dlg.isColorizeMethod())
            {
                widget->setImage(FilterManager::instance()->colorize(widget->image(), dlg.color()));
            }
            else
            {
                widget->setImage(FilterManager::instance()->hue(widget->image(), dlg.degrees()));
            }
        }
    }
}

void MainWindow::onHuePreviewChanged(QImage image, bool colorize, QColor color, int degrees)
{
    HueDialog *dlg = qobject_cast<HueDialog*>(sender());
    if(colorize)
    {
        dlg->setPreviewImage(FilterManager::instance()->colorize(image, color));
    }
    else
    {
        dlg->setPreviewImage(FilterManager::instance()->hue(image, degrees));
    }
}

void MainWindow::on_actionAuto_levels_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("autoLevels");
}

void MainWindow::on_actionAuto_contrast_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("autoContrast");
}

void MainWindow::on_actionNegative_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("negative");
}

void MainWindow::on_actionOpacity_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("setOpacity");
}

/*

    | FILTER MENU |

*/

void MainWindow::on_actionSoften_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("soften");
}

void MainWindow::on_actionBlur_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("blur");
}

void MainWindow::on_actionSharpen_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("sharpen");
}

void MainWindow::on_actionReinforce_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("reinforce");
}

void MainWindow::on_actionDustReduction_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("dustreduction");
}

void MainWindow::on_actionDespeckle_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("deSpeckle");
}

void MainWindow::on_actionGaussian_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("gaussianNoise");
}

void MainWindow::on_actionImpulse_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("impulseNoise");
}

void MainWindow::on_actionLaplacian_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("laplacianNoise");
}

void MainWindow::on_actionPoisson_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("poissonNoise");
}

void MainWindow::on_actionGrayScale_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("grayscale");
}

void MainWindow::on_actionOldPhoto_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("oldPhoto");
}

void MainWindow::on_actionSepia_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("sepia");
}

void MainWindow::on_actionEqualize_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("equalizeColours");
}

void MainWindow::on_actionNormalize_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("normalize");
}

void MainWindow::on_actionBlack_and_white_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("blackwhite");
}

void MainWindow::on_actionOil_Paint_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("oil");
}

void MainWindow::on_actionCharcoal_Drawing_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("charcoal");
}

void MainWindow::on_actionSolarize_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("solarize");
}

void MainWindow::on_actionMotion_blur_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("motionBlur");
}

void MainWindow::on_actionSwirl_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
   applyThreadedFilter("swirl");
}

void MainWindow::on_actionWave_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("wave");
}

void MainWindow::on_actionImplode_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("implode");
}

void MainWindow::on_actionExplode_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("explode");
}

void MainWindow::on_actionCrop_To_Center_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("cropToCenter");
}

void MainWindow::on_actionAdd_Simple_Frame_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("simpleFrame");
}

void MainWindow::on_action3D_frame_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("advFrame");
}

void MainWindow::on_actionEmboss_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("emboss");
}

void MainWindow::on_actionMonoChromatic_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("monoChromeEdges");
}

/*

    | VIEW MENU |

*/

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
    this->zoomCombo->setCurrentIndex(this->zoomCombo->currentIndex()+1);
    if(this->zoomCombo->currentText() == "")
    {
        this->zoomCombo->setCurrentIndex(this->zoomCombo->currentIndex()+1);
    }

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->setScale(this->zoomCombo->currentText());
    }
}

void MainWindow::on_actionZoom_out_triggered()
{
    if(this->zoomCombo->currentIndex() > 0)
    {
        this->zoomCombo->setCurrentIndex(this->zoomCombo->currentIndex()-1);
    }

    if(this->zoomCombo->currentText() == "")
    {
        this->zoomCombo->setCurrentIndex(this->zoomCombo->currentIndex()-1);
    }

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->setScale(this->zoomCombo->currentText());
    }
}

void MainWindow::on_actionOriginal_size_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        this->zoomCombo->setItemText(0,"100%");
        this->zoomCombo->setCurrentIndex(0);
        widget->setScale("100%");
    }
}

void MainWindow::on_actionAuto_zoom_triggered(PaintWidget *widget)
{
    widget->autoScale();
    connect(widget, &PaintWidget::zoomChanged, [this] (float scale) {
        this->zoomCombo->setItemText(0, QString::number((int)(scale*100)).append("%"));
        this->zoomCombo->setCurrentIndex(0);
    });
}

void MainWindow::on_actionFull_screen_triggered()
{
    if(this->isFullScreen())
    {
        setWindowSize();
    }
    else
    {
        this->showFullScreen();
    }
}

/*

    | TOOLS MENU |

*/

void MainWindow::on_actionAutomate_Batch_triggered()
{
    batchDialog *dialog = new batchDialog(this);
    if (dialog->exec() == QDialog::Accepted)
    {
        QThread *thread = new QThread();
        BatchProcessWorker *worker = new BatchProcessWorker();
        worker->setParams(dialog);
        worker->setParent(this);
        worker->moveToThread(thread);

        connect(thread, SIGNAL(started()), worker, SLOT(process()));
        connect(worker, SIGNAL(fileProcessFinished(QString,QImage)), this, SLOT(batchProcess_fileProcessFinished(QString,QImage)));
        connect(worker, SIGNAL(batchProgress(int,int)), this, SLOT(batchProcess_batchProgress(int,int)));

        BatchProgress *progress = new BatchProgress(this);
        connect(worker, SIGNAL(batchProgress(int,int)), progress, SLOT(progress(int,int)));
        progress->show();

        thread->start();
    }
}

void MainWindow::batchProcess_fileProcessFinished(QString file, QImage image)
{
    addPaintWidget(createPaintWidget(image.size(),Qt::white));
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->setImage(image);
    }

    if(saveImage(file,-1))
    {
        ui->mdiArea->currentSubWindow()->setWindowModified(false);
        ui->mdiArea->currentSubWindow()->setWindowTitle(file);
    }
}

void MainWindow::batchProcess_batchProgress(int index,int total)
{
    if(index < total)
    {
        this->setWindowTitle(QString("PhotoFlare - %1/%2 (%3%)").arg(index).arg(total).arg(int(100 * (float)index/(float)total)));
    }
    else
    {
        this->setWindowTitle(QString("PhotoFlare"));
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    prefsDialog dialog(this);
    dialog.exec();
}

void MainWindow::on_actionPlugins_triggered()
{
    PluginDialog dialog(this);
    dialog.exec();
}

/*

    | HELP MENU |

*/

void MainWindow::on_actionCheck_for_updates_triggered()
{
    checkupdateDialog cud;
    cud.exec();
}

void MainWindow::on_actionDonate_triggered()
{
    QString link = "http://photoflare.io/donate/";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionAbout_triggered()
{
    aboutDialog dialog(this);
    dialog.exec();
}

/*

    | TOOLPALETTE |

*/

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
    ui->toolButtonEraser->setChecked(false);
    ui->toolButtonSmudge->setChecked(false);

    m_ptSettingsWidget->setVisible(false);
    m_pbSettingsWidget->setVisible(false);
    m_pbAdvSettingsWidget->setVisible(false);
    m_scSettingsWidget->setVisible(false);
    m_lineSettingsWidget->setVisible(false);
    m_magicWandSettingsWidget->setVisible(false);
    m_stampSettingsWidget->setVisible(false);
    m_blurSettingsWidget->setVisible(false);
    m_eraserSettingsWidget->setVisible(false);
    m_smudgeSettingsWidget->setVisible(false);
}

//Make sure the selected tool is re-enabled when we create/open an image
void MainWindow::refreshTools()
{
    if(m_toolSelected == "pointer")
    {
        on_toolButtonPointer_clicked();
    }
    else if(m_toolSelected == "dropper")
    {
        on_toolButtonDropper_clicked();
    }
    else if(m_toolSelected == "wand")
    {
        on_toolButtonWand_clicked();
    }
    else if(m_toolSelected == "line")
    {
        on_toolButtonLine_clicked();
    }
    else if(m_toolSelected == "paintBucket")
    {
        on_toolButtonPaintBucket_clicked();
    }
    else if(m_toolSelected == "sprayCan")
    {
        on_toolButtonSprayCan_clicked();
    }
    else if(m_toolSelected == "paintBrush")
    {
        on_toolButtonPaintBrush_clicked();
    }
    else if(m_toolSelected == "paintBrushAdv")
    {
        on_toolButtonPaintBrushAdv_clicked();
    }
    else if(m_toolSelected == "stamp")
    {
        on_toolButtonStamp_clicked();
    }
    else if(m_toolSelected == "blur")
    {
        on_toolButtonBlur_clicked();
    }
    else if(m_toolSelected == "eraser")
    {
        on_toolButtonEraser_clicked();
    }
    else if(m_toolSelected == "smudge")
    {
        on_toolButtonSmudge_clicked();
    }
}

void MainWindow::on_toolButtonPointer_clicked()
{
    clearToolpalette();
    m_toolSelected = "pointer";
    ui->toolButtonPointer->setChecked(true);
    m_ptSettingsWidget->setVisible(true);
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(MOUSE_POINTER);
}

void MainWindow::on_actionCrop_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        MOUSE_POINTER->onCrop();
        widget->onSelectionChanged(QRect());
    }
}

void MainWindow::onCrop(const QRect& rect)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        QRect crop = widget->image().rect().intersected(rect);
        widget->setImage(widget->image().copy(crop));
        updateStatusArea(widget->image().width(),widget->image().height());
    }
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

void MainWindow::onPickPrimaryColor(const QPoint& pos)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    {
        const QImage& image = widget->image();
        const QColor& color = image.pixel(pos);

        if(transparentDialog)
        {
            transparentDialog->setColor(color.rgb());
        }
        else
        {
            ui->colorBoxWidget->setPrimaryColor(color);
        }
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

void MainWindow::on_toolButtonWand_clicked()
{
    clearToolpalette();
    m_toolSelected = "wand";
    ui->toolButtonWand->setChecked(true);
    m_magicWandSettingsWidget->setVisible(true);
    onMagicWandSettingsChanged();
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(MAGIC_WAND);
}

void MainWindow::onSelectPrimaryColor(const QPoint& pos, int tolerance, bool color)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        MAGIC_WAND->setSelection(FilterManager::instance()->selectArea(widget->image(), pos, tolerance, color));
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

void MainWindow::onFloodFillPrimaryColor(const QPoint& pos)
{
    const QColor& color = ui->colorBoxWidget->primaryColor();
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(FilterManager::instance()->floodFill(widget->image(), pos, color));
}

void MainWindow::onFloodFillSecondaryColor(const QPoint& pos)
{
    const QColor& color = ui->colorBoxWidget->secondaryColor();
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(FilterManager::instance()->floodFill(widget->image(), pos, color));
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
    m_stampSettingsWidget->setVisible(true);
    onStampSettingsChanged();

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(STAMP_TOOL);
}

void MainWindow::on_toolButtonBlur_clicked()
{
    clearToolpalette();
    m_toolSelected = "blur";
    ui->toolButtonBlur->setChecked(true);
    m_blurSettingsWidget->setVisible(true);
    onBlurSettingsChanged();

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(BLUR_TOOL);

}

void MainWindow::on_toolButtonEraser_clicked()
{
    clearToolpalette();
    m_toolSelected = "eraser";
    ui->toolButtonEraser->setChecked(true);
    m_eraserSettingsWidget->setVisible(true);
    onEraserSettingsChanged();

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(ERASER_TOOL);
}

void MainWindow::on_toolButtonSmudge_clicked()
{
    clearToolpalette();
    m_toolSelected = "smudge";
    ui->toolButtonSmudge->setChecked(true);
    m_smudgeSettingsWidget->setVisible(true);
    onSmudgeSettingsChanged();

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setPaintTool(SMUDGE_TOOL);
}

/*

    | TOOLPALETTE SETTINGS CHANGED |

*/

void MainWindow::onPointerToolSettingsChanged()
{
    MOUSE_POINTER->setStroke(m_ptSettingsWidget->stroke());
    MOUSE_POINTER->setFill(m_ptSettingsWidget->fill());
}

void MainWindow::onPaintBrushSettingsChanged()
{
    PAINT_BRUSH->setWidth(m_pbSettingsWidget->brushWidth());
    PAINT_BRUSH->setAntialiasing(m_pbSettingsWidget->antialiasing());
    PAINT_BRUSH->setCapStyle(m_pbSettingsWidget->brushCapStyle());
    PAINT_BRUSH->setPressure(m_pbSettingsWidget->brushPressure());
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

void MainWindow::onMagicWandSettingsChanged()
{
    MAGIC_WAND->setTolerance(m_magicWandSettingsWidget->tolerance());
    MAGIC_WAND->setColor(m_magicWandSettingsWidget->color());
}

void MainWindow::onStampSettingsChanged()
{
    STAMP_TOOL->setRadius(m_stampSettingsWidget->radius());
    STAMP_TOOL->setPressure(m_stampSettingsWidget->pressure());
    STAMP_TOOL->setStep(m_stampSettingsWidget->step());
    STAMP_TOOL->setFixed(m_stampSettingsWidget->fixed());
    STAMP_TOOL->setPrecise(m_stampSettingsWidget->precise());
    STAMP_TOOL->setDiffuse(m_stampSettingsWidget->diffuse());
}

void MainWindow::onBlurSettingsChanged()
{
    BLUR_TOOL->setRadius(m_blurSettingsWidget->radius());
    BLUR_TOOL->setPressure(m_blurSettingsWidget->pressure());
}

void MainWindow::onEraserSettingsChanged()
{
    ERASER_TOOL->setWidth(m_eraserSettingsWidget->radius());
}

void MainWindow::onSmudgeSettingsChanged()
{
    SMUDGE_TOOL->setRadius(m_smudgeSettingsWidget->radius());
    SMUDGE_TOOL->setPressure(m_smudgeSettingsWidget->pressure());
}

/*

    | WINDOW FUNCTIONS |

*/

void MainWindow::updateStatusArea(int width, int height)
{
    ui->statusBar->removeWidget(imagesizeLbl);
    imagesizeLbl = new QLabel(QString::number(width)+" x "+QString::number(height));
    ui->statusBar->addWidget(imagesizeLbl);
}

void MainWindow::onMultiWindowModeChanged(bool multiWindowMode)
{
    if(multiWindowMode)
    {
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
    }
    else
    {
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    }
}

void MainWindow::onSubWindowActivated(QMdiSubWindow *window)
{
    Q_UNUSED(window)
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        QString scale = QString::number((int)(widget->getScale()*100)).append("%");
        int index = this->zoomCombo->findText(scale);
        if(index < 0)
        {
            this->zoomCombo->setItemText(0, scale);
            this->zoomCombo->setCurrentIndex(0);
        }
        else
        {
            this->zoomCombo->setCurrentIndex(index);
        }
        updateStatusArea(widget->image().width(),widget->image().height());
    }
}

void MainWindow::setWindowSize()
{
    bool maximize = SETTINGS->isMaximizeWindow();
    if (maximize)
    {
        this->setWindowState(Qt::WindowMaximized);
    }
    else
    {
        QRect geometry = SETTINGS->customWindowGeometry();
        if (geometry.isValid())
            this->setGeometry(geometry);
    }
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
        if (!mdiSubWindow->isWindowModified())
            mdiSubWindow->setWindowModified(true);

        ui->actionSave->setEnabled(widget->isUndoEnabled());

        ui->actionUndo->setEnabled(widget->isUndoEnabled());
        ui->actionRedo->setEnabled(widget->isRedoEnabled());
    });
    refreshTools();
}

bool MainWindow::handleCloseChildWindow(QMdiSubWindow *subWindow)
{
    if (!subWindow)
        return false;

    ui->mdiArea->setActiveSubWindow(subWindow);

    if (subWindow->isWindowModified())
    {
        int buttonCode = QMessageBox::question(this, tr("Unsaved Changes"), tr("Save changes before leaving?"),
                                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

        if (buttonCode == QMessageBox::Cancel)
        {
            return true;
        }
        else if (buttonCode == QMessageBox::Yes)
        {
            saveContent();
        }
    }

    subWindow->setWindowModified(false);
    subWindow->close();

    PaintWidget *widget = getCurrentPaintWidget();
    if (!widget) {
        updateStatusArea(0,0);
    }

    return false;
}

bool MainWindow::handleCloseTabs()
{
    QList<QMdiSubWindow *> windows = ui->mdiArea->subWindowList();

    for (int i = 0; i < windows.size(); ++i)
    {
       QMdiSubWindow *subWindow = windows.at(i);
        if(subWindow->isWindowModified())
        {
            if (handleCloseChildWindow(subWindow))
               return true;
        }
        else
        {
           subWindow->close();
           PaintWidget *widget = getCurrentPaintWidget();
           if (!widget) {
               updateStatusArea(0,0);
           }
        }
    }

    return false;
}

void MainWindow::saveGeometryState()
{
    // Save maximized window state if user maximizes the window manually.
    if (this->isMaximized() && !SETTINGS->isMaximizeWindow())
    {
        SETTINGS->setMaximizeWindow(true);
    }
    else if (!SETTINGS->isMaximizeWindow()) // Save custom window geometry.
    {
        SETTINGS->setCustomWindowGeometry(this->geometry());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!handleCloseTabs())
    {
        saveGeometryState();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::createKeyboardShortcuts()
{
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
    ui->actionCopy->setShortcut(QString("Ctrl+C"));
    ui->actionPaste->setShortcut(QString("Ctrl+V"));
    ui->actionUndo->setShortcut(QString("Ctrl+Z"));
    ui->actionRedo->setShortcut(QString("Ctrl+Y"));
    ui->actionImage_properties->setShortcut(QString("Ctrl+J")); //not implemented
    //Image Menu
    ui->actionImage_Size->setShortcut(QString("Ctrl+H"));
    ui->actionCrop->setShortcut(QString("Ctrl+Shift+H")); //broken
    //View Menu
    ui->actionToolpalette->setShortcut(QString("Ctrl+L")); //broken
}

/*

    | CANVAS FUNCTIONS |

*/

PaintWidget *MainWindow::createPaintWidget(const QString &imagePath) const
{
    return new PaintWidget(imagePath);
}

PaintWidget *MainWindow::createPaintWidget(const QSize &imageSize, const QColor &color) const
{
    //QColor color = Qt::white;
    return new PaintWidget(imageSize,color);
}

void MainWindow::addPaintWidget(PaintWidget *widget)
{
    widget->autoScale();

    connect(widget, &PaintWidget::zoomChanged, [this] (float scale) {
        this->zoomCombo->setItemText(0, QString::number((int)(scale*100)).append("%"));
        this->zoomCombo->setCurrentIndex(0);
    });

    connect(widget, &PaintWidget::selectionChanged, this, &MainWindow::onSelectionChanged);

    addChildWindow(widget);
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

/*

    | TOOLBAR ONLY FUNCTIONS |

*/

void MainWindow::onZoomChanged(const QString& scale)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setScale(scale);
}

void MainWindow::on_actionBrightplus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("brightplus");
}

void MainWindow::on_actionBrightminus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("brightminus");
}

void MainWindow::on_actionContrastplus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("contrastplus");
}

void MainWindow::on_actionContrastminus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("contrastminus");
}

void MainWindow::on_actionSaturationplus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("saturationplus");
}

void MainWindow::on_actionSaturationminus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("saturationminus");
}

void MainWindow::on_actionGammaCorrectplus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("gammacorrectplus");
}

void MainWindow::on_actionGammaCorrectminus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("gammacorrectminus");
}

void MainWindow::on_actionGradient_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        GradientDialog dlg(this);
        if(dlg.exec())
        {
                QImage image = widget->image();
                QPoint startPoint;
                QPoint stopPoint;
                switch (dlg.direction()) {
                case N:
                    startPoint.setX(image.width()/2);startPoint.setY(0);
                    stopPoint.setX(image.width()/2);stopPoint.setY(image.height());
                    break;
                case NE:
                    startPoint.setX(image.width());startPoint.setY(0);
                    stopPoint.setX(0);stopPoint.setY(image.height());
                    break;
                case E:
                    startPoint.setX(image.width());startPoint.setY(image.height()/2);
                    stopPoint.setX(0);stopPoint.setY(image.height()/2);
                    break;
                case SE:
                    startPoint.setX(image.width());startPoint.setY(image.height());
                    stopPoint.setX(0);stopPoint.setY(0);
                    break;
                case S:
                    startPoint.setX(image.width()/2);startPoint.setY(image.height());
                    stopPoint.setX(image.width()/2);stopPoint.setY(0);
                    break;
                case SW:
                    startPoint.setX(0);startPoint.setY(image.height());
                    stopPoint.setX(image.width());stopPoint.setY(0);
                    break;
                case W:
                    startPoint.setX(0);startPoint.setY(image.height()/2);
                    stopPoint.setX(image.width());stopPoint.setY(image.height()/2);
                    break;
                case NW:
                    startPoint.setX(0);startPoint.setY(0);
                    stopPoint.setX(image.width());stopPoint.setY(image.height());
                    break;
                }
                widget->setImage(FilterManager::instance()->gradient(image,startPoint,stopPoint,dlg.startColor(),dlg.stopColor()));
        }
    }
}

/*

    | MISC FUNCTIONS |

*/

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
        case QEvent::LanguageChange:
        {
            ui->retranslateUi(this);
        }
        default:
            qt_noop();
    }

    return QObject::eventFilter(obj, e);
}

void MainWindow::showError(const QString &message)
{
    QMessageBox::critical(this, tr("Error"), message);
}

// This method disables actions that are not yet implemented.
void MainWindow::disableUnimplementedActions(bool hide)
{
    if(hide == true)
    {
        foreach(QAction* action, ui->menuEdit->actions())
        { if (action->isSeparator()) ui->menuEdit->removeAction(action);}

        foreach(QAction* action, ui->menuSelection->actions())
        { if (action->isSeparator()) ui->menuSelection->removeAction(action);}

        foreach(QAction* action, ui->menuAdjust->actions())
        { if (action->isSeparator()) ui->menuAdjust->removeAction(action);}

        ui->actionAged_effect->setVisible(false);
        ui->actionAntialiasing->setVisible(false);
        ui->actionAutomatic_Crop->setVisible(true);
        ui->actionAutomatic_transparency->setVisible(false);
        ui->actionBounding_box->setVisible(false);
        ui->actionBright_Contrast->setVisible(false);
        ui->actionCanvas_Size->setVisible(true);
        ui->actionCenter->setVisible(false);
        ui->actionClear->setVisible(false);
        ui->actionColour_balance->setVisible(false);
        ui->actionContract->setVisible(false);
        ui->actionCopy_shape->setVisible(false);
        ui->actionCopyright->setVisible(false);
        ui->actionCrop->setVisible(false);
        ui->actionCut->setVisible(false);
        ui->actionDefine_pattern->setVisible(false);
        ui->actionDithering->setVisible(false);
        ui->actionDuotone->setVisible(false);
        ui->actionDuplicate->setVisible(false);
        ui->actionExpand->setVisible(false);
        ui->actionExport_as_icon->setVisible(false);
        ui->actionFade->setVisible(false);
        ui->actionFill_with_pattern->setVisible(false);
        ui->actionFit_Image->setVisible(false);
        ui->actionFit_ratio->setVisible(false);
        ui->actionGamma_correct->setVisible(false);
        ui->actionHue_Saturation->setVisible(false);
        ui->actionAcquire_image->setVisible(true);
        ui->actionIndexed_Mode->setVisible(true);
        ui->actionInformation->setVisible(false);
        ui->actionInvert->setVisible(false);
        ui->actionLevels->setVisible(false);
        ui->actionLoad_shape->setVisible(false);
        ui->actionManual_settings->setVisible(false);
        ui->actionMore_highlights->setVisible(false);
        ui->actionMore_shadows->setVisible(false);
        ui->actionNegative->setVisible(true);
        ui->actionOptimized_Clipping->setVisible(false);
        ui->actionOther->setVisible(false);
        ui->actionOutside_drop_shadow->setVisible(true);
        ui->actionOptions->setVisible(false);
        ui->actionPaste_and_text_bounding_box->setVisible(false);
        ui->actionPaste_as_new_image->setVisible(false);
        ui->actionPaste_shape->setVisible(false);
        ui->actionPaste_special->setVisible(false);
        ui->actionPosterize->setVisible(false);
        ui->actionPrint->setVisible(true);
        ui->actionPurge->setVisible(false);
        ui->actionRGB_Mode->setVisible(true);
        ui->actionRelief->setVisible(false);
        ui->actionReplace_colour->setVisible(false);
        ui->actionReplace_colour_range->setVisible(false);
        ui->actionSave_shape->setVisible(false);
        ui->actionSelect_all->setVisible(false);
        ui->actionSet_shape->setVisible(false);
        ui->actionSet_wallpaper->setVisible(false);
        ui->actionShow_grid->setVisible(false);
        ui->actionShow_selection->setVisible(true);
        ui->actionSkew->setVisible(false);
        ui->actionSnap_to_grid->setVisible(false);
        ui->actionStroke_and_fill->setVisible(false);
        ui->actionStylize->setVisible(false);
        ui->actionSwap_RGB_channel->setVisible(false);
        ui->actionTexture->setVisible(false);
        ui->actionTransform->setVisible(false);
        ui->actionTransform_2->setVisible(false);
        ui->actionTransparency_mask->setVisible(false);
        ui->actionTransparent_colour->setVisible(true);
        ui->actionValidate->setVisible(false);
    }
    else
    {
        ui->actionAged_effect->setEnabled(false);
        ui->actionAntialiasing->setEnabled(false);
        ui->actionAutomatic_Crop->setEnabled(false);
        ui->actionAutomatic_transparency->setEnabled(false);
        ui->actionBounding_box->setEnabled(false);
        ui->actionBright_Contrast->setEnabled(false);
        ui->actionCanvas_Size->setEnabled(true);
        ui->actionCenter->setEnabled(false);
        ui->actionClear->setEnabled(false);
        ui->actionColour_balance->setEnabled(false);
        ui->actionContract->setEnabled(false);
        ui->actionCopy_shape->setEnabled(false);
        ui->actionCopyright->setEnabled(false);
        ui->actionCrop->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionDefine_pattern->setEnabled(false);
        ui->actionDithering->setEnabled(false);
        ui->actionDuotone->setEnabled(false);
        ui->actionDuplicate->setEnabled(false);
        ui->actionExpand->setEnabled(false);
        ui->actionExport_as_icon->setEnabled(false);
        ui->actionFade->setEnabled(false);
        ui->actionFill_with_pattern->setEnabled(false);
        ui->actionFit_Image->setEnabled(false);
        ui->actionFit_ratio->setEnabled(false);
        ui->actionGamma_correct->setEnabled(false);
        ui->actionHue_Saturation->setEnabled(false);
        ui->actionAcquire_image->setEnabled(true);
        ui->actionIndexed_Mode->setEnabled(true);
        ui->actionInformation->setEnabled(false);
        ui->actionInvert->setEnabled(false);
        ui->actionLevels->setEnabled(false);
        ui->actionLoad_shape->setEnabled(false);
        ui->actionManual_settings->setEnabled(false);
        ui->actionMore_highlights->setEnabled(false);
        ui->actionMore_shadows->setEnabled(false);
        ui->actionNegative->setEnabled(true);
        ui->actionOptimized_Clipping->setEnabled(false);
        ui->actionOther->setEnabled(false);
        ui->actionOutside_drop_shadow->setEnabled(true);
        ui->actionOptions->setEnabled(false);
        ui->actionPaste_and_text_bounding_box->setEnabled(false);
        ui->actionPaste_as_new_image->setEnabled(false);
        ui->actionPaste_shape->setEnabled(false);
        ui->actionPaste_special->setEnabled(false);
        ui->actionPosterize->setEnabled(false);
        ui->actionPrint->setEnabled(true);
        ui->actionPurge->setEnabled(false);
        ui->actionRGB_Mode->setEnabled(true);
        ui->actionRelief->setEnabled(false);
        ui->actionReplace_colour->setEnabled(false);
        ui->actionReplace_colour_range->setEnabled(false);
        ui->actionSave_shape->setEnabled(false);
        ui->actionSelect_all->setEnabled(false);
        ui->actionSet_shape->setEnabled(false);
        ui->actionSet_wallpaper->setVisible(false);
        ui->actionShow_grid->setEnabled(false);
        ui->actionShow_selection->setEnabled(true);
        ui->actionSkew->setEnabled(false);
        ui->actionSnap_to_grid->setEnabled(false);
        ui->actionStroke_and_fill->setEnabled(false);
        ui->actionStylize->setEnabled(false);
        ui->actionSwap_RGB_channel->setEnabled(false);
        ui->actionTexture->setEnabled(false);
        ui->actionTransform->setEnabled(false);
        ui->actionTransform_2->setEnabled(false);
        ui->actionTransparency_mask->setEnabled(false);
        ui->actionTransparent_colour->setEnabled(true);
        ui->actionValidate->setEnabled(false);
    }
}
