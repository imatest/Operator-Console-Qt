#include "passfaildialog.h"
#include "ui_passfaildialog.h"

PassFailDialog::PassFailDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PassFailDialog)
{
    ui->setupUi(this);
}

PassFailDialog::~PassFailDialog()
{
    delete ui;
}
