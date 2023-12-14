#pragma once

typedef unsigned int uint;

#include <map>
#include <vector>
#include <array>
#include <tuple>
#include <fstream>
#include "Noise.h"

#define WORLEY_NR_CHANNELS 3

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
};
