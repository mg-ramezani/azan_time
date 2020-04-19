#include "Widget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    Widget window;
    window.show();

    return application.exec();
}
