#include "ImageAugmentation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageAugmentation w;
    w.show();
    return a.exec();
}
