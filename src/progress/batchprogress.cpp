/*
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

#include "batchprogress.h"
#include "ui_batchprogress.h"

BatchProgress::BatchProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchProgress)
{
    ui->setupUi(this);
}

BatchProgress::~BatchProgress()
{
    delete ui;
}

void BatchProgress::progress(int value,int total)
{
    ui->progressBar->setValue(int(100 * (float)value/(float)total));
    if(value == total) 
        hide();
}

void BatchProgress::on_hideButton_clicked()
{
    hide();
}
