/*
 * AnisotropicDiffusion.h
 *
 * Anisotropic Diffusion filter (Perona–Malik diffusion)
 *
 *  Created on: 3 sept. 2011
 *      Author: Ghassen Hamrouni
 */

#pragma once

#include <stdio.h>

//Adobe GIL headers
#include <boost/gil/color_convert.hpp>
#include <boost/gil/pixel.hpp>
#include <boost/gil/image_view_factory.hpp>

#include <cmath>

#include "ColorSpace.hpp"

namespace Imaging {

using namespace boost::gil;

template<typename SrcView, typename DstView>
void anisotropicDiffusion(const SrcView& src, const DstView& dst, int iteration,
		float K, float lambda) {
	typedef pixel<typename channel_type<SrcView>::type, rgba_layout_t> rgba_pixel_t;

	auto srcT = color_converted_view<rgba_pixel_t>(src);

	anisotropicDiffusionRGBA(srcT, dst, K, lambda);

	for (int i = 0; i < iteration - 1; i++) {
		anisotropicDiffusionRGBA(dst, dst, K, lambda);
	}
}

template<typename SrcView, typename DstView>
void anisotropicDiffusionRGBA(const SrcView& src, const DstView& dst, float K,
		float lambda) {
	typedef pixel<typename channel_type<SrcView>::type, rgba_layout_t> rgba_pixel_t;
	typedef point2<ptrdiff_t> point_t;

	for (int y = 0; y < src.height(); ++y) {
		auto dst_it = dst.row_begin(y);

		for (int x = 0; x < src.width(); ++x) {
			Colors::RGBPixel p = diffusion(src, point_t(x, y), K, lambda, 1);

			dst_it[x] = rgba_pixel_t(p.R, p.G, p.B, 1);
		}
	}
}

//the conductivity function
float g(float grad, float K) {
	return expf(-powf((grad / K), 2));
}

float diffusion(float channelValue, float currentChannelValue, float K)
{
	float delta = channelValue - currentChannelValue;
	float grad = g(delta, K);

	return delta * grad;
}

template<typename SrcView, typename point_t>
static Colors::RGBPixel diffusion(const SrcView& src, const point_t& point, float K, float lambda, int radius) {

	const auto currentPixel = *src.xy_at(point);

	float currentR = get_color(currentPixel, red_t());
	float currentG = get_color(currentPixel, green_t());
	float currentB = get_color(currentPixel, blue_t());

	float sum_diffR = 0;
	float sum_diffG = 0;
	float sum_diffB = 0;

	for (int i = -radius; i<=radius; i++)
	{
		for (int j = -radius; j<=radius; j++)
		{
			point_t newPt = point + point_t(i, j);

			if ((newPt.y < src.height()) && (newPt.x < src.width()) && (newPt.x >= 0) && (newPt.y >= 0))
			{
				auto pixR = get_color(*src.xy_at(newPt), red_t());
				auto pixG = get_color(*src.xy_at(newPt), green_t());
				auto pixB = get_color(*src.xy_at(newPt), blue_t());

				sum_diffR += diffusion(pixR, currentR, K);
				sum_diffG += diffusion(pixG, currentG, K);
				sum_diffB += diffusion(pixB, currentB, K);
			}
		}
	}

	float diffR = fmin(fabs(currentR + lambda  * sum_diffR), 255);
	float diffG = fmin(fabs(currentG + lambda  * sum_diffG), 255);
	float diffB = fmin(fabs(currentB + lambda  * sum_diffB), 255);

	Colors::RGBPixel rgb(diffR, diffG, diffB);

	return rgb;
}
}
