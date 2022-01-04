#include "FFT.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FFT w;
    w.show();
    return a.exec();
}
