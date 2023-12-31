#pragma once

#include <map>
#include <vector>
#include <array>
#include <tuple>
#include <fstream>
#include "Noise.h"

#define WORLEY_NR_CHANNELS 4

// assume we only use integer types as T
template<class T>
class Worley3D : public Noise<T>
{
public:
	// res gives resolution of image and grid gives size of the points used to generate worley noise with grid << res
	Worley3D(uint res_x, uint res_y, uint res_z, std::vector<std::tuple<uint, uint, uint> > grid_res);
	~Worley3D();
private:
	std::vector<std::ofstream> files;

	std::vector<std::tuple<uint, uint, uint> > grid_res;

	std::array<std::vector<std::tuple<double, double, double> >, WORLEY_NR_CHANNELS> grid_points; // stores the per channel grid in a one dimensional vector
	void generate_points();

	void gen_img() override;
	double distance_to_point_in_grid(double px, double py, double pz, int ix, int iy, int iz, uint ic) const;
public:
	inline void regenerate(std::vector<std::tuple<uint, uint, uint> > grid_res);
};

template<class T>
inline void Worley3D<T>::regenerate(std::vector<std::tuple<uint, uint, uint>> grid_res)
{
	this->grid_res = grid_res;

	for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
		uint g_x = std::get<0>(grid_res[c]);
		uint g_y = std::get<1>(grid_res[c]);
		uint g_z = std::get<2>(grid_res[c]);

		this->grid_points[c].resize(g_z * g_y * g_x);
	}

	gen_img();
}