#include "ImageSegmentation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageSegmentation w;
    w.show();
    return a.exec();
}
