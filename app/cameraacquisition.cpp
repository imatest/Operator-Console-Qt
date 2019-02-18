#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QCameraViewfinderSettings>
#include <QtGlobal>
#include <QAbstractVideoBuffer>
#include "cameraacquisition.h"

CameraAcquisition::CameraAcquisition() : m_capture(nullptr), m_camera(nullptr), m_open(false), m_error(QCameraImageCapture::NoError)
{
}

CameraAcquisition::CameraAcquisition(QCameraInfo &info) : m_capture(nullptr), m_camera(nullptr), m_info(info), m_open(false), m_error(QCameraImageCapture::NoError)
{
    m_info = info;
}

CameraAcquisition::CameraAcquisition(QString &deviceID) : m_capture(nullptr), m_camera(nullptr), m_deviceID(deviceID), m_open(false), m_error(QCameraImageCapture::NoError)
{
}

CameraAcquisition::~CameraAcquisition()
{
    Close();
}

bool CameraAcquisition::CaptureFrame()
{
    bool success = false;

    if (!m_open)
    {
        Open();
    }

    if (m_capture->isReadyForCapture())
    {
        mutex.lock();
        m_capture->capture();
        waitCondition.wait(&mutex);
        mutex.unlock();
        success = true;
    }

    return success;
}


bool CameraAcquisition::Open(QString &deviceID)
{
    bool    success = false;

    if (NumCameras() == 0)
    {
        m_logMsg = "No built-in cameras were found.";
    }
    else
    {
        m_deviceID = deviceID;
        GetCameraInfo(deviceID);
        success = Open();
    }

    return success;
}


bool CameraAcquisition::Open()
{
    if (m_open)
    {
        Close();    // if camera is already open, close it and start over
    }

    GetCameraInfo();
    m_camera  = new QCamera(m_info);
    m_camera->load();   // this allows us to read camera viewfinder settings

    QList<QCameraViewfinderSettings> list = m_camera->supportedViewfinderSettings();
    m_settings = list[0];   // just use first settings object in list
//    DumpViewfinderSettings(m_settings, 0, true);

    m_capture = new QCameraImageCapture(m_camera);
    m_capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    m_camera->setCaptureMode(QCamera::CaptureMode::CaptureStillImage);
    m_camera->setViewfinderSettings(m_settings);

    SetWidth(m_settings.resolution().width());
    SetHeight(m_settings.resolution().height());

    m_camera->start();

    //
    // Connect the image cpture signals to our slots:
    // image_captured:  not used
    // image_available: copy image data from video frame into m_buf
    // image_saved:     called when an image is saved to a file (should never be called)
    // image_error:     called when an error occurrs capturing an image
    //
    connect(m_capture, &QCameraImageCapture::imageAvailable,  this, &CameraAcquisition::image_available);
    connect(m_capture, QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error), this, &CameraAcquisition::image_error);
    connect(m_capture, &QCameraImageCapture::imageSaved,      this,  &CameraAcquisition::image_saved);
#if 0
    connect(m_capture, &QCameraImageCapture::imageCaptured,   this, &CameraAcquisition::image_captured);
#endif

    m_open = reinterpret_cast<ImageAcquisition *>(this)->Init(m_width, m_height, 4);    // allocates m_buf

    return m_open;
}

bool CameraAcquisition::Close()
{
    if (m_capture != nullptr)
    {
        delete m_capture;
        m_capture = nullptr;
    }

    if (m_camera != nullptr)
    {
        m_camera->unload();
        delete m_camera;
        m_camera = nullptr;
    }

    m_open = false;

    return true;
}

void CameraAcquisition::image_saved(int id, const QString &filename)
{
    qDebug("Image %d saved to %s", id, filename.toStdString().c_str());
}

void CameraAcquisition::image_available(int, const QVideoFrame &frame)
{
    CopyFrame(frame);
    waitCondition.wakeAll();
}

void CameraAcquisition::image_captured(int, const QVideoFrame& /*frame*/)
{
#if 0
    CopyFrame(frame);
    waitCondition.wakeAll();
#endif
}

void CameraAcquisition::image_error(int id, QCameraImageCapture::Error error, const QString &errorString)
{
    m_error = error;

    qDebug("Capture error: id %d  error %d  %s", id, error, errorString.toStdString().c_str());
}

bool CameraAcquisition::GetCameraInfo(QString &deviceName)
{
    QList<QCameraInfo>  cameras = QCameraInfo::availableCameras();
    int                 i;

    for (i = 0; i < cameras.count(); ++i)
    {
        if (cameras[i].deviceName() == deviceName)
        {
            m_info = cameras[i];
            break;
        }
    }

    return i < cameras.count();
}

bool CameraAcquisition::GetCameraInfo()
{
    if (m_info.isNull())
    {
        GetCameraInfo(0);
    }

    return m_info.isNull();
}

bool CameraAcquisition::GetCameraInfo(int index)
{
    bool                success = false;
    QList<QCameraInfo>  cameras = QCameraInfo::availableCameras();

    if (index >= 0 && index < cameras.count())
    {
        m_info = cameras[index];
        success = true;
    }

    return success;
}


int CameraAcquisition::NumCameras()
{
    QList<QCameraInfo>  cameras = QCameraInfo::availableCameras();

    return cameras.count();
}

void CameraAcquisition::ViewfinderDebug(QCamera *camera)
{
    QList<QCameraViewfinderSettings> list = camera->supportedViewfinderSettings();

    int cnt = list.count();

    if (cnt)
    {
        for (int i = 0 ; i < cnt; i++)
        {
            DumpViewfinderSettings(list[i], i, i == 0);
        }
    }
}

void CameraAcquisition::DumpViewfinderSettings(const QCameraViewfinderSettings &settings, int index, bool header)
{
    QSize size     = settings.resolution();
    QSize aspect   = settings.pixelAspectRatio();
    qreal frameMin = settings.minimumFrameRate();
    qreal frameMax = settings.maximumFrameRate();
    int format     = static_cast<int>(settings.pixelFormat());

    if (header)
    {
        qDebug("viewfinder      size      aspect ratio   pixel format frame rate min  frame rate max");
    }

    qDebug("    %2d      (%4d, %4d)  (%4d, %4d)        %2d           %.1f            %.1f", index, size.width(), size.height(), aspect.width(), aspect.height(), format, frameMin, frameMax);
}


void CameraAcquisition::imageFromVideoFrame(const QVideoFrame& buffer)
{
    QVideoFrame frame(buffer);  // make a copy we can call map (non-const) on
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
    // BUT the frame.pixelFormat() is QVideoFrame::Format_Jpeg, and this is
    // mapped to QImage::Format_Invalid by
    // QVideoFrame::imageFormatFromPixelFormat
    if (imageFormat != QImage::Format_Invalid)
    {
        int width = frame.width();
        int height = frame.height();
        int bytesPerLine = frame.bytesPerLine();
        m_imageFrame = QImage(frame.bits(), width, height, bytesPerLine, imageFormat);
    }
    else
    {
        // e.g. JPEG
        int nbytes = frame.mappedBytes();
        m_imageFrame = QImage::fromData(frame.bits(), nbytes);
    }
    frame.unmap();
}


void  CameraAcquisition::CopyFrame(const QVideoFrame &constFrame)
{
    QVideoFrame frame(constFrame);  // make a non-const copy so we can call map() to get the pixels into app memory
    frame.map(QAbstractVideoBuffer::ReadOnly);

    int width         = frame.width();
    int height        = frame.height();
    int bytesPerLine  = frame.bytesPerLine();
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());

    //
    // If the image format is RGB32 or ARGB32, we can just copy the buffer directly.
    // Othwerwise we convert it to a QImage and then copy the buffer from the QImage.
    //
    if (imageFormat == QImage::Format_ARGB32 || imageFormat == QImage::Format_RGB32)
    {
        const uchar *bits = frame.bits();

        memcpy(m_buf, bits, m_numBytes);
    }
    else
    {
        //
        // Create a QImage from the frame in its existing format, convert
        // the image to RGB32, and copy the image data into our buffer.
        //
        QImage image(frame.bits(), width, height, bytesPerLine, imageFormat);   // create an image from the frame
        QImage rgbImage = image.convertToFormat(QImage::Format_RGB32);          // convert to RGB
        const uchar *bits = image.bits();                                       // get a pointer to the data
        memcpy(m_buf, bits, m_numBytes);                                        // copy the image data into our buffer
    }

    frame.unmap();
}

void CameraAcquisition::GetCameraList(QMap<QString, QSize> &list)
{
    //
    // for each available camera
    //   get its info
    //   load the camera
    //   get list of viewfinders
    //   get width and height of first viewfinder
    //   store camera deviceName and resolution in m_cameraList (deviceName is not an actual human-readable name)
    //
    QList<QCameraInfo>  cameras = QCameraInfo::availableCameras();

    for (int i = 0; i < cameras.count(); ++i)
    {
        QCamera camera(cameras[i]);
        camera.load();   // this allows us to read camera viewfinder settings
        QList<QCameraViewfinderSettings> settings = camera.supportedViewfinderSettings();
        list.insert(cameras[i].deviceName(), settings[0].resolution());
        camera.unload();
    }
}
