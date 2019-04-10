#include "operatorconsoledialog.h"
#include <QApplication>
#include <QFile>
#include <QProcess>
#include <QTextStream>
#include <QFontDatabase>
#include <QFont>
using namespace std;


int main(int argc, char *argv[])
{
    QFile   out;
    out.open(stdout, QIODevice::ReadOnly);

    QApplication a(argc, argv);
#ifdef __linux
    // On Linux, we're statically building QT so we must use an embedded font
    QFontDatabase::addApplicationFont(":/fonts/DejaVuSans.ttf");
    a.setFont(QFont("DejaVue", 11, QFont::Normal, false));
#endif
    OperatorConsoleDialog w(nullptr, &out);

    w.show();

    return a.exec();
}
