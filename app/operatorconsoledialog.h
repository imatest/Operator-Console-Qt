#ifndef OPERATORCONSOLEDIALOG_H
#define OPERATORCONSOLEDIALOG_H

#include <QDialog>
#include <QFile>
#include <QImage>
#include <QStackedWidget>
#include <QTextStream>
#include "textdialog.h"
#include "qso.h"

class OperatorConsole;

namespace Ui {
class OperatorConsoleDialog;
}

class OperatorConsoleDialog : public QDialog
{
    Q_OBJECT

 public:
    explicit OperatorConsoleDialog(QWidget *parent = nullptr, QFile *out = nullptr, QFile *err = nullptr);
    ~OperatorConsoleDialog();

    void resizeEvent(QResizeEvent *event);

    OperatorConsole *getOpConsole() {return console;}

    //
    // These methods are called from the OperatorConsole object. Perhaps they should be slots,
    // or custom event handlers.  For now they're just methods that get called directly
    //
    void log_message(QString &msg, bool timestamp=true);
    void qsoShow(bool visible); // show or hide the qso widgets
    void save_log(const QString &filename);
    void set_image_size(const QSize &newSize);
    void update_fps(QString &fps);
    void update_image(QImage &image);
    void update_quadrants(const bool *passed);
    void update_status(bool passed, QString &reason);
    void update_summary(QString &summary);

 protected:
    void qsoInit();             // qso is "Quadrant status overlay, the "tic-tac-toe" display overlaid on SFR plus images
    void qsoMove();
    void setupSignals();
    void setupTextDisplays();

    static void moveCenter(QWidget *widget, int centerX, int centerY);  // move a widget so that it's centered at (centerX, centerY)

 private slots:
    void on_btnQuit_clicked();
    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_OperatorConsoleDialog_accepted();
//    void on_ready_read();

    // add slots here for signals that will be sent by OperatorConsole object

    void on_analysisComboBox_currentIndexChanged(const QString &arg1);

private:
    void quit();
    void setupComboBox();
signals:
    void startTest();
    void stopTest();

protected:
    QFile  *m_out;
    QFile  *m_err;

public:
    OperatorConsole *console;
    QStackedWidget *qso[eNumQuadrants];

private:
    Ui::OperatorConsoleDialog *ui;
    static const QColor DEFAULT_IMAGE_BACKGROUND_COLOR;
};

#endif // OPERATORCONSOLEDIALOG_H
