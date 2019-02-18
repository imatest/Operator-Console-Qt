#include "waitdialog.h"
#include "ui_waitdialog.h"

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog)
{
    this->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);   // don't diaply close (X) button in title bar
    ui->setupUi(this);
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::start()
{
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    show();
}

void WaitDialog::stop()
{
    QGuiApplication::restoreOverrideCursor();
    hide();
}
