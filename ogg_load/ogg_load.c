/**************************************************
 *                  Galen Helfter
 *             ghelfter@protonmail.com
 *                    ogg_load.c
 **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ogg_load.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#define OGG_LOAD_BUFFER_SIZE 4096

static int word_size = 2;
static int sign = 1;
static int endian = 1;

int ogg_load_track_mem(const char *filename, char **memory,
                       unsigned int *ichannels, unsigned int *iword_size,
                       unsigned int *ibytes)
{
    int retval = AUDIO_LOAD_SUCCESS;
    long nbytes = 0;
    int needed_bytes = 0;
    int channels = 0;
    int rcode = 0;
    int eof = 0;
    int c_stream = 0;
    char *buffer = NULL;
    OggVorbis_File vf; /* Ogg/vorbis file structure */
    vorbis_info *vi = NULL;

    long read_bytes = 0;

    if(!filename)
    {
        retval = AUDIO_NULL_PTR;
        goto CLEANUP; /* Rather have a single point of exit */
    }

    /* Use their fopen command for portability to Windows */
    rcode = ov_fopen(filename, &vf);
    if(rcode != 0) /* File failed to open */
    {
        retval = AUDIO_FOPEN_FAIL;
        goto CLEANUP;
    }

    vi = ov_info(&vf, -1);
    channels = vi->channels;
    needed_bytes = ov_pcm_total(&vf, -1) * channels * word_size;

    *memory = NULL;

    buffer = malloc(sizeof(char) * OGG_LOAD_BUFFER_SIZE);
    *memory = malloc(sizeof(char) * needed_bytes);
    if(!buffer || !memory)
    {
        retval = AUDIO_OUT_OF_MEM;
        goto CLEANUP;
    }

    nbytes = 0;

    while(!eof)
    {
        read_bytes = ov_read(&vf, buffer, OGG_LOAD_BUFFER_SIZE, endian,
                             word_size, sign, &c_stream);
        if(read_bytes == 0)
        {
            eof = 1; /* Reached the end of the file */
        }
        else
        {
            memcpy(*memory+nbytes, buffer, read_bytes);
            nbytes += read_bytes;
        }
    }

    fprintf(stdout, "Read_bytes: %ld\n", (long) nbytes);

CLEANUP:
    if(retval != AUDIO_LOAD_SUCCESS)
    {
        free(*memory);
        *memory = NULL;
    }
    else
    {
        *ichannels = (unsigned int) channels;
        *iword_size = (unsigned int) word_size;
        *ibytes = (unsigned int) nbytes;
    }
    free(buffer);
    ov_clear(&vf);
    return retval;
}
