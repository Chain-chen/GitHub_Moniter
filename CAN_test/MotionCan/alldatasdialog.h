#ifndef ALLDATASDIALOG_H
#define ALLDATASDIALOG_H

#include <QWidget>

namespace Ui {
class AllDatasDialog;
}

class AllDatasDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AllDatasDialog(QWidget *parent = nullptr);
    ~AllDatasDialog();

private:
    Ui::AllDatasDialog *ui;
};

#endif // ALLDATASDIALOG_H
