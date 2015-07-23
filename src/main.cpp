#include "ui/MGuiApplication.h"

int
main(int argc, char* argv[])
{
    MGUI::MGuiApplication app(argc, argv);
    app.exec();
    return 0;
}
