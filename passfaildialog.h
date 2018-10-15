#ifndef PASSFAILDIALOG_H
#define PASSFAILDIALOG_H

#include <QDialog>

namespace Ui {
class PassFailDialog;
}

class PassFailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PassFailDialog(QWidget *parent = nullptr);
    ~PassFailDialog();

private:
    Ui::PassFailDialog *ui;
};

#endif // PASSFAILDIALOG_H
