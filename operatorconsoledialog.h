#ifndef OPERATORCONSOLEDIALOG_H
#define OPERATORCONSOLEDIALOG_H

#include <QDialog>
#include <QImage>
#include <QStackedWidget>
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
    explicit OperatorConsoleDialog(QWidget *parent = nullptr);
    ~OperatorConsoleDialog();

    void resizeEvent(QResizeEvent *event);

    OperatorConsole *getOpConsole() {return console;}

    //
    // These methods are called from the OperatorConsole object. Perhaps they should be slots,
    // or custom event handlers.  For now they're just methods that get called directly
    //
    void qsoShow(bool visible); // show or hide the qso widgets
    void set_image_size(const QSize &newSize);
    void update_fps(QString &fps);
    void update_image(QImage &image);
    void update_quadrants(const bool *passed);
    void update_status(bool passed, QString &reason);
    void update_summary(QString &summary);
    void log_message(QString &msg, bool timestamp=true);

 protected:
    void calcImageSize();
    void qsoInit();             // qso is "Quadrant status overlay, the "tic-tac-toe" display overlaid on SFR plus images
    void qsoMove();
    void setupSignals();

    static void moveCenter(QWidget *widget, int centerX, int centerY);  // move a widget so that it's centered at (centerX, centerY)

 private slots:
    void on_btnStart_clicked();
    void on_btnStop_clicked();

    // add slots here for signals that will be sent by OperatorConsole object


signals:
    void startTest();
    void stopTest();

 protected:
public:
    OperatorConsole *console;
    QStackedWidget *qso[eNumQuadrants];

private:
    Ui::OperatorConsoleDialog *ui;
};

#endif // OPERATORCONSOLEDIALOG_H
