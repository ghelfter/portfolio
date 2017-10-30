/**************************************************
 *                  Galen Helfter
 *             ghelfter@protonmail.com
 *                   image_load.h
 **************************************************/

#ifndef _IMAGE_LOAD_H_
#define _IMAGE_LOAD_H_

#include "image.h"

#define IMG_GOOD_READ    0
#define IMG_GOOD_WRITE   0
#define IMG_FOPEN_FAIL   1
#define IMG_BAD_SIG      2
#define IMG_BAD_PNG_PTR  3
#define IMG_BAD_INFO_PTR 4
#define IMG_BAD_ALLOC    5

int image_load(struct ImageStore *img, const char *filename);
int image_write(struct ImageStore *img, const char *filename);

#endif /* _IMAGE_LOAD_H_ */
