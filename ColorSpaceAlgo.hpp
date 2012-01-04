/*
 * ColorSpaceAlgo.hpp
 *
 *  Created on: 3 sept. 2011
 *      Author: Viridian
 */

#ifndef COLORSPACEALGO_HPP_
#define COLORSPACEALGO_HPP_

#include "ColorSpace.hpp"

namespace Imaging
{
	namespace Colors
	{
		//Convert from RGB [0..255]^3 to XYZ
		XYZPixel RGBToXYZ(RGBPixel rgb);

		//Convert from RGB [0..255]^3 to CIE Lab
		LabPixel RGBToLAB(RGBPixel rgb);

		//Convert from Lab to XYZ
		//Reference white D50
		XYZPixel LabToXYZ(LabPixel Lab);

		//Convert from Lab to RGB [0..255]^3
		//Reference white D50
		RGBPixel LabToRGB(LabPixel Lab);
	}
}

#endif /* COLORSPACEALGO_HPP_ */
