#include "MGuiApplication.h"

int
main(int argc, char* argv[])
{
    setenv("ILX_THEMEDIR", DATADIR"/theme/", 1);
    MGUI::MGuiApplication app(argc, argv);
    app.exec();
    return 0;
}
