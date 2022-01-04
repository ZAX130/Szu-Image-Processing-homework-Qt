#include "MathOperation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MathOperation w;
    w.show();
    return a.exec();
}
