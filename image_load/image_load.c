/**************************************************
 *                  Galen Helfter
 *             ghelfter@protonmail.com
 *                   image_load.c
 **************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <png.h>

#include "image_load.h"

#define PNGHEADSIZE 8

#define WRITE_BITDEPTH 8

int image_load(struct ImageStore *img, const char *filename)
{
    int retval = IMG_GOOD_READ;
    unsigned char header[PNGHEADSIZE];
    FILE *fp = NULL;
    png_byte *working_buffer = NULL;
    png_byte **rowptrs = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;

    png_uint_32 w = 0u, h = 0u;
    int bit_depth = 0, color_type = 0;
    int rowbytes = 0;
    unsigned int i = 0u;

    fp = fopen(filename, "r");
    if(!fp)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        retval = IMG_FOPEN_FAIL;
        goto CLEANUP;
    }

    /* Acquire header */
    fread(header, sizeof(char), PNGHEADSIZE, fp);

    /* Bad header */
    if(!png_check_sig(header, PNGHEADSIZE))
    {
        fprintf(stderr, "File %s failed PNG header check.\n", filename);
        retval = IMG_BAD_SIG;
        goto CLEANUP;
    }
    /* If no return, we continue forward */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
                  NULL);
    if(!png_ptr)
    {
        fprintf(stderr, "Failed to acquire png_ptr.\n");
        retval = IMG_BAD_PNG_PTR;
        goto CLEANUP;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr)
    {
        fprintf(stderr, "Failed to acquire info_ptr.\n");
        retval = IMG_BAD_INFO_PTR;
        goto CLEANUP;
    }

    end_info = png_create_info_struct(png_ptr);
    if(!end_info)
    {
        fprintf(stderr, "Failed to acquire info_ptr.\n");
        retval = IMG_BAD_INFO_PTR;
        goto CLEANUP;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, PNGHEADSIZE);
    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth,
                  &color_type, NULL, NULL, NULL);

    /* Continue to load the data, then process it into the img
     * struct and into floating point values */
    fprintf(stdout, "Width: %u, height %u\n", (unsigned int) w,
            (unsigned int) h);
    fprintf(stdout, "Bit Depth: %d\n", bit_depth);
    fprintf(stdout, "Color type: %d\n", color_type);

    /* Acquire memory */
    rowbytes = png_get_rowbytes(png_ptr,info_ptr);
    working_buffer = malloc(sizeof(png_byte)*rowbytes*h);
    if(!working_buffer)
    {
        fprintf(stderr, "Failed to allocate working buffer.\n");
        retval = IMG_BAD_ALLOC;
        goto CLEANUP;
    }

    /* Set up row pointers */
    rowptrs = malloc(sizeof(png_byte*)*h);
    if(!rowptrs)
    {
        fprintf(stderr, "Failed to allocate row pointer memory.\n");
        retval = IMG_BAD_ALLOC;
        goto CLEANUP;
    }

    for(i = 0u; i < h; ++i)
    {
        *(rowptrs+i) = working_buffer+(rowbytes*i);
    }

    /* Allocate ImageStore structure elements */
    img->data = malloc(sizeof(float)*rowbytes*h);
    if(!img->data)
    {
        fprintf(stderr, "Failed to acquire image memory.\n");
        retval = IMG_BAD_ALLOC;
        goto CLEANUP;
    }

    img->channels = rowbytes / w;
    img->w = w;
    img->h = h;

    fprintf(stdout, "Channels test: %d\n", img->channels);

    /* Read the image */
    png_read_image(png_ptr, rowptrs);

    /* Transform to floating point */
    for(i = 0u; i < img->channels*img->w*img->h; ++i)
    {
        *(img->data+i) = (float) *(working_buffer+i) / 255.0;
    }


CLEANUP:
    /* Cleanup */
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    if(fp)
        fclose(fp);
    if(working_buffer)
        free(working_buffer);
    if(rowptrs)
        free(rowptrs);
    fp = NULL;
    working_buffer = NULL;
    rowptrs = NULL;
    return retval;
}

int image_write(struct ImageStore *img, const char *filename)
{
    int retval = IMG_GOOD_WRITE;
    unsigned int i = 0u;
    const unsigned int s = img->w * img->channels * img->h;
    float working_byte = 0.0f;
    FILE *fp = NULL;
    png_byte *working_buffer = NULL;
    png_byte **rowptrs = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int color_type = 0;

    /* Acquire color type information */
    switch(img->channels)
    {
      case 4:
          color_type = 6;
          break;
      case 3:
          color_type = 2;
          break;
      case 2:
          color_type = 4;
          break;
      case 1:
      default:
          color_type = 0;
          break;
    };

    fprintf(stdout, "Writing color type %d\n", color_type);

    /* Allocate new memory */
    working_buffer = malloc(sizeof(png_byte)*s);
    if(!working_buffer)
    {
        retval = IMG_BAD_ALLOC;
        fprintf(stderr, "Failed to acquire memory.\n");
        goto CLEANUP1;
    }

    /* Set up row pointers */
    rowptrs = malloc(sizeof(png_byte*)*img->h);
    if(!rowptrs)
    {
        retval = IMG_BAD_ALLOC;
        fprintf(stderr, "Failed to acquire memory.\n");
        goto CLEANUP1;
    }

    /* Initialize row pointers */
    for(i = 0u; i < img->h; ++i)
    {
        *(rowptrs+i) = working_buffer+(img->w*img->channels*i);
    }

    /* Convert back to 8 bit */
    for(i = 0u; i < s; ++i)
    {
        /* Floor it to rid ourselves of any decimal */
        working_byte = (float) floor(*(img->data+i)*255.0f);

        /* Clamp value between zero and 255 */
        working_byte = working_byte < 255.0f ? working_byte : 255.0f;
        working_byte = working_byte > 0.0f ? working_byte : 0.0f;

        *(working_buffer+i) = (png_byte) working_byte;
    }
    
    /* Open image file */
    fp = fopen(filename, "w+");

    /* File failed to open */
    if(!fp)
    {
        retval = IMG_FOPEN_FAIL;
        fprintf(stderr, "Failed to open image %s to write.\n", filename);
        goto CLEANUP1;
    }

    /* Initialize png structures */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
                                      NULL);
    if(!png_ptr)
    {
        retval = IMG_BAD_PNG_PTR;
        fprintf(stderr, "Failed to create PNG struct.\n");
        goto CLEANUP1;
    }

    info_ptr = png_create_info_struct(png_ptr);

    if(!info_ptr)
    {
        retval = IMG_BAD_INFO_PTR;
        fprintf(stderr, "Failed to initialize info struct.\n");
        goto CLEANUP2;
    }

    /* Output image */
    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, img->w, img->h, WRITE_BITDEPTH,
                 color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    png_write_image(png_ptr, rowptrs);
    png_write_end(png_ptr, NULL);

    /* Cleanup and error handling code */
CLEANUP2:
    png_destroy_write_struct(&png_ptr, &info_ptr);
CLEANUP1:
    if(fp)
        fclose(fp);
    if(rowptrs)
        free(rowptrs);
    if(working_buffer)
        free(working_buffer);
    return retval;
}
