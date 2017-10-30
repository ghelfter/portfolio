/**************************************************
 *                  Galen Helfter
 *             ghelfter@protonmail.com
 *                    ogg_load.h
 **************************************************/

#ifndef _OGG_LOAD_H_
#define _OGG_LOAD_H_

/* Various library error codes */
#define AUDIO_LOAD_SUCCESS  0 /* Function completed successfully */
#define AUDIO_NULL_PTR     -1 /* Function given a NULL pointer */
#define AUDIO_OUT_OF_MEM   -2 /* Failed memory allocation */
#define AUDIO_OGG_LIB_FAIL -3 /* Internal ogg library failure */
#define AUDIO_FOPEN_FAIL   -4 /* File failed to open or is invalid ogg file */

/* Loads an ogg/vorbis track into memory. Returns an error code if it fails
 * to do so, and a success code (0) if it succeeds. */
int ogg_load_track_mem(const char *filename, char **memory,
                       unsigned int *ichannels, unsigned int *iword_size,
                       unsigned int *ibytes);

#endif /* _OGG_LOAD_H_ */
