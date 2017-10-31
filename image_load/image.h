/**************************************************
 *                  Galen Helfter
 *             ghelfter@protonmail.com
 *                     image.h
 **************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#ifdef IMG_INDEX
#undef IMG_INDEX
#endif
#define IMG_INDEX(ind,channel,nchannels) ((ind)*(nchannels)+(channel))

#ifdef IMG_SIZE
#undef IMG_SIZE
#endif
#define IMG_SIZE(img) ((img->w)*(img->channels)*(img->h))

/* Store images as floats internally */
struct ImageStore
{
    float *data;
    unsigned int w;
    unsigned int h;
    unsigned int channels;
};

#define IMG_SUCCESS     0
#define IMG_BAD_CHANNEL 1
#define IMG_BAD_STRUCT  2

int image_invert_channel(struct ImageStore *img, unsigned int channel);

void image_init_clear(struct ImageStore *img);
void image_insert_channel_after(const struct ImageStore *img,
                                struct ImageStore *res);
void image_clear_channel(struct ImageStore *img, unsigned int channel,
                         float val);
void image_invert_inplace(struct ImageStore *img);
void image_copy(const struct ImageStore *img, struct ImageStore *res);
void image_invert(const struct ImageStore *img, struct ImageStore *res);
void image_create_positive_matte(const struct ImageStore *img,
                                 struct ImageStore *res);
void image_create_negative_matte(const struct ImageStore *img,
                                 struct ImageStore *res);

void image_cleanup(struct ImageStore *img);

#endif /* _IMAGE_H_ */
