#include <QCoreApplication>
#include "viewerconsole.h"
int main()
{
    //QCoreApplication a(argc, argv);
    viewerConsole *v = new viewerConsole();
    v->display();
    //return a.exec();
}
