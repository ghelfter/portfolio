/* ===============  Galen Helfter =============== */
/* ===============    CPSC 4040   =============== */
/* ===============  Assignment 5  =============== */
/* ===============   g_float.cpp  =============== */

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cinttypes>
#include <memory>
#include <algorithm>

/*  Include OpenImageIO headers  */
#include <OpenImageIO/imageio.h>

#include "./headers/g_float.h"


/*
 * Writes the current buffer out to a file using the OpenImageIO library.
 * Input:
 *   filename - filename of image to be written
 *   x_dim - horizontal dimension of the image
 *   y_dim - vertical dimension of the image
 *   channels - channels in the image
 *   buffer - buffer containing image data
 **/
void g_image::f_write_screen(std::string *filename, unsigned int x_dim,
                             unsigned int y_dim, unsigned int channels,
                             float *buffer)
{
    /*  You are by far the worst openGL call I have ever heard of  */
    /*  "But you have heard of me!"  */
    
    /*  Removed glReadPixels because it is a terrible function. I keep a
     *  buffer manually now. Life is better.
    glReadPixels(0, 0, x_dim, y_dim, GL_RGBA | GL_ALPHA,
                 GL_UNSIGNED_BYTE, buffer);
    */
    /*  Open file  */
    OpenImageIO::ImageSpec spec(x_dim, y_dim, channels,
                                OpenImageIO::TypeDesc::FLOAT);
    std::unique_ptr<OpenImageIO::ImageOutput> output(OpenImageIO::ImageOutput::create(*filename));

    output->open(*filename, spec);

    /*  Write information  */
    output->write_image(OpenImageIO::TypeDesc::FLOAT, buffer);

    /*  Close output  */
    output->close();
}

/*
 * Takes an input and output buffer, and flips the input buffer horizontally
 * to be placed in the output buffer.
 * Input:
 *   out_buffer - buffer to be written to
 *   in_buffer - buffer to be read from
 *   w - width of the image
 *   h - height of the image
 *   channels - channels in the image
 */
void g_image::f_flip_horizontal(float *out_buffer, const float *in_buffer,
                                uint32_t w, uint32_t h, uint32_t channels)
{
    /*  Must flip everything. This loop is pretty damn terrible, though.  */
    for(unsigned int r_in = 0; r_in < h; ++r_in)
    {
        for(unsigned int c_in = 0; c_in < w; ++c_in)
        {
            for(unsigned int pix = 0; pix < channels; ++pix)
            {
                out_buffer[(r_in * w * channels) + ((w - 1 - c_in) * channels)
                           + pix] = in_buffer[(r_in * w * channels) +
                                              (c_in * channels) + pix];
            }
        }
    }
}

/*
 * Takes an input and output buffer, and flips the input buffer vertically
 * to be placed in the output buffer.
 * Input:
 *   out_buffer - buffer to be written to
 *   in_buffer - buffer to be read from
 *   w - width of the image
 *   h - height of the image
 *   channels - channels in the image
 */
void g_image::f_flip_vertical(float *out_buffer, const float *in_buffer,
                            uint32_t w, uint32_t h, uint32_t channels)
{
    for(unsigned int r_in = 0; r_in < h; ++r_in)
    {
        memcpy(out_buffer + (r_in * w * channels), in_buffer +
               ((h - r_in - 1) * w * channels), w * channels * sizeof(float));
    }
}


void g_image::F_image::print(std::ostream &output)
{
    output << "Width: " << this->x_dim << std::endl;
    output << "Height: " << this->y_dim << std::endl;
    output << "Channels: " << this->channels << std::endl;
    output << "Total bytes: " << this->total_size << std::endl;
}

/*
 * Clears a specific channel in the image by setting it all to a value.
 * Input:
 *   channel - channel to be cleared
 *   value - value to clear channel to
 */
void g_image::F_image::clear_channel(uint32_t channel, float value)
{
    if(channel < this->channels)
        return;
    for(uint32_t i = channel - 1; i < this->total_size; i += this->channels)
        this->data[i] = value;
}

float g_image::F_image::get(unsigned int pos, unsigned int channel)
{
    return *(this->data + (pos * this->channels) + channel);
}

float g_image::F_image::get(unsigned int x, unsigned int y, unsigned int channel) const
{
    return *(this->data + ((x + y*this->x_dim) * this->channels) + channel);
}

void g_image::F_image::set(unsigned int x, unsigned int y, unsigned int channel,
                           float value)
{
    this->data[(x + y*this->x_dim)*this->channels + channel] = value;
}

/*
 * Constructor: Build a member of the Image class given a lot of stuff.
 * Input:
 *   in_xdim - width of the image
 *   in_ydim - height of the image
 *   in_channels - number of channels in the image
 *   buffer - buffer containing data to put in image
 *   buff_len - length of the buffer
 */
g_image::F_image::F_image(uint32_t in_xdim, uint32_t in_ydim,
                          uint32_t in_channels, float *buffer,
                          unsigned int buff_len)
{
    this->x_dim = in_xdim;
    this->y_dim = in_ydim;
    this->channels = in_channels;
    this->total_size = in_xdim * in_ydim * in_channels;

    /*  Initialize arrays, make sure they end up as NULL if not given a proper
     *  dimension  */
    if(in_xdim * in_ydim > 0)
    {
        this->data = new float[this->total_size];
    }
    else
    {
        this->data = 0;
    }

    std::fill(this->data, this->data + this->total_size, 1);

    /*  Must add stuff to copy image data into data buffer  */
    std::copy(buffer, buffer + this->total_size, this->data);
}

/*
 * Constructor reading the image in from a file using the OpenImageIO library.
 * Input:
 *   filename - name of the file to read from
 *   success - pointer to a boolean determining success of reading in the file
 */
g_image::F_image::F_image(std::string filename, bool *success)
{
    *success = false;
    /*  Open input stuff  */
    std::unique_ptr<OpenImageIO::ImageInput> input(
                    OpenImageIO::ImageInput::open(filename));

    if(!input.get())
        return;

    /*  Get all of our informations  */
    const OpenImageIO::ImageSpec &spec = input->spec();
    this->x_dim = spec.width;
    this->y_dim = spec.height;
    this->channels = spec.nchannels;
    this->total_size = this->x_dim * this->y_dim * this->channels;
    std::unique_ptr<float[]> t_buff(new float[this->total_size]);


    /*  Read in input data  */
    input->read_image(OpenImageIO::TypeDesc::FLOAT, t_buff.get());

    /*  Close input file  */
    input->close();
    
    this->data = new float[this->total_size];
    
    std::copy(t_buff.get(), t_buff.get() + this->total_size, this->data);
    // std::memcpy(this->data, t_buff.get(), this->total_size);

    /*  Done.  */
    *success = true;
}

/*
 * Copy constructor taking a reference to another F_image and creating
 * a new instance.
 * Inputs:
 *   image - Float image that we will be copying into the new image
 */
g_image::F_image::F_image(const F_image &image)
    : x_dim(image.x_dim)
    , y_dim(image.y_dim)
    , channels(image.channels)
    , total_size(image.total_size)
{
    this->data = new float[total_size];
    std::copy(image.data, image.data + image.total_size, this->data);
}

g_image::F_image::F_image()
{
    this->x_dim = 0;
    this->y_dim = 0;
    this->channels = 0;
    this->total_size = 0;
    this->data = 0;
}

/*
 * Reset
 *
 */
bool g_image::F_image::reset(std::string filename)
{
    bool success = false;
    /*  Open input stuff  */
    std::unique_ptr<OpenImageIO::ImageInput> input(
                    OpenImageIO::ImageInput::open(filename));

    if(!input.get())
        return success;

    /*  Get all of our informations  */
    const OpenImageIO::ImageSpec &spec = input->spec();
    this->x_dim = spec.width;
    this->y_dim = spec.height;
    this->channels = spec.nchannels;
    this->total_size = this->x_dim * this->y_dim * this->channels;
    std::unique_ptr<float[]> t_buff(new float[this->total_size]);


    /*  Read in input data  */
    input->read_image(OpenImageIO::TypeDesc::FLOAT, t_buff.get());

    /*  Close input file  */
    input->close();
    
    this->data = new float[this->total_size];
    
    std::copy(t_buff.get(), t_buff.get() + this->total_size, this->data);
    // std::memcpy(this->data, t_buff.get(), this->total_size);

    /*  Done.  */
    success = true;

    return success;
}

void g_image::F_image::replicate(const F_image &image)
{
    if(total_size != image.total_size)
    {
        total_size = image.total_size;
        delete[] data;
        data = new float[total_size];
    }
    x_dim = image.x_dim;
    y_dim = image.y_dim;
    channels = image.channels;
    std::copy(image.data, image.data + image.total_size, this->data);
}

/*
 * Destructor: Clears the data array stored in the class
 */
g_image::F_image::~F_image()
{
    delete[] data;
}

/*  Getter functions  */

/*
 * Returns the width dimension of the image
 */
uint32_t g_image::F_image::get_x_dim() const
{
    return x_dim;
}

/*
 * Returns the height dimension of the image
 */
uint32_t g_image::F_image::get_y_dim() const
{
    return y_dim;
}

/*
 * Returns the number of channels the image contains
 */
uint32_t g_image::F_image::get_channels() const
{
    return channels;
}

/*
 * Returns the total size of the data stored in the image
 */
uint32_t g_image::F_image::get_total_size() const
{
    return total_size;
}

/*
 * Writes the image out to a file using the OpenImageIO library.
 * Input:
 *   filename - filename of the file to be written to
 */
void g_image::F_image::write(const std::string &filename)
{
    /*  Open file  */
    OpenImageIO::ImageSpec spec(this->x_dim, this->y_dim, this->channels,
                                OpenImageIO::TypeDesc::FLOAT);
    std::unique_ptr<OpenImageIO::ImageOutput> output(
                             OpenImageIO::ImageOutput::create(filename));

    output->open(filename, spec);

    if(output.get())
    {
        /*  Write information  */
        output->write_image(OpenImageIO::TypeDesc::FLOAT, this->data);

        output->close();
    }
}

/*
 * Copies a buffer into the internal data store of the image class.
 * Input:
 *   buffer - data to copy in
 *   buff_len - length of the data to be copied in
 */
bool g_image::F_image::copy_buffer_in(float *buffer, unsigned int buff_len)
{
    bool result = false;
    if(buffer != 0 && buff_len <= this->total_size)
    {
        std::copy(buffer, buffer + buff_len, this->data);
        result = true;
    }

    return result;
}

/*
 * Fills a buffer with the data stored in the internal data store.
 * Input:
 *   buffer - buffer to store data in
 *   buff_len - length of the buffer
 */
bool g_image::F_image::fill_buffer(float *buffer, unsigned int buff_len)
{
    bool result = false;

    if(buff_len >= this->total_size && buffer != 0)
    {
        std::copy(this->data, this->data + this->total_size, buffer);
        result = true;
    }

    return result;
}
