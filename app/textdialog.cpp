#include "textdialog.h"
#include "ui_textdialog.h"

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

TextDialog::~TextDialog()
{
    delete ui;
}

void TextDialog::setText(QString &text)
{
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(text);
}

void TextDialog::setTitle(QString &title)
{
    setWindowTitle(title);
}
