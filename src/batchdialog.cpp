#include "batchdialog.h"
#include "ui_batchdialog.h"

#include <QFileDialog>

class BatchDialogPrivate
{
public:
    QStringList fileList;
    QStringList filterList;
    QString outDir;
};

batchDialog::batchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::batchDialog),
    d(new BatchDialogPrivate)
{
    ui->setupUi(this);

    QStringList formats;
    formats << tr("*.png");
    formats << tr("*.jpg");
    formats << tr("*.gif");

    ui->comboBox_2->addItems(formats);
    ui->comboBox_3->addItems(formats);

    QStringList filters;
    filters << tr("Oil");
    filters << tr("Charcoal");
    filters << tr("Swirl");
    filters << tr("Solarize");
    filters << tr("Wave");
    filters << tr("Implode");
    filters << tr("Blur");
    filters << tr("Sharpen");
    filters << tr("Reinforce");
    filters << tr("Grayscale");
    ui->listWidget_2->addItems(filters);
}

batchDialog::~batchDialog()
{
    delete ui;
}

void batchDialog::on_pushButton_clicked()
{
    d->fileList = QFileDialog::getOpenFileNames(this, tr("Select Files"),
                                                    QString(), tr("Image Files (*.png *.jpg *.jpeg *.gif);;All Files (*)"));
    ui->listWidget->clear();
    ui->listWidget->addItems(d->fileList);
}

void batchDialog::on_pushButton_4_clicked()
{
    d->outDir = QFileDialog::getExistingDirectory();
    ui->lineEdit_3->setText(d->outDir);
}

void batchDialog::on_listWidget_2_doubleClicked(const QModelIndex &index)
{
    QListWidgetItem *item = ui->listWidget_2->item(index.row());
    ui->listWidget_3->addItem(item->text());
    d->filterList.append(item->text());
    ui->listWidget_2->removeItemWidget(item);
    delete item;
}

void batchDialog::on_listWidget_3_doubleClicked(const QModelIndex &index)
{
    QListWidgetItem *item = ui->listWidget_3->item(index.row());
    ui->listWidget_2->addItem(item->text());
    ui->listWidget_3->removeItemWidget(item);
    d->filterList.removeOne(item->text());
    delete item;
}

QStringList batchDialog::fileList()
{
    return d->fileList;
}

QStringList batchDialog::filterList()
{
    return d->filterList;
}

QString batchDialog::outDir()
{
    return d->outDir;
}

void batchDialog::on_tabWidget_currentChanged(int index)
{
    if(index == 5)
    {
        ui->plainTextEdit->clear();
        ui->plainTextEdit->insertPlainText("Input files:\n");
        foreach (QString file, d->fileList) {
            ui->plainTextEdit->insertPlainText("  " + file + "\n");
        }
        ui->plainTextEdit->insertPlainText("\n");
        ui->plainTextEdit->insertPlainText("Output folder: " + d->outDir + "\n");
        ui->plainTextEdit->insertPlainText("\n");
        ui->plainTextEdit->insertPlainText("Filters: \n");
        foreach (QString filter, d->filterList) {
            ui->plainTextEdit->insertPlainText("  " + filter + "\n");
        }
    }
}

void batchDialog::onFinished()
{
    ui->tabWidget->setCurrentIndex(5);
    ui->plainTextEdit->insertPlainText("\n");
    ui->plainTextEdit->insertPlainText("Batch processing finished successfully !");
}
