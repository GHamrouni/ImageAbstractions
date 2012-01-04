//
//
// 	Smoothed step function (Quantization)
//
//  http://www.cs.northwestern.edu/~holger/Research/papers/videoabstraction.pdf
//  http://www.kyprianidis.com/p/tpcg2008/jkyprian-tpcg2008.pdf

#pragma once

#include <stdio.h>

//Adobe GIL headers
#include <boost/gil/color_convert.hpp>
#include <boost/gil/pixel.hpp>
#include <boost/gil/image_view_factory.hpp>

#include <cmath>

#include "ColorSpaceAlgo.hpp"

namespace Imaging {

using namespace boost::gil;

float nearestBin(int c, int binNum) {
	float binWidth = 100.0 / binNum;
	return (int) (((int) (c / 100.0 * binNum)) * (binWidth + 0.5));
}

float smoothedStep(float c, float phi, int binNum) {
	float qNearest = nearestBin(c, binNum);
	float dQ = 100.0 / binNum;

	return qNearest + (dQ / 2.0f) * tanh(phi * (c - qNearest) / 100.0);
}

template<typename SrcView, typename DstView>
void quantizeColors(const SrcView& src, const DstView& dst, float phi,
		int binNum) {
	typedef pixel<typename channel_type<SrcView>::type, rgba_layout_t> rgba_pixel_t;

	auto srcT = color_converted_view<rgba_pixel_t>(src);

	quantizeColorsRGBA(srcT, dst, phi, binNum);
}

template<typename SrcView, typename DstView>
void quantizeColorsRGBA(const SrcView& src, const DstView& dst, float phi,
		int binNum) {
	typedef pixel<typename channel_type<SrcView>::type, rgba_layout_t> rgba_pixel_t;
	typedef point2<ptrdiff_t> point_t;

	for (int y = 0; y < src.height(); ++y) {
		auto src_it = src.row_begin(y);
		auto dst_it = dst.row_begin(y);

		for (int x = 0; x < src.width(); ++x) {

			Colors::RGBPixel rgb;

			rgb.R = get_color(*src.xy_at(point_t(x, y)), red_t());
			rgb.G = get_color(*src.xy_at(point_t(x, y)), green_t());
			rgb.B = get_color(*src.xy_at(point_t(x, y)), blue_t());


			Colors::LabPixel lab = Colors::RGBToLAB(rgb);

			float L = smoothedStep(lab.L, phi, binNum);
			lab.L = fmax(fmin(L, 100), 0);

			rgb = Colors::LabToRGB(lab);

			rgb.R = fmax(fmin(rgb.R, 255), 0);
			rgb.G = fmax(fmin(rgb.G, 255), 0);
			rgb.B = fmax(fmin(rgb.B, 255), 0);

			dst_it[x] = rgba_pixel_t(rgb.R, rgb.G, rgb.B, 255);
		}
	}
}
}
