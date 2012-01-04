/*
 * ColorSpace.hpp
 *
 *  Created on: 3 sept. 2011
 *      Author: Hamrouni Ghassen
 */

#ifndef COLORSPACE_HPP_
#define COLORSPACE_HPP_

#include <cmath>

namespace Imaging
{
	namespace Colors
	{
		//Represent a pixel in the Lab color space
		struct LabPixel
		{
			LabPixel()
			{
				L = a = b = 0;
			}

			LabPixel(double L, double a, double b)
			{
				this->L = L;
				this->b = b;
				this->a = a;
			}

			//lightness
			double L;

			//Colors
			double a, b;
		};

		//Represent a pixel in the XYZ color space
		struct XYZPixel
		{
			XYZPixel()
			{
				X = Y = Z = 0;
			}

			XYZPixel(double X, double Y, double Z)
			{
				this->X = X;
				this->Y = Y;
				this->Z = Z;
			}

			double X, Y, Z;
		};

		//Represent a pixel in the XYZ color space
		struct RGBPixel
		{
			RGBPixel()
			{
				R = G = B = 0;
			}

			RGBPixel(double R, double G, double B)
			{
				this->R = R;
				this->G = G;
				this->B = B;
			}

			double R, G, B;

			void normalize()
			{
				R = R / 255.0;
				G = G / 255.0;
				B = B / 255.0;
			}

			void to_sRGB()
			{
				R = linear(R);
				G = linear(G);
				B = linear(B);
			}

			void to_sRGB_inverse()
			{
				R = linearRGBinv(R);
				G = linearRGBinv(G);
				B = linearRGBinv(B);
			}

		private:
			float linearRGBinv(float v) {
				if (v <= 0.0031308)
					return v * 12.92;

				return 1.055 * (powf(v, 1 / 2.4)) - 0.055;
			}

			float linear(float V) {
				if (V <= 0.04045)
					return V / 12.92;

				return powf(((V + 0.055) / 1.055), 2.4);
			}
		};
	}
}


#endif /* COLORSPACE_HPP_ */
