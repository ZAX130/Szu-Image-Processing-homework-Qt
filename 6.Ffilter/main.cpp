#include "Ffilter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ffilter w;
    w.show();

    return a.exec();
}
