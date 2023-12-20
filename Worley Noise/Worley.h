#pragma once

#include <map>
#include <vector>
#include <tuple>
#include "Noise.h"

#define WORLEY_NR_CHANNELS 3

// assume we only use integer types as T
template<class T>
class Worley : public Noise<T>
{
public:
	// res gives resolution of image and grid gives size of the points used to generate worley noise with grid << res
	Worley(uint res_x, uint res_y, std::vector<std::pair<uint, uint> > grid_res);
	void print();
private:
	std::vector<std::pair<uint, uint> > grid_res;

	std::map<std::tuple<uint, uint, uint>, std::pair<double, double> > grid_points;
	void generate_points();

	void gen_img() override;
	double distance_to_point_in_grid(double px, double py, int ix, int iy, uint ic) const;
public:
	inline void regenerate(std::vector<std::pair<uint, uint> > grid_res);
};

template<class T>
inline void Worley<T>::regenerate(std::vector<std::pair<uint, uint>> grid_res)
{
	this->grid_res = grid_res;
	gen_img();
}
