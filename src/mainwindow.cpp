﻿/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

// Main window class for the Photoflare application.

//#include <QDebug>

#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QThread>
#include <QMimeData>
#include <QDesktopServices>
#include <QTimer>
#include <QImageReader>
#include <QInputDialog>
#include <QSysInfo>

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

#include "Settings.h"
#include "managers/ToolManager.h"
#include "managers/FilterManager.h"

#include "widgets/PaintWidget.h"
#include "dialogs/NewDialog.h"
#include "dialogs/aboutdialog.h"
#include "dialogs/textdialog.h"
#include "dialogs/prefsdialog.h"
#include "dialogs/gradientdialog.h"
#include "dialogs/imagepropertiesdialog.h"
#include "dialogs/outerframedialog.h"
#include "dialogs/dropshadowdialog.h"
#include "dialogs/huedialog.h"
#include "dialogs/compressiondialog.h"
#include "dialogs/transparentdialog.h"
#include "dialogs/batchdialog.h"
#include "dialogs/plugindialog.h"
#include "dialogs/checkupdatedialog.h"

#include "progress/batchprogress.h"
#include "workers/BatchProcessWorker.h"
#include "workers/filterworker.h"
#include "workers/filterworkermp.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::MainWindow() :
    QMainWindow(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add Settings Widgets to the Dock
    addSettingsWidgets();

    // Connect signals to the various Tools
    connectTools();

    // Setup additional workspace UI after parsing ui file
    setupWorkspace();

    // Disable actions that are not yet implemented. True = hidden, False = Disabled
    disableUnimplementedActions(true);

    // Create the keyboard shortcut bindings
    createKeyboardShortcuts();

    // Setup some other defaults on startup
    setWindowSize();

    // Update recents
    updateRecentFilesMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*

    | Initial GUI Setup |

*/

void MainWindow::setupWorkspace()
{
    // Add zoom ComboBox
    addZoomCombo();

    // Set window mode
    if(SETTINGS->isMultiWindowMode())
    {
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView);
    }
    else
    {
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    }

    // Set dock layout
    if(SETTINGS->getDockLayout() == "1")
    {
        addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidget_palette);
        addDockWidget(Qt::LeftDockWidgetArea, ui->dockWidgetSettings);
    }

    // Center colorBox in tool palette.
    ui->verticalLayout->setAlignment(ui->colorBoxWidget, Qt::AlignCenter);

    // Default the transparent dialog
    transparentDialog = 0;

    // Default the colour mode
    ui->actionRGB_Mode->setChecked(true);

    // Remove Qt contextmenu from the toolbars
    setContextMenuPolicy(Qt::NoContextMenu);

    // Setup status text defaults
    batchLbl = new QLabel(tr("Ready"));
    batchLbl->setStyleSheet("margin:0 100 0 0");
    ui->statusBar->addWidget(batchLbl);

    // Disable undo/redo buttons on startup
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    // Set checked by default
    ui->actionFilterbar->setChecked(true);
    ui->actionToolpalette->setChecked(true);

    // Pointer tool selected by default
    on_toolButtonPointer_clicked();
}

void MainWindow::addZoomCombo()
{
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
}

void MainWindow::connectTools()
{
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

    ERASER_TOOL->setSecondaryColor(ui->colorBoxWidget->secondaryColor());
    QObject::connect(ui->colorBoxWidget, &ColorBoxWidget::secondaryColorChanged, ERASER_TOOL, &EraserTool::setSecondaryColor);

    // Connect PointerTool signals
    QObject::connect(MOUSE_POINTER, SIGNAL(crop(const QRect&)), this, SLOT(onCrop(const QRect&)));
    QObject::connect(MOUSE_POINTER, SIGNAL(save()), this, SLOT(on_actionSave_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(saveAs()), this, SLOT(on_actionSave_As_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(close()), this, SLOT(on_actionClose_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(copy()), this, SLOT(on_actionCopy_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(paste()), this, SLOT(on_actionPaste_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(imageSize()), this, SLOT(on_actionImage_Size_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(canvasSize()), this, SLOT(on_actionCanvas_Size_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(undo()), this, SLOT(on_actionUndo_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(redo()), this, SLOT(on_actionRedo_triggered()));
    QObject::connect(MOUSE_POINTER, SIGNAL(showhotspots()), this, SLOT(onShowHotspotsTriggered()));

    // Setup signals for more Tools
    QObject::connect(COLOUR_PICKER, SIGNAL(pickPrimaryColor(const QPoint&)), this, SLOT(onPickPrimaryColor(const QPoint&)));
    QObject::connect(COLOUR_PICKER, SIGNAL(pickSecondaryColor(const QPoint&)), this, SLOT(onPickSecondaryColor(const QPoint&)));

    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillPrimaryColor(const QPoint&)), this, SLOT(onFloodFillPrimaryColor(const QPoint&)));
    QObject::connect(PAINT_BUCKET, SIGNAL(floodFillSecondaryColor(const QPoint&)), this, SLOT(onFloodFillSecondaryColor(const QPoint&)));

    QObject::connect(TEXT_TOOL, SIGNAL(editTextFinished()), this, SLOT(onTextToolFinished()));
    QObject::connect(TEXT_TOOL, SIGNAL(editText(const QString&,const QFont&, const QColor&)), this, SLOT(onEditText(const QString&,const QFont&, const QColor&)));

    QObject::connect(MAGIC_WAND, SIGNAL(selectPrimaryColor(const QPoint&,int,bool)), this, SLOT(onSelectPrimaryColor(const QPoint&,int,bool)));

    QObject::connect(SETTINGS, SIGNAL(multiWindowModeChanged(bool)), this, SLOT(onMultiWindowModeChanged(bool)));
}

void MainWindow::addSettingsWidgets()
{
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
}

/*

    | FILTER WORKER |

*/

void MainWindow::onImageFiltered(QImage image)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImage(image);

    batchLbl->setText(tr("Ready"));
}

void MainWindow::applyThreadedFilter(QString filterName, double dV)
{
    QThread *thread = new QThread();
    FilterWorker *worker = new FilterWorker();
    worker->setFilter(filterName);

    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    worker->setImage(widget->image());
    worker->setDoubleVal(dV);
    worker->setParent(this);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(filterProcessFinished(QImage)), this, SLOT(onImageFiltered(QImage)));
    thread->start();

    batchLbl->setText(tr("Working..."));
}

void MainWindow::applyThreadedFilterMP(QString filterName, double dV)
{
    QThread *thread = new QThread();
    FilterWorkerMP *worker = new FilterWorkerMP();
    worker->setFilter(filterName);

    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    worker->setImage(widget->image());
    worker->setDoubleVal(dV);
    worker->setParent(this);
    worker->moveToThread(thread);

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(filterProcessFinished(QImage)), this, SLOT(onImageFiltered(QImage)));
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
        if(dialog.backgroundColorEnabled() == true)
        {
            addPaintWidget(createPaintWidget(dialog.newImageSize(),dialog.newImageBackgroundColor()));
        }
        else if(dialog.backgroundColorEnabled() == false){
            addPaintWidget(createPaintWidget(dialog.newImageSize(),Qt::transparent));
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    const QStringList& fileName = QFileDialog::getOpenFileNames(this, tr("Open File"),
    SETTINGS->getOpenFolder(),
    tr("Image Files")+
    "(*.png *.PNG *.jpg *.jpeg *.JPG *.JPEG *.gif *.GIF *.tif *.tiff *.TIF *.TIFF *.bmp *.BMP *.ico *.ICO *.pbm *.PBM *.pgm *.PGM *.ppm *.PPM);;"

    "PNG(*.png *.PNG);;"
    "JPEG(*.jpg *.jpeg *.JPG *.JPEG);;"
    "GIF(*.gif *.GIF);;"
    "TIFF(*.tif *.tiff *.TIF *.TIFF);;"
    "BMP(*.bmp *.BMP);;"
    "ICO(*.ico *.ICO);;"
    "PBM(*.pbm *.PBM);;"
    "PGM(*.pgm *.PGM);;"
    "PPM(*.ppm *.PPM);;"
    +tr("All Files")+"(*)");
/*
 *  Previously supported RAW formats
    "ARW (*.arw *.srf *.sr2);;"
    "BAY (*.bay);;"
    "CR2 (*.crw *.cr2);;"
    "DCS (*.dcs *.dcr *.drf *.k25 *.kdc);;"
    "MOS (*.mos);;"
    "NEF (*.nef *.nrw);;"
    "RAW (*.raw *.rw2)"));
*/

    if(fileName.length()>0)
    {
        for(int i=0;i<fileName.length();i++)
        {
            openFile(fileName[i]);
        }

        if(SETTINGS->getPreviouslyOpened() == true)
        {
            QDir d = QFileInfo(fileName[0]).absoluteDir();
            SETTINGS->setOpenFolder(d.absolutePath());
        }
    }
}

bool MainWindow::fileTypeSupported(QList<QByteArray> formats, QString ext)
{
    bool status = false;
    for(int i=0;i<formats.length();i++)
    {
        if(formats[i] == ext)
        {
            status = true;
            break;
        }
    }
    return status;
}

QString MainWindow::prepareFile(const QString& fileName)
{
    QString newFileName = fileName;
    QFileInfo info(fileName);
    QImageReader reader(fileName);
    reader.setDecideFormatFromContent(true); // Autodetect file type without depending on extension

    if(!fileTypeSupported(reader.supportedImageFormats(),reader.format()))
    {
        newFileName = "";
        showError(tr("Please open a valid image file"));
    }

    return newFileName;
}

void MainWindow::openFile(const QString& fileName)
{
    if(!fileExists(fileName))
    {
        showError(tr("Image does not exist at this file path"));
    }
    else
    {
        QString updatedFileName = prepareFile(fileName);

        if (!updatedFileName.isEmpty())
        {
            addPaintWidget(createPaintWidget(updatedFileName));
            SETTINGS->addRecentFile(updatedFileName);
            updateRecentFilesMenu();
        }
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

void MainWindow::updateRecentFilesMenu()
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
                openFile(fileName);
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
        QString currentFileName = ui->mdiArea->currentSubWindow()->windowTitle();
        QString suffix = QFileInfo(currentFileName).suffix();

        // Setup file filters
        QStringList filters;
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

        // Get fileName from the dialog
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
            SETTINGS->getSaveFolder(), filters.join(";;"), &defaultFilter);

        // Escape if no fileName
        if (fileName.isEmpty())
            return;

        // Set Save path to previously used location
        if(SETTINGS->getPreviouslyOpenedSave() == true)
        {
            QDir d = QFileInfo(fileName).absoluteDir();
            SETTINGS->setSaveFolder(d.absolutePath());
        }

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
        // Default image quality 100 percent
        int quality = -1;

        // Compression dialog to set the quality
        if(SETTINGS->getCompressionDialogEnabled() && (fileNameSuffix == "jpg" || fileNameSuffix == "jpeg"))
        {
            CompressionDialog dlg;
            dlg.exec();

            // Dialog Accepted
            if(dlg.enableSaveImage)
            {
                quality = dlg.quality();
            }
        }
        // Save image with the selected quality value
        if (saveImage(fileName, quality))
        {
            PaintWidget *widget = getCurrentPaintWidget();
            if (widget)
                widget->setImage(QImage(fileName));

            // Update tab label
            ui->mdiArea->currentSubWindow()->setWindowModified(false);
            ui->mdiArea->currentSubWindow()->setWindowTitle(fileName + " [*]");

            // Update image path so the file properties dialog will show all file details
            QDir d = QFileInfo(fileName).absoluteDir();
            widget->setImagePath(d.absolutePath());

            // Update recents
            SETTINGS->addRecentFile(fileName);
            updateRecentFilesMenu();
        }
        else
            showError(tr("Unable to save image."));
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

    PaintWidget *widget = getCurrentPaintWidget();

    return widget ? widget->image().save(fileName,nullptr,quality) : false;
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
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        QPrinter printer;

        QPrintDialog *dialog = new QPrintDialog(&printer);
        if (dialog->exec() == QDialog::Accepted)
        {
            QPainter painter;
            if (!painter.begin(&printer))
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

            QImageReader reader(widget->imagePath());
            dialog.setFileType(reader.format());
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
        QImage cpy = widget->selection().isEmpty()
            ? widget->image()
            : widget->image().copy(widget->selection().boundingRect());
        clipboard->setImage(cpy);
    }
}

void MainWindow::onPaste()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        QClipboard *clipboard = QApplication::clipboard();
        on_toolButtonPointer_clicked();
        MOUSE_POINTER->setOverlayImage(clipboard->image());
    }
    else
    {
        on_actionPaste_as_new_image_triggered();
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

void MainWindow::on_actionPaste_as_new_image_triggered()
{
    on_actionCopy_triggered();  
    QClipboard *clipboard = QApplication::clipboard();
    if(clipboard->mimeData()->hasImage())
    {
        addPaintWidget(createPaintWidget(clipboard->image().size(),Qt::white));
        PaintWidget *widget = getCurrentPaintWidget();
        if (widget)
        {
            widget->setImage(clipboard->image());
        }
    }
}

void MainWindow::on_actionClearHistory_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    {
        widget->clearUndoHistory();
        ui->actionUndo->setEnabled(widget->isUndoEnabled());
        ui->actionRedo->setEnabled(widget->isRedoEnabled());
    }
}

void MainWindow::on_actionClearClipboard_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
}

/*

    | IMAGE MENU |

*/

void MainWindow::on_actionRGB_Mode_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    {
        ui->actionIndexed_Mode->setChecked(false);
        widget->setImageModeIndexed(false);
    }
    else {
        ui->actionRGB_Mode->setChecked(false);
    }
}

void MainWindow::on_actionIndexed_Mode_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    {
        ui->actionRGB_Mode->setChecked(false);
        widget->setImageModeIndexed(true);
    }
    else {
        ui->actionIndexed_Mode->setChecked(false);
    }
}

void MainWindow::on_actionDuplicate_triggered()
{
    PaintWidget *oldwidget = getCurrentPaintWidget();
    if (oldwidget)
    {
        addPaintWidget(createPaintWidget(oldwidget->image().size(),Qt::white));
        PaintWidget *widget = getCurrentPaintWidget();
        if (widget)
        {
            widget->setImage(oldwidget->image());
        }
    }
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

void MainWindow::on_actionFree_Rotate_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        bool ok;
        double d = QInputDialog::getDouble(this, tr("Set Rotate Degrees"),
                                               tr("Degrees:"), 1.00, 0.0, 360.00, 0, &ok, Qt::WindowFlags(), 1.0);
        if(ok)
        {
            applyThreadedFilter("rotate", d);
        }
    }
}

void MainWindow::on_actionImage_Size_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (!widget)
        return;

    NewDialog dialog;
    dialog.setWindowTitle(tr("Resize Image"));
    dialog.setMode(NewDialog::ResizeImage);
    dialog.setImageSize(widget->image().size());
    if (dialog.exec() == QDialog::Accepted)
    {
        widget->setImage(widget->image().scaled(dialog.newImageSize()));
        updateStatusArea(dialog.newImageSize().width(),dialog.newImageSize().height());
    }
}

void MainWindow::on_actionCanvas_Size_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (!widget)
        return;

    NewDialog dialog;
    dialog.setWindowTitle(tr("Resize Canvas"));
    dialog.setMode(NewDialog::ResizeCanvas);
    dialog.setImageSize(widget->image().size());
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
        // Set the previous tool so we can revert back to it after finishing transparent colour operation
        m_previousToolSelected = m_toolSelected;

        origImage = widget->image();
        on_toolButtonDropper_clicked();
        transparentDialog = new TransparentDialog();
        QObject::connect(transparentDialog, SIGNAL(previewTransparent(QColor,int)), this, SLOT(onPreviewTransparent(QColor,int)));
        QObject::connect(transparentDialog, SIGNAL(dialogFinished(int)), this, SLOT(onTransparentFinished()));
        QObject::connect(transparentDialog, SIGNAL(dialogAccepted()), this, SLOT(onTransparentAccepted()));
        QObject::connect(transparentDialog, SIGNAL(dialogRejected()), this, SLOT(onTransparentRejected()));
        transparentDialog->setColor(widget->image().pixel(0,0));
        transparentDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        transparentDialog->show();
    }
}

void MainWindow::onTransparentFinished()
{
    transparentDialog = 0;
    // Restore previous tool
    m_toolSelected = m_previousToolSelected;
    QTimer::singleShot(1000, this, SLOT(refreshTools()));
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
        widget->setImageOriginal(origImage);
}

void MainWindow::onPreviewTransparent(QColor color, int tolerance)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
        widget->setImageOriginal(FilterManager::instance()->floodFillOpacity(origImage, color, tolerance));
}

void MainWindow::onTextToolFinished()
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
    {
        TEXT_TOOL->setText(dialog.text(), dialog.font(), dialog.color(), dialog.antialias(), dialog.position());
    }
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

void MainWindow::on_actionSelect_all_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->selectAll();
    }
}

void MainWindow::onSelectionChanged(bool visible)
{
    ui->actionCrop->setEnabled(ui->actionShow_selection->isChecked() && visible);
    ui->actionShow_selection->setChecked(visible);
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
    {
        bool ok;
        double d = QInputDialog::getDouble(this, tr("Set Image Opacity"),
                                               tr("Opacity:"), 0.5, 0.0, 1.0, 2, &ok, Qt::WindowFlags(), 0.1);
        if(ok)
        {
            applyThreadedFilter("setOpacity", d);
        }
    }
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
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("dustreduction");
        }
        else
        {
            applyThreadedFilterMP("dustreduction");
        }
    }
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
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("impulseNoise");
        }
        else
        {
            applyThreadedFilterMP("impulseNoise");
        }
    }
}

void MainWindow::on_actionLaplacian_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("laplacianNoise");
        }
        else
        {
            applyThreadedFilterMP("laplacianNoise");
        }
    }
}

void MainWindow::on_actionPoisson_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("poissonNoise");
        }
        else
        {
            applyThreadedFilterMP("poissonNoise");
        }
    }
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

void MainWindow::on_actionColour_Threshold_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    applyThreadedFilter("colourthreshold");
}

void MainWindow::on_actionOil_Paint_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("oil");
        }
        else
        {
            applyThreadedFilterMP("oil");
        }
    }
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
    {
        ui->toolBar->show();
    }
}

void MainWindow::on_actionToolpalette_triggered()
{
    if(ui->dockWidget_palette->isVisible())
    {
        ui->dockWidget_palette->hide();
        ui->dockWidgetSettings->hide();
    }
    else
    {
        ui->dockWidget_palette->show();
        ui->dockWidgetSettings->show();
    }
}

void MainWindow::on_actionShow_grid_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        bool enabled = widget->isGridEnabled();
        // Only show cell width dialog if we are about to show the grid
        if(!enabled)
        {
            bool ok;
            int i = QInputDialog::getInt(this, tr("Set Grid size"),
                                                   tr("Cell width"), 1, 1.0, 999, 1, &ok);
            if(ok)
            {
                widget->showGrid(i);
                ui->actionShow_grid->setChecked(true);
            }
            else
            {
                ui->actionShow_grid->setChecked(false);
            }
        }
        else
        {
            widget->showGrid(1);
            ui->actionShow_grid->setChecked(false);
        }
    }
    else
    {
        ui->actionShow_grid->setChecked(false);
    }
}

void MainWindow::getPrevZoomFromScale(QString scaletext)
{
    int count = this->zoomCombo->count();
    for(int i=count;i>0;i--)
    {
        int a = this->zoomCombo->itemText(i-1).replace("%","").toInt();
        int b = scaletext.replace("%","").toInt();

        if((a != 0) && a < b)
        {
            this->zoomCombo->setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::getNextZoomFromScale(QString scaletext)
{
    int count = this->zoomCombo->count();
    for(int i=1;i<count;i++)
    {
        int a = this->zoomCombo->itemText(i).replace("%","").toInt();
        int b = scaletext.replace("%","").toInt();

        if((a != 0) && a > b)
        {
            this->zoomCombo->setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::on_actionZoom_in_triggered()
{
    if(this->zoomCombo->currentIndex()==0)
    {
        getNextZoomFromScale(this->zoomCombo->currentText());
    }
    else
    {
        if(this->zoomCombo->currentIndex()+1 < this->zoomCombo->count() && this->zoomCombo->currentIndex()>0)
        {
            this->zoomCombo->setCurrentIndex(this->zoomCombo->currentIndex()+1);
        }

        if(this->zoomCombo->currentText() == "")
        {
            this->zoomCombo->setCurrentIndex(this->zoomCombo->currentIndex()+1);
        }
    }

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->setScale(this->zoomCombo->currentText());
    }
}

void MainWindow::on_actionZoom_out_triggered()
{
    if(this->zoomCombo->currentIndex()==0)
    {
        getPrevZoomFromScale(this->zoomCombo->currentText());
    }

    if(this->zoomCombo->currentIndex()-1 > 1)
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
        this->zoomCombo->setCurrentIndex(9);
        widget->setScale(this->zoomCombo->currentText());
    }
}

void MainWindow::on_actionFull_screen_triggered()
{
    if(this->isFullScreen())
    {
        this->setWindowState(Qt::WindowMaximized);
        ui->actionFull_screen->setChecked(false);
    }
    else
    {
        this->showFullScreen();
        ui->actionFull_screen->setChecked(true);
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
        this->setWindowTitle(QString("Photoflare - %1/%2 (%3%)").arg(index).arg(total).arg(int(100 * static_cast<float>(index)/static_cast<float>(total))));
    }
    else
    {
        this->setWindowTitle(QString("Photoflare"));
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    prefsDialog = new PrefsDialog();
    QObject::connect(prefsDialog, SIGNAL(safeQuitApp()), this, SLOT(onSafeQuitApp()));
    prefsDialog->show();
}

void MainWindow::onSafeQuitApp()
{
    on_actionQuit_triggered();
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
    QString link = "https://photoflare.io/donate/";
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
        widget->onSelectionChanged(QPolygon());
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
        // Make sure we are inside the image area
        if(pos.x() >= 0 && pos.x() < image.width() && pos.y() >= 0 && pos.y() < image.height())
        {
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
}

void MainWindow::onPickSecondaryColor(const QPoint& pos)
{
    PaintWidget *widget = getCurrentPaintWidget();
    if(widget)
    {
        const QImage& image = widget->image();
        // Make sure we are inside the image area
        if(pos.x() >= 0 && pos.x() < image.width() && pos.y() >= 0 && pos.y() < image.height())
        {
            const QColor& color = image.pixel(pos);
            ui->colorBoxWidget->setSecondaryColor(color);
        }
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
    PAINT_BRUSH_ADV->setPressure(m_pbAdvSettingsWidget->pressure());
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

void MainWindow::clearStatusArea()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (!widget) {
        updateStatusArea(0,0);
    }
}

void MainWindow::updateStatusArea(int width, int height)
{
    ui->statusBar->removeWidget(imagesizeLbl);
    imagesizeLbl = new QLabel(QString::number(width)+" x "+QString::number(height));
    if(width == 0 && height == 0)
    {
        imagesizeLbl = new QLabel(QString());
    }
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
    Q_UNUSED(window);
    // Set the previously selected tool as active
    refreshTools();

    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        QString scale = QString::number(static_cast<int>(widget->getScale()*100)).append("%");
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
        ui->actionUndo->setEnabled(widget->isUndoEnabled());
        ui->actionRedo->setEnabled(widget->isRedoEnabled());

        // Set view grid checked status
        if(widget->isGridEnabled())
        {
            ui->actionShow_grid->setChecked(true);
        }
        else
        {
            ui->actionShow_grid->setChecked(false);
        }

        // Set view toolbar checked status
        if(ui->toolBar->isVisible())
        {
            ui->actionFilterbar->setChecked(true);
        }
        else
        {
            ui->actionFilterbar->setChecked(false);
        }

        // Set view toolPalette checked status
        if(ui->dockWidget_palette->isVisible())
        {
            ui->actionToolpalette->setChecked(true);
        }
        else
        {
            ui->actionToolpalette->setChecked(false);
        }
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

    // Clear Undo History just before we close the tab
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->clearUndoHistory();
        ui->actionUndo->setEnabled(widget->isUndoEnabled());
        ui->actionRedo->setEnabled(widget->isRedoEnabled());
    }
    subWindow->close();

    clearStatusArea();

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
           qCritical() << "Close event";qCritical() << "Close event";
        }
    }

    return false;
}

void MainWindow::saveGeometryState()
{
    // Save maximized window state if user maximizes the window manually.
    if (this->isMaximized())
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
    ui->actionSave_As->setShortcut(QString("Ctrl+Shift+S"));
    ui->actionPrint->setShortcut(QString("Ctrl+P"));
    ui->actionClose->setShortcut(QString("Ctrl+Shift+W"));
    ui->actionQuit->setShortcut(QString("Ctrl+Q"));
    //Edit Menu
    ui->actionCut->setShortcut(QString("Ctrl+X")); //not implemented
    ui->actionCopy->setShortcut(QString("Ctrl+C"));
    ui->actionPaste->setShortcut(QString("Ctrl+V"));
    ui->actionPaste_as_new_image->setShortcut(QString("Ctrl+Shift+V"));
    ui->actionUndo->setShortcut(QString("Ctrl+Z"));
    ui->actionRedo->setShortcut(QString("Ctrl+Y"));
    ui->actionImage_properties->setShortcut(QString("Ctrl+J"));
    //Image Menu
    ui->actionDuplicate->setShortcut(QString("Ctrl+U"));
    ui->actionImage_Size->setShortcut(QString("Ctrl+H"));
    ui->actionCrop->setShortcut(QString("Ctrl+Shift+H"));
    // Selection Menu
    ui->actionSelect_all->setShortcut(QString("Ctrl+A"));
    //View Menu
    ui->actionToolpalette->setShortcut(QString("Ctrl+L"));
    ui->actionZoom_in->setShortcut(QKeySequence::ZoomIn);
    ui->actionZoom_out->setShortcut(QKeySequence::ZoomOut);
    ui->actionOriginal_size->setShortcut(QString("Ctrl+0"));
    ui->actionFull_screen->setShortcut(QString("Ctrl+F"));
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
    return new PaintWidget(imageSize,color);
}

void MainWindow::addPaintWidget(PaintWidget *widget)
{
    widget->autoScale();

    connect(widget, &PaintWidget::zoomChanged, [this] (float scale) {
        this->zoomCombo->setItemText(0, QString::number(static_cast<int>(scale*100)).append("%"));
        this->zoomCombo->setCurrentIndex(0);
    });
    connect(widget, &PaintWidget::selectionChanged, this, &MainWindow::onSelectionChanged);
    addChildWindow(widget);
}

PaintWidget* MainWindow::getCurrentPaintWidget()
{
    PaintWidget *widget = nullptr;
    if(ui->mdiArea->currentSubWindow())
    {
        widget = static_cast<PaintWidget *>(ui->mdiArea->currentSubWindow()->widget());
    }
    return widget;
}

void MainWindow::onShowHotspotsTriggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        widget->setHotspotVisble(true);
    }
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
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("contrastplus");
        }
        else
        {
            applyThreadedFilterMP("contrastplus");
        }
    }
}

void MainWindow::on_actionContrastminus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("contrastminus");
        }
        else
        {
            applyThreadedFilterMP("contrastminus");
        }
    }
}

void MainWindow::on_actionSaturationplus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("saturationplus");
        }
        else
        {
            applyThreadedFilterMP("saturationplus");
        }
    }
}

void MainWindow::on_actionSaturationminus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("saturationminus");
        }
        else
        {
            applyThreadedFilterMP("saturationminus");
        }
    }
}

void MainWindow::on_actionGammaCorrectplus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("gammacorrectplus");
        }
        else
        {
            applyThreadedFilterMP("gammacorrectplus");
        }
    }
}

void MainWindow::on_actionGammaCorrectminus_triggered()
{
    PaintWidget *widget = getCurrentPaintWidget();
    if (widget)
    {
        if(QSysInfo::productType() == "windows")
        {
            applyThreadedFilter("gammacorrectminus");
        }
        else
        {
            applyThreadedFilterMP("gammacorrectminus");
        }
    }
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
                widget->setImage(FilterManager::instance()->gradient(image, startPoint, stopPoint, dlg.startColor(), dlg.stopColor()));
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
        ui->actionAutomatic_transparency->setVisible(false);
        ui->actionBounding_box->setVisible(false);
        ui->actionBright_Contrast->setVisible(false);
        ui->actionCenter->setVisible(false);
        ui->actionClear->setVisible(false);
        ui->actionColour_balance->setVisible(false);
        ui->actionContract->setVisible(false);
        ui->actionCopy_shape->setVisible(false);
        ui->actionCopyright->setVisible(false);
        ui->actionCut->setVisible(false);
        ui->actionDefine_pattern->setVisible(false);
        ui->actionDithering->setVisible(false);
        ui->actionDuotone->setVisible(false);
        ui->actionExpand->setVisible(false);
        ui->actionExport_as_icon->setVisible(false);
        ui->actionFade->setVisible(false);
        ui->actionFill_with_pattern->setVisible(false);
        ui->actionFit_Image->setVisible(false);
        ui->actionFit_ratio->setVisible(false);
        ui->actionGamma_correct->setVisible(false);
        ui->actionHue_Saturation->setVisible(false);
        ui->actionInformation->setVisible(false);
        ui->actionInvert->setVisible(false);
        ui->actionLevels->setVisible(false);
        ui->actionLoad_shape->setVisible(false);
        ui->actionManual_settings->setVisible(false);
        ui->actionMore_highlights->setVisible(false);
        ui->actionMore_shadows->setVisible(false);
        ui->actionOptimized_Clipping->setVisible(false);
        ui->actionOther->setVisible(false);
        ui->actionOptions->setVisible(false);
        ui->actionPaste_and_text_bounding_box->setVisible(false);
        ui->actionPaste_shape->setVisible(false);
        ui->actionPaste_special->setVisible(false);
        ui->actionPosterize->setVisible(false);
        ui->actionRelief->setVisible(false);
        ui->actionReplace_colour->setVisible(false);
        ui->actionReplace_colour_range->setVisible(false);
        ui->actionSave_shape->setVisible(false);
        ui->actionSelect_all->setVisible(true);
        ui->actionSet_shape->setVisible(false);
        ui->actionSet_wallpaper->setVisible(false);
        ui->actionSkew->setVisible(false);
        ui->actionSnap_to_grid->setVisible(false);
        ui->actionStroke_and_fill->setVisible(false);
        ui->actionStylize->setVisible(false);
        ui->actionSwap_RGB_channel->setVisible(false);
        ui->actionTexture->setVisible(false);
        ui->actionTransform->setVisible(false);
        ui->actionTransform_2->setVisible(false);
        ui->actionTransparency_mask->setVisible(false);
        ui->actionValidate->setVisible(false);
    }
    else
    {
        ui->actionAged_effect->setEnabled(false);
        ui->actionAntialiasing->setEnabled(false);
        ui->actionAutomatic_transparency->setEnabled(false);
        ui->actionBounding_box->setEnabled(false);
        ui->actionBright_Contrast->setEnabled(false);
        ui->actionCenter->setEnabled(false);
        ui->actionClear->setEnabled(false);
        ui->actionColour_balance->setEnabled(false);
        ui->actionContract->setEnabled(false);
        ui->actionCopy_shape->setEnabled(false);
        ui->actionCopyright->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionDefine_pattern->setEnabled(false);
        ui->actionDithering->setEnabled(false);
        ui->actionDuotone->setEnabled(false);
        ui->actionExpand->setEnabled(false);
        ui->actionExport_as_icon->setEnabled(false);
        ui->actionFade->setEnabled(false);
        ui->actionFill_with_pattern->setEnabled(false);
        ui->actionFit_Image->setEnabled(false);
        ui->actionFit_ratio->setEnabled(false);
        ui->actionGamma_correct->setEnabled(false);
        ui->actionHue_Saturation->setEnabled(false);
        ui->actionInformation->setEnabled(false);
        ui->actionInvert->setEnabled(false);
        ui->actionLevels->setEnabled(false);
        ui->actionLoad_shape->setEnabled(false);
        ui->actionManual_settings->setEnabled(false);
        ui->actionMore_highlights->setEnabled(false);
        ui->actionMore_shadows->setEnabled(false);
        ui->actionOptimized_Clipping->setEnabled(false);
        ui->actionOther->setEnabled(false);
        ui->actionOptions->setEnabled(false);
        ui->actionPaste_and_text_bounding_box->setEnabled(false);
        ui->actionPaste_shape->setEnabled(false);
        ui->actionPaste_special->setEnabled(false);
        ui->actionPosterize->setEnabled(false);
        ui->actionRelief->setEnabled(false);
        ui->actionReplace_colour->setEnabled(false);
        ui->actionReplace_colour_range->setEnabled(false);
        ui->actionSave_shape->setEnabled(false);
        ui->actionSet_shape->setEnabled(false);
        ui->actionSet_wallpaper->setVisible(false);
        ui->actionSkew->setEnabled(false);
        ui->actionSnap_to_grid->setEnabled(false);
        ui->actionStroke_and_fill->setEnabled(false);
        ui->actionStylize->setEnabled(false);
        ui->actionSwap_RGB_channel->setEnabled(false);
        ui->actionTexture->setEnabled(false);
        ui->actionTransform->setEnabled(false);
        ui->actionTransform_2->setEnabled(false);
        ui->actionTransparency_mask->setEnabled(false);
        ui->actionValidate->setEnabled(false);
    }
}
