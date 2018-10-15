#include "operatorconsoledialog.h"
#include <QApplication>
#include <QProcess>

static void outputready();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OperatorConsoleDialog w;

    w.show();

    return a.exec();
}
