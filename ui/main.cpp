#include "Widget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    azan_widget window;
    window.show();

    return application.exec();
}
