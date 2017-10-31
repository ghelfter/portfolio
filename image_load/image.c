/**************************************************
 *                  Galen Helfter
 *             ghelfter@protonmail.com
 *                     image.c
 **************************************************/

#include <stdlib.h>
#include <string.h>

#include "image.h"

#ifndef INVF
#define INVF(x) ((1.0f)-(x))
#endif

int image_invert_channel(struct ImageStore *img, unsigned int channel)
{
    int retval = IMG_SUCCESS;
    unsigned int i = 0u;
    unsigned int s = 0u;
    float tmp = 0.0f;

    /* Passed a null pointer */
    if(!img)
    {
        retval = IMG_BAD_STRUCT;
    }
    /* Channel doesn't exist in given image */
    else if(channel > img->channels-1u)
    {
        retval = IMG_BAD_CHANNEL;
    }
    /* Image data doesn't exist */
    else if(!img->data)
    {
        retval = IMG_BAD_STRUCT;
    }
    /* Perform operation */
    else
    {
        s = IMG_SIZE(img);
        for(i = 0u; i < s; i += img->channels)
        {
            tmp = INVF(*(img->data+i+channel));
            *(img->data+i+channel) = tmp;
        }
    }

    return retval;
}

/* This will clear an ImageStore struct, setting all values and data to zero
 * and NULL. This function assumes the image has not been initialized with any
 * data, and will leak if passed an image that has been used for something. */
void image_init_clear(struct ImageStore *img)
{
    img->data = NULL;

    img->w = 0u;
    img->h = 0u;
    img->channels = 0u;
}

/* This will insert a new channel into the res structure, copying all of
 * the values stored in img to the new positions in res. This will
 * allocate the new memory for the res structure and initialize it,
 * meaning that it should not have been initialized with anything
 * but the image_init_clear function.
 * */
void image_insert_channel_after(const struct ImageStore *img,
                                struct ImageStore *res)
{
    unsigned int res_i = 0u, img_i = 0u;
    const unsigned int img_size = IMG_SIZE(res);

    /* Acquire new values */
    res->channels = img->channels + 1u;
    res->w = img->w;
    res->h = img->h;

    /* Allocate memory for res */
    res->data = malloc(sizeof(float)*img_size);

    while(img_i < img_size)
    {
        memcpy(res->data+res_i, img->data+img_i, img->channels);
        *(res->data+res_i+res->channels-1) = 0.0f; /* Clear to 0.0 */

        img_i += img->channels;
        res_i += res->channels;
    }
}

/* Given a channel and the image, it will clear every element of that
 * channel to the given float value. */
void image_clear_channel(struct ImageStore *img, unsigned int channel,
                         float val)
{
    const unsigned int s = IMG_SIZE(img);
    unsigned int i = 0u;
    if(channel < img->channels)
    {
        for(i = 0u; i < s; i += img->channels)
        {
            *(img->data+i+channel) = val;
        }
    }
}

/* Inverts every channel of the image, overriding the value already stored.
 * This preserves the information of the image, as it may be reinverted. */
void image_invert_inplace(struct ImageStore *img)
{
    const unsigned int s = IMG_SIZE(img);
    unsigned int index = 0u;
    float t_val = 0.0;

    for(index = 0u; index < s; ++index)
    {
        /* Invert value */
        t_val = *(img->data+index);
        t_val = 1.0f - t_val;

        *(img->data+index) = t_val;
    }
}

void image_copy(const struct ImageStore *img, struct ImageStore *res)
{
    res->data = malloc(sizeof(float)*img->w*img->channels*img->h);
    res->w = img->w;
    res->h = img->h;
    res->channels = img->channels;
}

/* Inverts every channel of the image, storing in res. */
void image_invert(const struct ImageStore *img, struct ImageStore *res)
{
    const unsigned int s = IMG_SIZE(img);
    unsigned int index = 0u;

    for(index = 0u; index < s; ++index)
    {
        /* Invert value */
        *(res->data+index) = 1.0f - *(img->data+index);
    }
}

/* Assumes there is at least four channels, this will create  */
void image_create_positive_matte(const struct ImageStore *img,
                                 struct ImageStore *res)
{
    const unsigned int s = img->w*img->h*img->channels;
    unsigned int index = 0u;

    for(index = 0u; index < s; index+=img->channels)
    {
        *(res->data+index) = *(img->data+index+3);
    }
}

void image_cleanup(struct ImageStore *img)
{
    if(img->data)
    {
        free(img->data);
    }

    img->w = 0u;
    img->h = 0u;
    img->channels = 0u;
}
