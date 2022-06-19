#pragma once


#include <stdio.h>
#include <jpeglib.h> // ref to https://raw.githubusercontent.com/libjpeg-turbo/libjpeg-turbo/main/libjpeg.txt
#include <png.h>
#include <zlib.h>

namespace file_handler {

enum C_TYPE {
    JPEG,
    PNG,
    NONE
};

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
    }

    static void save_png_from_buffer(const char * filename, 
                                        const int width, const int height, 
                                        unsigned char * buffer) {
        FILE *outfile = fopen(filename, "wb"); //write binary file
        if (!outfile)
            return;
        
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr)
        return;

        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr)
        {
        png_destroy_write_struct(&png_ptr,
            (png_infopp)NULL);
        return;
        }

        //If you would rather avoid the complexity of setjmp/longjmp issues, you can compile libpng with PNG_NO_SETJMP

        png_set_check_for_invalid_index(png_ptr, 0);
        //set up the output code
        png_init_io(png_ptr, outfile);

        /* Set the zlib compression level */
        png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
        /* Set the parameters for the image to write */
        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, 
                    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        /* Proceed with compression */
        png_bytepp rows = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (int i = 0; i < height; ++i)
            rows[i] = (png_bytep)(buffer + (height - i - 1) * width * 4);

        png_set_rows(png_ptr, info_ptr, rows);
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

        /* now write the image */

        png_write_image(png_ptr, rows);
        png_write_end(png_ptr, info_ptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);

        fclose(outfile);
        delete[] rows;
    }

    static void save_from_buffer(const char * filename, 
                                    const int width, const int height, 
                                    unsigned char * buffer, C_TYPE type) {
        switch(type) {
            case JPEG:
                save_jpeg_from_buffer(filename, width, height, buffer);
                break;
            case PNG:
                save_png_from_buffer(filename, width, height, buffer);
                break;
            default:
                break;
        }
    }
}