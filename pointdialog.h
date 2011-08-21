#ifndef POINTDIALOG_H
#define POINTDIALOG_H

#include <QDialog>

namespace Ui {
    class PointDialog;
}

class PointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointDialog(QWidget *parent = 0);
    ~PointDialog();

private:
    Ui::PointDialog *ui;
};

#endif // POINTDIALOG_H
