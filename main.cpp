#include "operatorconsoledialog.h"
#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QTextStream>
using namespace std;


int main(int argc, char *argv[])
{
    QFile   out;

    out.open(stdout, QIODevice::ReadOnly);
    QApplication a(argc, argv);

    OperatorConsoleDialog w(nullptr, &out);

    w.show();

    return a.exec();
}
