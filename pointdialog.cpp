#include "pointdialog.h"
#include "ui_pointdialog.h"

PointDialog::PointDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointDialog)
{
    ui->setupUi(this);
}

PointDialog::~PointDialog()
{
    delete ui;
}
