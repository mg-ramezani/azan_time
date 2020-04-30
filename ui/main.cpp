#include "Widget.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    azan_widget window;

    if (argc > 1 && std::string(argv[1]) == "min")
    {
        window.hide();
    }
    else
    {
        window.show();
    }

    return application.exec();
}
