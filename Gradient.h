#pragma once

#include <stdio.h>

//Adobe GIL headers
#include <boost/gil/color_convert.hpp>
#include <boost/gil/pixel.hpp>
#include <boost/gil/image_view_factory.hpp>

#include <cmath>

namespace Imaging {

using namespace boost::gil;

template<typename SrcView, typename point_t, typename Color>
static float gradient(const SrcView& src, const point_t& point, Color channel) {
	point_t pR, pL, pU, pD;

	if (point.x == 0)
		pL = point_t(0, 0);
	else
		pL = point_t(-1, 0);

	if (point.y == 0)
		pD = point_t(0, 0);
	else
		pD = point_t(0, -1);

	auto xRight = src.xy_at(point + point_t(1, 0));
	auto xLeft = src.xy_at(point + pL);

	auto yUp = src.xy_at(point + point_t(0, 1));
	auto yDown = src.xy_at(point + pD);

	auto xx = fabs(get_color(*xRight, channel) - get_color(*xLeft, channel));
	auto yy = fabs(get_color(*yUp, channel) - get_color(*yDown, channel));

	return sqrt((powf(xx, 2) + powf(yy, 2)));
}

template<typename Pixel>
static Pixel intensity(const Pixel& src) {
	auto c = (get_color(src, red_t()) + get_color(src, green_t())
			+ get_color(src, blue_t())) / 3.0;

	return Pixel(c, c, c, 1);
}

template<typename Pixel>
static Pixel threshold(const Pixel& src, float n) {
	const unsigned char maxT = 255;
	const unsigned char minT = 0;

	auto c = (get_color(src, red_t()) + get_color(src, green_t())
			+ get_color(src, blue_t())) / 3.0;

	if (c > n)
		return Pixel(maxT, maxT, maxT, maxT);

	return Pixel(minT, minT, minT, minT);
}

}
