/* ===============  Galen Helfter =============== */
/* ===============    CPSC 4040   =============== */
/* ===============  Final Project =============== */
/* ===============    masking.h   =============== */

#ifndef _MASKING_H_
#define _MASKING_H_

/*  Make sure it can be called properly by C++ code  */
#ifdef __cplusplus
namespace masking
{
extern "C"
{
#endif

#define BINARY_OPAQUE 0xff
#define BINARY_TRANSPARENT 0x00

#define APPROX_HSV_GREEN 120
#define EPSILON 30

#define R_LOC 0
#define G_LOC 1
#define B_LOC 2

#ifndef FINAL_CHANNELS
#define FINAL_CHANNELS 4
#endif
#ifndef MAX_C
#define MAX_C 255.0
#endif

void to_given_channels(uint8_t *out_buffer, const uint8_t *in_buffer,
                       uint32_t new_channels, uint32_t current_channels,
                       uint32_t new_size, uint32_t old_size);

void to_given_channels_f(float *out_buffer, const float *in_buffer,
                         uint32_t new_channels, uint32_t current_channels,
                         uint32_t new_size, uint32_t old_size);

uint32_t new_channel_size(uint32_t n_channels, uint32_t c_channels, uint32_t len);

void binary_mask(uint8_t *buffer, uint32_t buffer_len, double hue, double sat,
                 double val, double h_eps, double s_eps, double v_eps, bool invert);

void fancy_mask(uint8_t *buffer, uint32_t buffer_len, double hue, double sat,
                double val, double h_eps, double s_eps, double v_eps, bool invert);

void petro_vlahos(uint8_t *buffer, uint32_t buffer_len, double hue, double sat,
                  double val, double h_eps, double s_eps, double v_eps, bool invert);

void no_spill(uint8_t *buffer, uint32_t buffer_len);

void spill_suppression(uint8_t *buffer, uint32_t buffer_len);

void convert_rgb_hsv(double *H, double *S, double *V, uint8_t R, uint8_t G, uint8_t B);

#ifdef __cplusplus
}
}
#endif

#endif
