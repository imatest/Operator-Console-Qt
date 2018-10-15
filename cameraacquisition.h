#ifndef QCAMERAACQUISITION_H
#define QCAMERAACQUISITION_H

#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QMutex>
#include <QObject>
#include <QCameraViewfinderSettings>
#include <QWaitCondition>
#include "imageacquisition.h"

class CameraAcquisition : public ImageAcquisition
{
    Q_OBJECT

public:
    CameraAcquisition();
    CameraAcquisition(QCameraInfo &info);
    CameraAcquisition(QString &deviceID);   // this is a non-human-readable string
    ~CameraAcquisition();

    bool	CaptureFrame();
    bool	Close();
    QString &DeviceID() {return m_deviceID;}
    int     NumCameras();
    bool	Open();
    bool    Open(QString &deviceID);

    static void ViewfinderDebug(QCamera *camera);

protected:
    void        CopyFrame(const QVideoFrame &constFrame);
    static void DumpViewfinderSettings(const QCameraViewfinderSettings &settings, int index, bool header);
    bool        GetCameraInfo(QString &deviceID);
    bool        GetCameraInfo(int index);
    bool        GetCameraInfo();
    void        imageFromVideoFrame(const QVideoFrame& buffer);

private slots :
    void image_error(int id, QCameraImageCapture::Error error, const QString &errorString);
    void image_available(int id, const QVideoFrame &frame);
    void image_captured(int id, const QVideoFrame &frame);
    void image_saved(int id, const QString &fileName);

protected:
    QCameraImageCapture         *m_capture;
    QCamera                     *m_camera;
    QString                     m_deviceID;
    QCameraInfo                 m_info;
    bool                        m_open;
    QWaitCondition              waitCondition;
    QMutex                      mutex;
    QCameraViewfinderSettings   m_settings;
    QCameraViewfinder           m_viewfinder;
    QCameraImageCapture::Error  m_error;
    QImage                      m_imageFrame;
};

#endif // QCAMERAACQUISITION_H
