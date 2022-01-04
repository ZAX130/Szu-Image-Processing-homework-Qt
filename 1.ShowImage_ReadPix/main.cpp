#include "myWin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    myWin w;
    w.show();
    return a.exec();
}
