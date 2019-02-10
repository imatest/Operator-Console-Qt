#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QIntValidator>
#include "setup_settings.h"

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    SetupDialog(const setup_settings& input_settings, QWidget *parent= nullptr);   // non-standard constructor
    ~SetupDialog();
    setup_settings &GetSettings() {return m_settings;}

protected:
    void Init();

    int getBitsPerPixel();
    int getBayer();
    int getDeviceID();  // epiphan device id
    void getCamera();
    void setCameraList();

private:
    void ShowOmnivisionElements(void);  //!< This function shows dialog elements that are meant only for Omnivision and hides those meant for Epiphan
    void ShowNormalElements(void);		//!< This function hides dialog elements that are meant only for Omnivision or Epiphan
    void ShowEpiphanElements(void);		//!< This function shows dialog elements that are meant only for Epiphan and hides those meant for Omnivision
    void ShowAllElements(void);			//!< This function shows dialog elements that are meant only for Omnivision or Epiphan
    void ShowDirectShowElements(void);
    void ShowDynamicDeviceElements(const AcquisitionDeviceInfo& device);
    void UpdateVideoFormatDropdown(const AcquisitionDeviceInfo & device);

protected:
    setup_settings m_settings;	//!< this contains settings for the setup dialog including the strings to fill the various CcomboBoxes and CListBoxes
    QIntValidator  m_positive;

private slots:
    void on_okay();
    void on_deviceList_itemSelectionChanged();

    void on_browseRegister_clicked();

    void on_browseIni_clicked();

    void on_browseChart_clicked();

    void on_browseProgram_clicked();

private:
    Ui::SetupDialog *ui;
    void addDynamicDevicesToList();
};

#endif // SETUPDIALOG_H
