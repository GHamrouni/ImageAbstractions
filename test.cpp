/*
 * main.cpp
 *
 *  Created on: 3 sept. 2011
 *      Author: Ghassen Hamrouni
 */

#include "AnisotropicDiffusion.h"
#include "SmoothedStep.h"
#include <iostream>

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>

using namespace Imaging;


int main(int argc, char **argv)
{
    rgb8_image_t img;
    jpeg_read_image(std::string("test.jpg"), img);
    
    rgb8_image_t img_out(img);

    anisotropicDiffusion(const_view(img), view(img_out), 16, 5.6f, 1/8.0);
	
    jpeg_write_view("diffX.jpg", view(img_out));
	
	quantizeColors(const_view(img_out), view(img_out), 3.5 , 4);
	
    jpeg_write_view("quant.jpg", view(img_out));

    return 0;
}
