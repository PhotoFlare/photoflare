#include "plugindialog.h"
#include "ui_plugindialog.h"

#include <QListWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>

#include "../plugins/PluginManager.h"

PluginDialog::PluginDialog(PluginManager* manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginDialog),
    m_manager(manager)
{
    ui->setupUi(this);

    // Shrink the list to make room for path label + buttons at the bottom
    ui->listWidget->setGeometry(10, 10, 361, 260);
    ui->buttonBox->setGeometry(10, 340, 361, 32);

    // Plugins folder label
    const QString pluginsPath = m_manager && !m_manager->scannedPaths().isEmpty()
        ? QDir::toNativeSeparators(m_manager->scannedPaths().first())
        : QString();

    auto* pathLabel = new QLabel(tr("Plugins folder:  ") + pluginsPath, this);
    pathLabel->setGeometry(10, 278, 361, 16);
    pathLabel->setWordWrap(false);
    pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // Open Folder + Rescan buttons
    auto* openBtn = new QPushButton(tr("Open Folder"), this);
    openBtn->setGeometry(10, 298, 100, 26);
    connect(openBtn, &QPushButton::clicked, this, &PluginDialog::onOpenFolder);

    auto* rescanBtn = new QPushButton(tr("Rescan"), this);
    rescanBtn->setGeometry(118, 298, 80, 26);
    connect(rescanBtn, &QPushButton::clicked, this, &PluginDialog::onRescan);

    setFixedSize(size());

    // Relabel buttons
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Apply"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->listWidget, &QListWidget::itemSelectionChanged,
            this, &PluginDialog::onSelectionChanged);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem*) { onApply(); });
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &PluginDialog::onApply);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &QDialog::reject);

    populateList();
}

PluginDialog::~PluginDialog()
{
    delete ui;
}

void PluginDialog::populateList()
{
    ui->listWidget->clear();
    m_plugins.clear();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    auto addDisabled = [&](const QString& text) {
        auto* item = new QListWidgetItem(text, ui->listWidget);
        item->setFlags(Qt::NoItemFlags);
        m_plugins << nullptr;
    };

    if (!m_manager || m_manager->allPlugins().isEmpty()) {
        const QStringList errors = m_manager ? m_manager->loadErrors() : QStringList();
        if (!errors.isEmpty()) {
            addDisabled(tr("── Load errors ──"));
            for (const QString& err : errors)
                addDisabled(err);
        } else {
            addDisabled(tr("No plugins found. Place plugin DLL files in the"));
            addDisabled(tr("folder shown below, then click Rescan."));
        }
        return;
    }

    for (IPhotoflarePlugin* p : m_manager->allPlugins()) {
        QString typeLabel;
        switch (p->type()) {
            case PluginType::Filter:   typeLabel = tr("Filter");   break;
            case PluginType::Exporter: typeLabel = tr("Exporter"); break;
        }
        const QString text = QString("%1  [%2]  v%3\n%4")
            .arg(p->displayName(), typeLabel, p->version(), p->author());
        auto* item = new QListWidgetItem(text, ui->listWidget);
        item->setSizeHint(QSize(item->sizeHint().width(), 50));
        m_plugins << p;
    }
}

void PluginDialog::onRescan()
{
    if (!m_manager) return;
    m_manager->unloadAll();
    m_manager->scanDirectories(m_manager->scannedPaths());
    populateList();
}

void PluginDialog::onOpenFolder()
{
    if (!m_manager || m_manager->scannedPaths().isEmpty()) return;
    const QString path = m_manager->scannedPaths().first();
    // Create the folder if it doesn't exist so Explorer can open it
    QDir().mkpath(path);
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void PluginDialog::onSelectionChanged()
{
    const int row = ui->listWidget->currentRow();
    const bool isFilter = row >= 0
                          && row < m_plugins.size()
                          && m_plugins[row] != nullptr
                          && m_plugins[row]->type() == PluginType::Filter;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isFilter);
}

void PluginDialog::onApply()
{
    const int row = ui->listWidget->currentRow();
    if (row < 0 || row >= m_plugins.size()) return;
    if (m_plugins[row] == nullptr) return;
    if (m_plugins[row]->type() != PluginType::Filter) return;
    emit filterRequested(static_cast<IFilterPlugin*>(m_plugins[row]));
    accept();
}

