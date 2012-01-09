/*
 * ColorSpaceAlgo.cpp
 *
 *  Created on: 3 sept. 2011
 *      Author: Hamrouni Ghassen
 */

#include "ColorSpaceAlgo.hpp"

namespace Imaging
{
	namespace Colors
	{
			const float epsilonXYZ = 0.008856;
			const float kXYZ = 903.3;

			XYZPixel RGBToXYZ(RGBPixel rgb) {
				//Normalize (set colors from [0..255] to [0..1])
				rgb.normalize();

				//Convert to sRGB
				rgb.to_sRGB();

				XYZPixel xyz;

				//Convert Linear RGB to XYZ
				xyz.X = 0.4124564 * rgb.R + 0.3575761 * rgb.G + 0.1804375 * rgb.B;
				xyz.Y = 0.2126729 * rgb.R + 0.7151522 * rgb.G + 0.0721750 * rgb.B;
				xyz.Z = 0.0193339 * rgb.R + 0.1191920 * rgb.G + 0.9503041 * rgb.B;

				return xyz;
			}

			float fxyz(float xr) {
				if (xr > epsilonXYZ)
					return powf(xr, 1.0 / 3.0);

				return 7.787 * xr + 16 / 116.0;
			}

			LabPixel RGBToLAB(RGBPixel rgb) {
				//Convert RGB to XYZ
				XYZPixel xyz = RGBToXYZ(rgb);

				//Reference white D50
				float Xr = 95.05 / 100.0;
				float Yr = 100.000 / 100.0;
				float Zr = 108.88 / 100.0;

				float xr = xyz.X / Xr;
				float yr = xyz.Y / Yr;
				float zr = xyz.Z / Zr;

				//Lightness
				float L;
				if (yr > epsilonXYZ)
					L = 116 * powf(yr, 1.0 / 3.0) - 16.0;
				else
					L = (kXYZ * yr);


				float a = 500 * (fxyz(xr) - fxyz(yr));
				float b = 200 * (fxyz(yr) - fxyz(zr));

				LabPixel Lab;
				Lab.L = L;
				Lab.a = a;
				Lab.b = b;

				return Lab;
			}

			float fxr(float f) {
				if (powf(f, 3) > epsilonXYZ)
					return powf(f, 3);

				return (116 * f - 16) / kXYZ;
			}

			float fyr(float L) {
				if (L > epsilonXYZ * kXYZ)
					return powf((L + 16) / 116.0, 3);

				return L / kXYZ;
			}

			float fzr(float f) {
				if (powf(f, 3) > epsilonXYZ)
					return powf(f, 3);

				return (116 * f - 16) / kXYZ;
			}

			XYZPixel LabToXYZ(LabPixel Lab)
			{
				float L  = Lab.L;
				float a_ = Lab.a;
				float b_ = Lab.b;

				float fy = (L + 16) / 116.0;
				float fz = fy - b_ / 200.0;
				float fx = a_ / 500.0 + fy;

				float xr = fxr(fx);
				float yr = fyr(L);
				float zr = fzr(fz);

				//Reference white D50
				float Xr = 95.05 / 100.0;
				float Yr = 100.000 / 100.0;
				float Zr = 108.88 / 100.0;

				XYZPixel xyz;
				xyz.X = xr * Xr;
				xyz.Y = yr * Yr;
				xyz.Z = zr * Zr;

				return xyz;
			}


			RGBPixel LabToRGB(LabPixel Lab)
			{
				XYZPixel xyz = LabToXYZ(Lab);

				RGBPixel rgb;

				rgb.R =  3.2404542 * xyz.X + -1.5371385 * xyz.Y + -0.4985314 * xyz.Z;
				rgb.G = -0.9692660 * xyz.X +  1.8760108 * xyz.Y +  0.0415560 * xyz.Z;
				rgb.B =  0.0556434 * xyz.X + -0.2040259 * xyz.Y +  1.0572252 * xyz.Z;

				rgb.to_sRGB_inverse();

				rgb.R = rgb.R * 255;
				rgb.B = rgb.B * 255;
				rgb.G = rgb.G * 255;

				return rgb;
			}
	}
}


