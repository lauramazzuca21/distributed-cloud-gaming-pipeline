
#include "src/GstRawFramesApp.h"
// #include "src/RawFramesApp.h"

int main(int argc, char *argv[])
{
    GstRawFramesApp app;
    // RawFramesApp* app = new RawFramesApp();

    app.run(argc, argv);
    
    return 0;
}
