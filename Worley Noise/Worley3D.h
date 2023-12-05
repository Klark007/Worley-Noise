#pragma once

typedef unsigned int uint;

#include <map>
#include <vector>
#include <array>
#include <tuple>
#include <fstream>

#define WORLEY_NR_CHANNELS 3

// assume we only use integer types as T
template<class T>
class Worley3D
{
public:
	// res gives resolution of image and grid gives size of the points used to generate worley noise with grid << res
	Worley3D(uint res_x, uint res_y, uint res_z, std::vector<std::tuple<uint, uint, uint> > grid_res);
	~Worley3D();
private:
	std::vector<std::ofstream> files;

	unsigned int r_x, r_y, r_z;
	std::vector<std::tuple<uint, uint, uint> > grid_res;

	// TODO: Replace with std::vector
	T* img_data;
	unsigned int gen_img_idx(unsigned int x, unsigned int y, unsigned int z, unsigned int c) const;

	std::array<std::vector<std::tuple<double, double, double> >, WORLEY_NR_CHANNELS> grid_points; // stores the per channel grid in a one dimensional vector
	void generate_points();

	void generate_img();
	double distance_to_point_in_grid(double px, double py, double pz, int ix, int iy, int iz, uint ic) const;
public:
	inline T* get_data() const;
};

template<class T>
inline T* Worley3D<T>::get_data() const
{
	return img_data;
}

template<class T>
inline unsigned int Worley3D<T>::gen_img_idx(unsigned int x, unsigned int y, unsigned int z, unsigned int c) const
{
	return ((y + z * r_y) * r_x + x) * WORLEY_NR_CHANNELS + c;
}
