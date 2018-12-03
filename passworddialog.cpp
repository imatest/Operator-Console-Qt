#include "passworddialog.h"
#include "ui_passworddialog.h"

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    unlock = false;
    ui->setupUi(this);
    ui->password->setFocus();
}

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

void PasswordDialog::on_buttonBox_accepted()
{
    password = ui->password->text();
    unlock   = ui->checkBox->checkState() == Qt::CheckState::Checked;
}
