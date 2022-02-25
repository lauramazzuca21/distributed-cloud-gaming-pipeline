
#include "src/GstRawFramesApp.h"
// #include "src/RawFramesApp.h"

int main(int argc, char *argv[])
{
    GstRawFramesApp* app = new GstRawFramesApp();
    // RawFramesApp* app = new RawFramesApp();

    app->run(argc, argv);
    
    return 0;
}
