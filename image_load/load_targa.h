/**************************************************
 *                  Galen Helfter
 *              ghelfte@g.clemson.edu
 *                   load_targa.h
 **************************************************/

#ifndef _LOAD_TARGA_H_
#define _LOAD_TARGA_H_

#include "image.h"

/* Defined error codes */
#define TARGA_SUCCESS     0
#define TARGA_NULL_PTR   -1
#define TARGA_FOPEN_FAIL -2
#define TARGA_BAD_ALLOC   -3

int parse_targa_file(const char *filename, struct ImageStore *img);

#endif /* _LOAD_TARGA_H_ */
