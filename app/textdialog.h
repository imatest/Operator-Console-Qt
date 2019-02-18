#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

namespace Ui {
class TextDialog;
}

class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(QWidget *parent = nullptr);
    ~TextDialog();

    void setText(QString &text);
    void setTitle(QString &title);

private:
    Ui::TextDialog *ui;
};

#endif // TEXTDIALOG_H
