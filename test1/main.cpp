#include <QtGui/QApplication>
#include "test1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    test1 w;
    w.show();

    return a.exec();
}
