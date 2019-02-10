#include <QFileDialog>
#include <QIntValidator>
#include "imatestsourceids.h"
#include "setupdialog.h"
#include "ui_setupdialog.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->defaultPage);
}

SetupDialog::SetupDialog(const setup_settings& input_settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    m_positive.setBottom(0);    // validator that allows all positive ints
    m_settings = input_settings;

    ui->setupUi(this);
    ui->width->setValidator(&m_positive);
    ui->height->setValidator(&m_positive);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SetupDialog::on_okay);

    Init();
}

SetupDialog::~SetupDialog()
{
    delete ui;
}

void SetupDialog::addDynamicDevicesToList()
{
    for (auto iDevice = m_settings.device_infos.begin(); iDevice != m_settings.device_infos.end(); ++iDevice){
        if (iDevice->isValid() && !iDevice->m_deviceName.isEmpty()){
            ui->deviceList->addItem(iDevice->m_deviceName);
        }
    }
}

void SetupDialog::Init()
{
    int index;
    QString width, height, str;
    ui->bitdepth->hide();
    ui->deviceID->hidePopup();

    width.setNum(m_settings.width);
    height.setNum(m_settings.height);

    ui->width->setText(width);
    ui->height->setText(height);

    // fill the list of devices
    for (int i = 0; i < m_settings.device_list.size(); ++i)
    {
        ui->deviceList->addItem(m_settings.device_list[i]);
    }

    // Add the dynamically detected devices to the device list
    addDynamicDevicesToList();

    // fill the list of Bayer patterns
    for (int i = 0; i < m_settings.bayer_list.size(); ++i)
    {
        ui->bayer->addItem(m_settings.bayer_list[i]);
    }

    // fill the list of allowed bits per pixel (Omnivision only)
    for (int i = 0; i < m_settings.allowed_bits_per_pixel.size(); ++i)
    {
        str.setNum(m_settings.allowed_bits_per_pixel[i]);
        ui->bitdepth->addItem(str);
    }

    // fill the combo-box of Epiphan device IDs
    for ( int i = 0; i < m_settings.epiphan_deviceID_list.size(); ++i)
    {
        str = m_settings.epiphan_deviceID_list[i];
        ui->deviceID->addItem(str);
    }

    // fill the combo box with QCamera device descriptions (i.e. names)
    setCameraList();


#if 0
    // file the combo-box of DirectShow device names
   for (int i = 0; i < m_settings.directshow_device_names.size(); ++i)
   {
       str = m_settings.directshow_device_names[i];
       ui->qcamera->addItem(str);
   }

   if (m_settings.directshow_device_names.size() > 0)
      ui->qcamera->setCurrentIndex(m_settings.directshow_deviceID);
#endif

    //
    // Select the stored value for the device and hide/show objects accordingly
    //
    if (m_settings.sourceID >= 0)
    {
        switch (m_settings.sourceID)
        {
        case SOURCE_Aptina:
            index = EDevIndex::eAptina; // Aptina DevWare
            ShowNormalElements();
            break;
        case SOURCE_Omnivision:
            index = EDevIndex::eOmnivision; // Omnivision OVTA
            ShowOmnivisionElements();
            break;
        case SOURCE_Toshiba:
            index = EDevIndex::eToshiba; // Toshiba ImaTuning
            ShowNormalElements();
            break;
        case SOURCE_STM:
            index = EDevIndex::eSTM; // STM Conduit
            ShowNormalElements();
            break;
        case SOURCE_Graphin:
            index = EDevIndex::eGraphin; // Graphin EasyLab
            ShowNormalElements();
            break;
        case SOURCE_Epiphan:
            index = EDevIndex::eEpiphan; // Epiphan
            ShowEpiphanElements();
            break;
        case SOURCE_OpConsoleDirectShow:
            index = EDevIndex::eQCamera; // Qt Camera interface
            ShowDirectShowElements();
            break;
        case SOURCE_File:
            index = EDevIndex::eLoadFile; // Load an image file
            ShowNormalElements();
            break;
        default:
            if (m_settings.sourceID >= 128 && m_settings.device_infos.size() > 0) {
                index = EDevIndex::eAptina;
                int sourceID = m_settings.sourceID;
                auto selection = std::find_if(m_settings.device_infos.begin(), m_settings.device_infos.end(), [sourceID](AcquisitionDeviceInfo& x) {return x.m_deviceID == sourceID; });
                if (selection != m_settings.device_infos.end()) {

                    auto pList = ui->deviceList;
                    auto results = pList->findItems(selection->m_deviceName, Qt::MatchFixedString);
                    if (results.size() > 0) {
                        // Select the first item with a matching name
                        index = pList->row(*results.begin());
                    }

                    ShowDynamicDeviceElements(*selection);


                }
            } else {
                index = EDevIndex::eAptina; // Default: Aptina DevWare
                ShowNormalElements();
            }
        }

        ui->deviceList->setCurrentRow(index,QItemSelectionModel::Select);
    }

    if (m_settings.bayer > 0) // select the Bayer pattern item using the stored value
    {
        ui->bayer->setCurrentIndex(m_settings.bayer-1);
    }
    else
    {
        ui->bayer->setCurrentIndex(0);
        m_settings.bayer = 1;
    }

    if ( m_settings.bits_per_pixel > 0) // select the bits per pixel item using the stored value
    {
        str.setNum(m_settings.bits_per_pixel);

        int index = ui->bitdepth->findText(str);

        if (index != -1)
        {
            ui->bitdepth->setCurrentIndex(index);
        }
        else
        {
            ui->bitdepth->setCurrentIndex(0);
        }
    }
    else
    {
        ui->bitdepth->setCurrentIndex(0);
        m_settings.bits_per_pixel = 8;
    }

    // enable (or disable) the various edit-browse controls
    str.setNum(m_settings.epiphan_deviceID);
    index = ui->deviceID->findText(str);
    ui->deviceID->setCurrentIndex(index);

    ui->pathRegister->setText(m_settings.omnivision_reg_file);
    ui->pathIni->setText(m_settings.ini_file);

    if (!m_settings.chart_def_file.isEmpty())
    {
        ui->pathChartDef->setText(m_settings.chart_def_file);
    }

    ui->pathProgram->setEnabled(false);   // the program path is a read-only display for now
    ui->browseProgram->hide();            // hide the browse button, since the progam path field is read-only
    ui->pathProgram->setText(m_settings.program_path);

    ui->pathChartDef->setText(m_settings.chart_def_file);

    // load the part number and serial number into their respective edit boxes
    ui->partNumber->setText(m_settings.part_number);
    ui->serialNumber->setText(m_settings.serial_number);
}

void SetupDialog::ShowNormalElements(void)
{
    ui->bitdepth->hide();
    ui->labelBitdepth->hide();

    ui->deviceID->hide();
    ui->labeldeviceID->hide();

    ui->bayer->hide();
    ui->labelBayer->hide();

    ui->pathRegister->hide();
    ui->browseRegister->hide();
    ui->labelRegister->hide();

    ui->qcamera->hide();
    ui->labelQcamera->hide();

    ui->width->show();
    ui->height->show();
    ui->labelWidth->show();
    ui->labelHeight->show();

    ui->stackedWidget->setCurrentWidget(ui->defaultPage);
}

void SetupDialog::ShowDynamicDeviceElements(const AcquisitionDeviceInfo &device){

    UpdateVideoFormatDropdown(device);

    ui->stackedWidget->setCurrentWidget(ui->dynamicDevicePage);

}

void SetupDialog::UpdateVideoFormatDropdown(const AcquisitionDeviceInfo &device){

   QComboBox* pCombobox = ui->videoFormatComboBox;

   // Clear the combobox
   pCombobox->clear();

   // Populate with video formats for 'device'
   for (auto iFormat = device.m_supportedFormats.begin(); iFormat != device.m_supportedFormats.end(); ++iFormat)
       pCombobox->addItem(*iFormat);

   // Determine the appropriate selection for initial display
   QString formatString;
   if (m_settings.video_format.isEmpty()) {
       // Set the selection to the default video format
       formatString = device.m_defaultFormat;
   }
   else {
       formatString = m_settings.video_format;
   }

   int index = pCombobox->findText(formatString);
   index = index >= 0? index : 0;
   pCombobox->setCurrentIndex(index);

}

//
// This function shows the dialog elements that are meant only
// for Epiphan and hides those meant only for Omnivision
//
void SetupDialog::ShowEpiphanElements(void)
{
    ui->bitdepth->hide();
    ui->labelBitdepth->hide();

    ui->deviceID->show();
    ui->labeldeviceID->show();

    ui->bayer->hide();
    ui->labelBayer->hide();

    ui->pathRegister->hide();
    ui->browseRegister->hide();
    ui->labelRegister->hide();

    ui->qcamera->hide();
    ui->labelQcamera->hide();

    ui->width->show();
    ui->height->show();
    ui->labelWidth->show();
    ui->labelHeight->show();

    ui->stackedWidget->setCurrentWidget(ui->defaultPage);
}

//
// This function shows the dialog elements that are meant only
// for Epiphan and hides those meant only for Omnivision
//
void SetupDialog::ShowDirectShowElements(void)
{
    ui->bitdepth->hide();
    ui->labelBitdepth->hide();

    ui->deviceID->hide();
    ui->labeldeviceID->hide();

    ui->bayer->hide();
    ui->labelBayer->hide();

    ui->pathRegister->hide();
    ui->browseRegister->hide();
    ui->labelRegister->hide();

    ui->qcamera->show();
    ui->labelQcamera->show();

    ui->width->hide();
    ui->height->hide();
    ui->labelWidth->hide();
    ui->labelHeight->hide();

    ui->stackedWidget->setCurrentWidget(ui->defaultPage);
}

//
// This function shows the dialog elements that are meant only
// for Omnivision and hides those meant only for Epiphan
//
void SetupDialog::ShowOmnivisionElements(void)
{
    ui->bitdepth->show();
    ui->labelBitdepth->show();

    ui->deviceID->hide();
    ui->labeldeviceID->hide();

    ui->bayer->show();
    ui->labelBayer->show();

    ui->pathRegister->show();
    ui->browseRegister->show();
    ui->labelRegister->show();

    ui->qcamera->hide();
    ui->labelQcamera->hide();

    ui->width->show();
    ui->height->show();
    ui->labelWidth->show();
    ui->labelHeight->show();

    ui->stackedWidget->setCurrentWidget(ui->defaultPage);
}

//
// This function shows all the dialog elements that should
// only be shown for either Omnivision or Epiphan
//
void SetupDialog::ShowAllElements(void)
{
    ui->bitdepth->show();
    ui->labelBitdepth->show();

    ui->deviceID->show();
    ui->labeldeviceID->show();

    ui->bayer->show();
    ui->labelBayer->show();

    ui->pathRegister->show();
    ui->browseRegister->show();
    ui->labelRegister->show();

    ui->qcamera->show();
    ui->labelQcamera->show();

    ui->width->show();
    ui->height->show();
    ui->labelWidth->show();
    ui->labelHeight->show();

    ui->stackedWidget->setCurrentWidget(ui->defaultPage);
}

void SetupDialog::on_okay()
{
    QString str;
    // copy widget settings into appropriate setup fields

    m_settings.bits_per_pixel      = getBitsPerPixel();
    m_settings.epiphan_deviceID    = getDeviceID();
    m_settings.width               = ui->width->text().toInt();
    m_settings.height              = ui->height->text().toInt();
    m_settings.bayer               = getBayer();
//    m_settings.directshow_deviceID = ui->qcamera->currentIndex();
    m_settings.omnivision_reg_file = ui->pathRegister->text();
    m_settings.part_number         = ui->partNumber->text();
    m_settings.serial_number       = ui->serialNumber->text();
    m_settings.ini_file            = ui->pathIni->text();
    m_settings.chart_def_file      = ui->pathChartDef->text();
    m_settings.program_path        = ui->pathProgram->text();

    getCamera();
}


void SetupDialog::on_deviceList_itemSelectionChanged()
{
    QString str(ui->deviceList->currentItem()->text());

    if (str.compare(m_settings.device_list[0]) ==0) // Aptina DevWare
    {
        m_settings.sourceID = SOURCE_Aptina;
        ShowNormalElements();
    }
    else if (str.compare(m_settings.device_list[1]) ==0) // Omnivision OVTA
    {
        m_settings.sourceID = SOURCE_Omnivision;
        ShowOmnivisionElements();
    }
    else if (str.compare(m_settings.device_list[2]) ==0) // Toshiba ImaTuning
    {
        m_settings.sourceID = SOURCE_Toshiba;
        ShowNormalElements();
    }
    else if (str.compare(m_settings.device_list[3]) ==0) // STM Conduit
    {
        m_settings.sourceID = SOURCE_STM;
        ShowNormalElements();
    }
    else if (str.compare(m_settings.device_list[4]) ==0) // Graphin EasyLab
    {
        m_settings.sourceID = SOURCE_Graphin;
        ShowNormalElements();
    }
    else if (str.compare(m_settings.device_list[5]) ==0) // Epiphan
    {
        m_settings.sourceID = SOURCE_Epiphan;
        ShowEpiphanElements();
    }
    else if (str.compare(m_settings.device_list[6]) ==0) // DirectShow camera
    {
        m_settings.sourceID = SOURCE_OpConsoleDirectShow;
        ShowDirectShowElements();
    }
   else if (str.compare(m_settings.device_list[7]) ==0) // Image File
    {
        m_settings.sourceID = SOURCE_File;
        ShowNormalElements();
    }
    else
    {
        auto selection = std::find_if(m_settings.device_infos.begin(), m_settings.device_infos.end(), [str](AcquisitionDeviceInfo x) { return str.compare(x.m_deviceName) == 0; });
        if (selection != m_settings.device_infos.end()) {
            m_settings.sourceID = selection->m_deviceID;
            ShowDynamicDeviceElements(*selection);
        }
        else {
            m_settings.sourceID = SOURCE_Aptina;
            fprintf(stderr, "Error: Unknown device selection. The program will revert to Aptina DevWare");
            ShowNormalElements();
        }

    }
}

// add other message handlers and data functions from MFC version here

int SetupDialog::getBitsPerPixel()
{
    QString str;
    int value= ui->bitdepth->currentText().toInt();

#if 0
    //
    // if the value is invalid, set it to the default of 8
    //
    QVector<int>::iterator itr = std::find(m_settings.allowed_bits_per_pixel.begin(), m_settings.allowed_bits_per_pixel.end(), value);

    if ( itr == m_settings.allowed_bits_per_pixel.end())
    {
        value = 8;
    }
#endif

    return value;
}

int SetupDialog::getBayer()
{
    QString str = ui->bayer->currentText();
    int bayer = 0;

    if (str.compare(m_settings.bayer_list[0], Qt::CaseInsensitive) == 0) // Red in R1C1 (RG/GB)
    {
        bayer = 1;
    }
    else if (str.compare(m_settings.bayer_list[1], Qt::CaseInsensitive) == 0) // Red in R1C2 (GR/BG)
    {
        bayer = 2;
    }
    else if (str.compare(m_settings.bayer_list[2], Qt::CaseInsensitive) == 0) // Red in R2C1 (GB/RG)
    {
        bayer = 3;
    }
    else if (str.compare(m_settings.bayer_list[3], Qt::CaseInsensitive) == 0) // Red in R2C2 (BG/GR)
    {
        bayer = 4;
    }
    else
    {
        m_settings.bayer = 0;
    }

    return bayer;
}

int SetupDialog::getDeviceID()
{
    int value = ui->deviceID->currentData().toInt();

    return value;
}

void SetupDialog::on_browseRegister_clicked()
{

}

void SetupDialog::on_browseIni_clicked()
{
    //
    // Get the ini file
    //
    QString filename = QFileDialog::getOpenFileName(this, "Select inifile", m_settings.ini_file, "Imatest ini Files (*.ini)");

    if (!filename.isNull())
    {
        m_settings.ini_file = filename;
    }
}

void SetupDialog::on_browseChart_clicked()
{
    //
    // Get the arbitrary charts definition file (do these have an extension?)
    //
    QString filename = QFileDialog::getOpenFileName(this, "Select chart definition file", m_settings.chart_def_file, nullptr);

    if (!filename.isNull())
    {
        ui->pathChartDef->setText(filename);
        m_settings.ini_file = filename;
    }
}

void SetupDialog::on_browseProgram_clicked()
{
#if 0
    //
    // Get the path to the program directory
    //
    QString filename = QFileDialog::getOpenFileName(this, nullptr, m_settings.program_path, nullptr);

    if (!filename.isNull())
    {
        m_settings.program_path = filename;
    }
#endif
}

void SetupDialog::setCameraList()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    for (int i = 0; i < cameras.count(); i++)
    {
         ui->qcamera->addItem(cameras[i].description());

         if (!m_settings.qcam_deviceID.isEmpty() && cameras[i].deviceName() == m_settings.qcam_deviceID)
         {
            ui->qcamera->setCurrentIndex(i);
         }
     }
}

void SetupDialog::getCamera()
{
    int index = ui->qcamera->currentIndex();
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();

    m_settings.qcam_deviceID = cameras[index].deviceName();
    m_settings.qcam_description = cameras[index].description();
}

void SetupDialog::on_videoFormatComboBox_currentIndexChanged(const QString &selectedFormat)
{
    m_settings.video_format = selectedFormat;
}
