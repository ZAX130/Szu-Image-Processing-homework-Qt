#include "RGB2HSV.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RGB2HSV w;
    w.show();
    return a.exec();
}
