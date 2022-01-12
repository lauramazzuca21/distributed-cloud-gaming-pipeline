#include <jpeglib.h>
#include <stdio.h>

namespace file_handler {

    static void save_jpeg_from_buffer(const char * filename, 
                                        const int width, const int height, 
                                        unsigned char * buffer) {
        FILE *outfile = fopen(filename, "wb"); //write binary file
        if (!outfile)
            return;
        
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr       jerr;

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_compress(&cinfo);
        jpeg_stdio_dest(&cinfo, outfile);

        cinfo.image_width      = width;
        cinfo.image_height     = height;
        cinfo.input_components = 4;
        cinfo.in_color_space   = JCS_EXT_RGBA;

        jpeg_set_defaults(&cinfo);
        jpeg_set_quality (&cinfo, 75, true);
        jpeg_start_compress(&cinfo, true);

        JSAMPROW row_pointer[1];        /* pointer to a single row */
        int row_stride;                 /* physical row width in buffer */

        row_stride = width * 4;   /* JSAMPLEs per row in image_buffer */

        while (cinfo.next_scanline < cinfo.image_height) {
            row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
            jpeg_write_scanlines(&cinfo, row_pointer, 1);
        }
        jpeg_finish_compress(&cinfo);
        fclose(outfile);
        jpeg_destroy_compress(&cinfo);
        printf("Done.\n");
    }
}