#pragma once

typedef unsigned int uint;

#include <map>
#include <tuple>

#define WORLEY_NR_CHANNELS 3

template<class T>
class Worley
{
public:
	// res gives resolution of image and grid gives size of the points used to generate worley noise with grid << res
	Worley(uint res_x, uint res_y, uint grid_x, uint grid_y);
	~Worley();

	void generate_points1();
	void generate_points2();
private:
	unsigned int r_x, r_y;
	unsigned int g_x, g_y;

	T* img_data;
	unsigned int gen_img_idx(unsigned int x, unsigned int y, unsigned int c) const;

	std::map<std::tuple<uint, uint, uint>, std::pair<double, double> > grid_points;
	void generate_points();
};

template<class T>
inline unsigned int Worley<T>::gen_img_idx(unsigned int x, unsigned int y, unsigned int c) const
{
	return (y * r_x + x) * WORLEY_NR_CHANNELS + c;
}

