#include "operatorconsoledialog.h"
#include "operatorconsole.h"
#include "ui_operatorconsoledialog.h"
#include "timestamp.h"
#include <QApplication>

OperatorConsoleDialog::OperatorConsoleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OperatorConsoleDialog)
{

    ui->setupUi(this);
    setupTextDisplays();

    console = new OperatorConsole;
    console->Init1(this);    // init the operator console object
    setupSignals();
    qsoInit();
    qsoShow(true);
}

OperatorConsoleDialog::~OperatorConsoleDialog()
{
    delete console;
    delete ui;
}

void OperatorConsoleDialog::setupSignals()
{
    connect(ui->btnQuit, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(ui->radioBlemish, &QRadioButton::clicked, console, &OperatorConsole::OnSetBlemish);
    connect(ui->radioSfr,     &QRadioButton::clicked, console, &OperatorConsole::OnSetSFRplus);
    connect(ui->radioCharts,  &QRadioButton::clicked, console, &OperatorConsole::OnSetArbitraryChart);
    connect(ui->btnJSON,      &QPushButton::clicked,  console, &OperatorConsole::OnShowJSON);
    connect(ui->btnSetup,     &QPushButton::clicked,  console, &OperatorConsole::OnSetup);
    connect(ui->btnPassFail,  &QPushButton::clicked,  console, &OperatorConsole::OnPassFail);

    connect(this, &OperatorConsoleDialog::startTest, console, &OperatorConsole::OnStart);
    connect(this, &OperatorConsoleDialog::stopTest,  console, &OperatorConsole::OnStop);
}

void OperatorConsoleDialog::setupTextDisplays()
{
    ui->reasonText->setReadOnly(true);
    ui->summaryText->setReadOnly(true);
    ui->logText->setReadOnly(true);
}

void OperatorConsoleDialog::on_btnStart_clicked()
{
    if (console->ReadyForTesting())
    {
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        emit startTest();
    }
}

void OperatorConsoleDialog::on_btnStop_clicked()
{
    ui->btnStop->setEnabled(false);
    ui->btnStart->setEnabled(true);
    console->OnStop();  // ideally we would emit a signal, but for now just call the method directly (that should be okay aslong as it returns right away)
}

void OperatorConsoleDialog::calcImageSize()
{
    //
    // If the size of the image is smaller than the screen size, resize the image to full size
    // Otherwise we need to resize the window to fill the screen, then resize the image (while
    // preserving its aspect ratio) so that it fits in the available space.
    //
    // The available space is equal to the window area minus a border around the image item
    // and leaving space for the widgets contained in moveableWidget collection
    //
    // get screen size
    // subtract width of moveable widget
    // subtract 2 * border width and 2* border height (use current image left and top)
    // resulting rect is available space
    // will image fit?
    // if yes, fiture
}


void OperatorConsoleDialog::set_image_size(const QSize &newSize)
{
    //
    // This function should be called when the size of the image changes.
    // This would generally happen whenever a new camera is selected.
    //
    // We want to make the image widget large enough to hold a full-size image.
    // The upper left corner of the image will stay the same, and the width and
    // height will be adjust as needed.
    //
    // We also want to change the width and height to the main window to make it
    // large enough to fit the image, and we need to move the other widgets so
    // that they stay near the right edge of the main window.  The other widgets
    // are all contained within a parent widget (called "moveable widget").
    //
    // Ultimately the main window needs to be large enough to contain the image
    // and the other widgets, without too much extra space.
    //

    //
    // TODO:  we need to handle cases when the image is larger than the screen size.  Two options would be to scale the image
    // (preserving the aspec ratior) so that the entire window would fit on the screen, or keep the image full-ize, but add
    // scroll bars.
    //

    QSize delta = newSize - ui->image->size();  // figure out the delta in width and height

    //
    // Resize the image widget, expanding/contracting width and height as needed
    //
    QRect   geometry = ui->image->geometry();
    geometry.adjust(0, 0, delta.width(), delta.height());
    ui->image->setGeometry(geometry);

    //
    // move the qso widgets to that they're centered in the image qwuadrants
    //
    qsoMove();

    //
    // Move the fps label so it is centred in the bottom 1/8th of the image
    //
    moveCenter(ui->labelFps, geometry.width()/2, geometry.bottom() - geometry.height()/8);

    //
    // Figure out the new window size.  The height of the window should be the larger
    // of the height of the image or the moveable widget
    //
    QSize windowOld = this->size();


    int imageHeight  = geometry.height();
    int widgetHeight = ui->moveableWidget->height();
    int windowHeight = imageHeight > widgetHeight ? imageHeight : widgetHeight;
    int windowWidth  =  windowOld.width() + delta.width();
    QSize windowNew(windowWidth, windowHeight+64);

    //
    // Resize the window (this sends a resize event)
    //
    this->resize(windowNew);
}

void OperatorConsoleDialog::update_image(QImage &image)
{
    QPixmap pixmap = QPixmap::fromImage(image);

    ui->image->setPixmap(pixmap);
}

void OperatorConsoleDialog::update_status(bool passed, QString &reason)
{
    if (passed)
    {
        ui->status->setStyleSheet("background-color: green;");
        ui->status->setText("PASSED");
    }
    else
    {
        ui->status->setStyleSheet("background-color: red;");
        ui->status->setText("FAILED");
    }

    ui->reasonText->clear();

    if (!passed)
    {
        ui->reasonText->appendPlainText(reason);
    }
}

void OperatorConsoleDialog::update_summary(QString &summary)
{
    ui->summaryText->clear();
    ui->summaryText->appendPlainText(summary);
}

void OperatorConsoleDialog::update_fps(QString &fps)
{
    ui->labelFps->setText(fps);
}

void OperatorConsoleDialog::update_quadrants(const bool *passed)
{
    for (int quadrant = 0; quadrant < eNumQuadrants; quadrant++)
    {
        qso[quadrant]->setCurrentIndex(passed[quadrant] ? 0 : 1);
    }
}

void OperatorConsoleDialog::log_message(QString &msg, bool timestamp)
{
    if (timestamp)
    {
        Timestamp now;
        QString time = now.Get();
        msg.prepend(time);
//      ui->logText->setPlainText(time);
    }

    ui->logText->appendPlainText(msg);
}

void OperatorConsoleDialog::resizeEvent(QResizeEvent *event)
{
    int     newWidth = event->size().width();
    int     oldWidth = event->oldSize().width();
    int     dx       = newWidth - oldWidth;
    QRect   rect = ui->moveableWidget->geometry();

    if (oldWidth != -1) // -1 means window is being created for the first time
    {
        ui->moveableWidget->move(rect.left()+dx, rect.top());
    }
}


void OperatorConsoleDialog::moveCenter(QWidget *widget, int centerX, int centerY)
{
    QRect   rect = widget->geometry();
    QPoint  pos(centerX, centerY);

    rect.moveCenter(pos);
    widget->move(rect.left(), rect.top());
}

void OperatorConsoleDialog::qsoMove()
{
    QPoint  center;
    QRect   rect;
    int     dx, dy;

    //
    // Get the location, size, and center point of the image
    //
    rect   = ui->image->geometry();
    center = rect.center();

    //
    // Calculate the distance in x and y from the center
    // of the image rect to the center of a quadrant
    //
    dx = (rect.width()  / 4);
    dy = (rect.height() / 4);


    //
    // Now move each qso widget so that it's centered at the center of its quadrant
    //
    //-----------
    //
    //  +     +
    //
    //     +
    //
    //  +     +
    //
    //-----------
    //
    moveCenter(ui->qsoUL,     center.x() - dx, center.y() - dy);	// upper left
    moveCenter(ui->qsoUR,     center.x() + dx, center.y() - dy);	// upper right
    moveCenter(ui->qsoLL,     center.x() - dx, center.y() + dy);	// lower left
    moveCenter(ui->qsoLR,     center.x() + dx, center.y() + dy);	// lower right
    moveCenter(ui->qsoCenter, center.x(),      center.y());         // center
}

void OperatorConsoleDialog::qsoInit()
{
    //
    // qso is an array of pointers to stacked widgets.
    //
    qso[eCenter]     = ui->qsoCenter;
    qso[eUpperRight] = ui->qsoUR;
    qso[eLowerRight] = ui->qsoLR;
    qso[eUpperLeft]  = ui->qsoUL;
    qso[eLowerLeft]  = ui->qsoLL;

}

void OperatorConsoleDialog::qsoShow(bool visible)
{
    for (int i = 0; i < eNumQuadrants; i++)
    {
        qso[i]->setVisible(visible);
    }
}
