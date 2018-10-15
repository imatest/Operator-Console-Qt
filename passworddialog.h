#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class PasswordDialog;
}

class PasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordDialog(QWidget *parent = nullptr);
    ~PasswordDialog();

    const QString &getResponse(void) {return password;}
    bool getUnlockStatus(void) {return unlock;} //!< returns whether the user decided to unlock access to pass/fail settings until the program closes

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PasswordDialog  *ui;
    QString             password;
    bool                unlock;
};

#endif // PASSWORDDIALOG_H
