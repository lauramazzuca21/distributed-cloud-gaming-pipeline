#include "GstRawFramesApp.h"

int main(int argc, char *argv[])
{
    GstRawFramesApp* app = new GstRawFramesApp();

    app->run(argc, argv);
    
    return 0;
}