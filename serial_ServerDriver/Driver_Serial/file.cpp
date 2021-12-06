#include "file.h"
#include "ui_file.h"
#include <QDebug>

File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);


}

File::~File()
{
    delete ui;
}
