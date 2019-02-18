#ifndef PASSFAILDIALOG_H
#define PASSFAILDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "passfailsettings.h"

namespace Ui {
class PassFailDialog;
}

class PassFailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PassFailDialog(PassFailSettings& settings, QWidget *parent = nullptr);
    ~PassFailDialog();

    PassFailSettings& GetSettings() {return m_settings;}

protected:
    template <typename T> bool AddSubEntry(QTreeWidgetItem *parent, entry<T>& ent);
    template <typename T> bool AddSubMinMaxEntry(QTreeWidgetItem *parent, minMaxEntry<T>& ent);
    template <typename T> bool AddSubVecEntry(QTreeWidgetItem *parent, vecEntry<T>& ent);

    template <typename T> void UpdateEntry(entry<T>& ent,       QTreeWidgetItem *item);
    template <typename T> void UpdateEntry(minMaxEntry<T>& ent, QTreeWidgetItem *parent);
    template <typename T> void UpdateEntry(vecEntry<T>& ent,    QTreeWidgetItem *parent);

    void Cleanup();
    void Init();
    void UpdateBlemish(QTreeWidgetItem *blemish);
    void UpdateSfrplus(QTreeWidgetItem *sfrplus);
    void UpdateOis(QTreeWidgetItem *ois);
    void UpdateOther(QTreeWidgetItem *other);

    QString value_to_str(long    num) {return QString::number(num);}
    QString value_to_str(double  num) {return QString::number(num);}
    QString value_to_str(QString num) {return num;}
    QString value_to_str(std::string num) {return QString(num.c_str());}

    void str_to_value(QString str, long    &value) {value = str.toLong();}
    void str_to_value(QString str, double  &value) {value = str.toDouble();}
    void str_to_value(QString str, QString &value) {value = str;}
    void str_to_value(QString str, std::string &value) {value = str.toStdString();}


protected:
    PassFailSettings    m_settings;

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous); // called when selected item changes

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);  // called when item text changes (i.e. when edited)

    void on_buttonBox_accepted();

private:
    Ui::PassFailDialog *ui;
};

#endif // PASSFAILDIALOG_H
