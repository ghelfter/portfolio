/* ===============  Galen Helfter =============== */
/* ===============    CPSC 4040   =============== */
/* ===============  Assignment 5  =============== */
/* ===============   g_float.h    =============== */

#ifndef _G_FLOAT_H_
#define _G_FLOAT_H_

#include <cstdint>
#include <iostream>

#include <GL/gl.h>

/*  Isolate all of the image stuff in a namespace for oganization   */
namespace g_image
{
    void f_write_screen(std::string *filename, unsigned int width,
                        unsigned int height, unsigned int channels,
                        float *buffer);
    void f_flip_horizontal(float *out_buffer, const GLfloat *in_buffer,
                           uint32_t w, uint32_t h, uint32_t channels);
    void f_flip_vertical(float *out_buffer, const GLfloat *in_buffer,
                         uint32_t w, uint32_t h, uint32_t channels);
    class F_image
    {
      public:
          /*  Data is a pointer to the beginning of the data in single
           *  dimension form. Data is of size x_dim * y_dim * channels  */
          float *data;

          /*  Manually print out meta-information about the image  */
          void print(std::ostream &output);

          /*  Getters allowing for retrieval of dimension and channel
           *  information  */
          uint32_t get_x_dim() const;
          uint32_t get_y_dim() const;
          uint32_t get_channels() const;
          uint32_t get_total_size() const;

          void write(const std::string &filename);
          void clear_channel(uint32_t channel, float value);

          float get(unsigned int pos, unsigned int channel);
          float get(unsigned int x, unsigned int y, unsigned int channel) const;

          void set(unsigned int x, unsigned int y, unsigned int channel,
                   float value);

          bool copy_buffer_in(float *buffer, unsigned int buff_len);
          bool fill_buffer(float *buffer, unsigned int buff_len);

          bool reset(std::string filename);
          /*  Constructor  */
          F_image(uint32_t in_xdim, uint32_t in_ydim,
                  uint32_t in_channels = 1, float *buffer = 0,
                  unsigned int buff_len = 0);
          F_image(std::string filename, bool *success);
          F_image(const F_image &image);
          F_image();

          void replicate(const F_image &image);

          /*  Destructor  */
          ~F_image();
      protected:
          uint32_t x_dim;
          uint32_t y_dim;
          uint32_t channels;
          uint32_t total_size;
    };
}

#endif
