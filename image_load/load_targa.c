/**************************************************
 *                  Galen Helfter
 *              ghelfte@g.clemson.edu
 *                   load_targa.c
 **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "load_targa.h"

#define DEFAULT_BUFFER_SIZE 512
#define TARGA_FOOTER_STRING "TRUEVISION-XFILE"

#define TARGA_RUN_LENGTH_MASK 0x80
#define TARGA_COUNT_MASK      0x7f

struct targa_header
{
    uint8_t id_length;
    uint8_t color_map;
    uint8_t image_type;
    uint8_t color_map_spec[5];
    uint8_t image_spec[10];
};

static void print_image_type(unsigned int code);

int parse_targa_file(const char *filename, struct ImageStore *img)
{
    int retcode = TARGA_SUCCESS;
    struct targa_header header;
    const unsigned int read_amt = sizeof(struct targa_header);
    unsigned int image_size = 0u;
    unsigned int index = 0u;
    unsigned int i = 0u;
    FILE *fd = NULL;
    unsigned char run_length = 0;
    unsigned char run_length_count = 0;
    unsigned char *buffer = NULL;

    if(filename == NULL || img == NULL)
    {
        retcode = TARGA_NULL_PTR;
        goto CLEANUP;
    }

    buffer = malloc(sizeof(unsigned char) * DEFAULT_BUFFER_SIZE);

    if(buffer == NULL)
    {
        retcode = TARGA_BAD_ALLOC;
        goto CLEANUP;
    }

    /* Open the file */
    fd = fopen(filename, "r");

    if(fd == NULL)
    {
        retcode = TARGA_FOPEN_FAIL;
        goto CLEANUP;
    }
    /* Clear image */
    image_init_clear(img);

    /* Parse the header of the file */
    fread(&header, sizeof(uint8_t), read_amt, fd);

    /* Seek to the end to check whether it is a new or old format image */
    fseek(fd, -26, SEEK_END);
    fread(buffer, sizeof(unsigned char), 26, fd);

    /* Assumption will be that houdini does not output the newer style
     * of targa files. This is what this code implies and is what I see
     * when I inspect it with a hex editor */
    if(!strncmp((const char*)buffer+8, TARGA_FOOTER_STRING, 15))
    {
        fprintf(stdout, "This is a newer file.");
    }

    img->w = *((uint16_t*)(header.image_spec+4));
    img->h = *((uint16_t*)(header.image_spec+6));
    img->channels = 4;

    /* Calculate the image size needed */
    image_size = sizeof(float) * 4 * img->w * img->h;
    img->data = malloc(image_size);

    /* Iterate over the data */
    fseek(fd, 0, SEEK_SET);

    fread(buffer, sizeof(unsigned char),
          sizeof(struct targa_header) + header.id_length, fd);

    index = 0u;

    /* Note: To deal with Houdini's image, run-length encoding must be
     * dealt with. Run-length encoded true-color is to be assumed for
     * this piece of code. */
    while(index < img->w * img->h)
    {
        fread(buffer, sizeof(unsigned char), 1, fd);

        /* Check the run length encoding information */
        run_length = buffer[0] & TARGA_RUN_LENGTH_MASK;
        run_length_count = buffer[0] & TARGA_COUNT_MASK;

        if(run_length)
        {
            fread(buffer, sizeof(unsigned char), 4, fd);
            for(i = 0u; i < run_length_count+1u; ++i)
            {
                *(img->data + ((index+i)*4) + 0u) = (float) buffer[2] / 255.0;
                *(img->data + ((index+i)*4) + 1u) = (float) buffer[1] / 255.0;
                *(img->data + ((index+i)*4) + 2u) = (float) buffer[0] / 255.0;
                *(img->data + ((index+i)*4) + 3u) = (float) buffer[3] / 255.0;
            }
        }
        else
        {
            for(i = 0; i < run_length_count+1u; ++i)
            {
                fread(buffer, sizeof(unsigned char), 4, fd);
                *(img->data + ((index+i)*4) + 0u) = (float) buffer[2] / 255.0;
                *(img->data + ((index+i)*4) + 1u) = (float) buffer[1] / 255.0;
                *(img->data + ((index+i)*4) + 2u) = (float) buffer[0] / 255.0;
                *(img->data + ((index+i)*4) + 3u) = (float) buffer[3] / 255.0;
            }
        }

        /* Increment the index accordingly */
        index += run_length_count + 1u;
    }

CLEANUP:
    /* Close file */
    if(fd != NULL)
    {
        fclose(fd);
        fd = NULL;
    }

    /* Clear memory */
    if(buffer != NULL)
    {
        free(buffer);
        buffer = NULL;
    }

    return retcode;
}

static void print_image_type(unsigned int code)
{
    switch(code)
    {
        case 0:
            fprintf(stdout, "No image data is given.\n");
            break;
        case 1:
            fprintf(stdout, "An uncompressed color image\n");
            break;
        case 2:
            fprintf(stdout, "An uncompressed true-color image.\n");
            break;
        case 3:
            fprintf(stdout, "An uncompressed black and white image.\n");
            break;
        case 9:
            fprintf(stdout, "A run-length encoded color image.\n");
            break;
        case 10:
            fprintf(stdout, "A run-length encoded true-color image.\n");
            break;
        case 11:
            fprintf(stdout, "A run-length encoded black and white image.\n");
            break;
        default:
            fprintf(stdout, "Undefined code.");
            break;
    }
}
