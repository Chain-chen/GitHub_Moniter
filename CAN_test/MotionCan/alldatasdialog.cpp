#include "alldatasdialog.h"
#include "ui_alldatasdialog.h"

AllDatasDialog::AllDatasDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllDatasDialog)
{
    ui->setupUi(this);
}

AllDatasDialog::~AllDatasDialog()
{
    delete ui;
}
