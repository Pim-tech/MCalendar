#include <QtWidgets>
#include "window.h"

int main(int argv, char *args[])
{
    QApplication app(argv, args);

    QTranslator translator;

    translator.load("MCalendar_fr","/home/moi/cpp_sources/Qt/MCalendar");

    app.installTranslator(&translator);

    Window window;
    window.show();
    return app.exec();
}
