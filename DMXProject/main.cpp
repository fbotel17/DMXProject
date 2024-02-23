#include "DMXProject.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DMXProject w;
    w.show();
    return a.exec();
}
