#include "Worley3D.h"

#include<cmath> // for pow, sqrt
#include <stdlib.h> // for rand
#include <algorithm> // for min
#include <limits>

#include <iostream>

#include "stb_image.h"
#include "math_helper.h"


template<class T>
Worley3D<T>::Worley3D(uint res_x, uint res_y, uint res_z, std::vector<std::tuple<uint, uint, uint>> grid_res)
	: grid_res{ grid_res }
{
	img_data = new T[res_x * res_y * res_z * WORLEY_NR_CHANNELS];
	r_x = res_x;
	r_y = res_y;
	r_z = res_z;

	generate_points();
	generate_img();
}

template<class T>
Worley3D<T>::~Worley3D()
{
	delete[] img_data;
}

template<class T>
inline void Worley3D<T>::generate_points()
{
	for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
		uint g_x = std::get<0>(grid_res[c]);
		uint g_y = std::get<1>(grid_res[c]);
		uint g_z = std::get<2>(grid_res[c]);
		
		for (uint x = 0; x < g_x; x++) {
			for (uint y = 0; y < g_y; y++) {
				for (uint z = 0; z < g_z; z++) {
					double d1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					double d2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					double d3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					grid_points.insert({ std::make_tuple(x,y,z,c), std::make_tuple(x + d1,y + d2, z + d3) });
				}
			}
		}
	}
}

template<class T>
void Worley3D<T>::generate_img()
{
	std::cerr << "Size mult by: " << (unsigned long)std::numeric_limits<T>::max() << std::endl;

	uint z = 0;
	std::cout << "P3\n" << r_x << ' ' << r_y << "\n255\n";

	double max_dist = 0.0;
	for (uint y = 0; y < r_y; y++) {
		for (uint x = 0; x < r_x; x++) {
			for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
				uint g_x = std::get<0>(grid_res[c]);
				uint g_y = std::get<1>(grid_res[c]);
				uint g_z = std::get<2>(grid_res[c]);

				double px = remap((double)x + 0.5, 0.0, (double)r_x, 0.0, (double)g_x);
				double py = remap((double)y + 0.5, 0.0, (double)r_y, 0.0, (double)g_y);
				double pz = remap((double)z + 0.5, 0.0, (double)r_z, 0.0, (double)g_z);

				//std::cerr << px << "," << py << ": ";


				int ix = (int)floor(px);
				int iy = (int)floor(py);
				int iz = (int)floor(pz);

				double distance = std::min({
					distance_to_point_in_grid(px, py, pz, ix,     iy, iz - 1, c),
					distance_to_point_in_grid(px, py, pz, ix - 1, iy, iz - 1, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy, iz - 1, c),
					distance_to_point_in_grid(px, py, pz, ix, iy - 1, iz - 1, c),
					distance_to_point_in_grid(px, py, pz, ix, iy + 1, iz - 1, c),

					distance_to_point_in_grid(px, py, pz, ix - 1, iy - 1, iz - 1, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy - 1, iz - 1, c),
					distance_to_point_in_grid(px, py, pz, ix - 1, iy + 1, iz - 1, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy + 1, iz - 1, c),

					distance_to_point_in_grid(px, py, pz, ix,     iy, iz, c),
					distance_to_point_in_grid(px, py, pz, ix - 1, iy, iz, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy, iz, c),
					distance_to_point_in_grid(px, py, pz, ix, iy - 1, iz, c),
					distance_to_point_in_grid(px, py, pz, ix, iy + 1, iz, c),
													
					distance_to_point_in_grid(px, py, pz, ix - 1, iy - 1, iz, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy - 1, iz, c),
					distance_to_point_in_grid(px, py, pz, ix - 1, iy + 1, iz, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy + 1, iz, c),

					distance_to_point_in_grid(px, py, pz, ix,     iy, iz + 1, c),
					distance_to_point_in_grid(px, py, pz, ix - 1, iy, iz + 1, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy, iz + 1, c),
					distance_to_point_in_grid(px, py, pz, ix, iy - 1, iz + 1, c),
					distance_to_point_in_grid(px, py, pz, ix, iy + 1, iz + 1, c),

					distance_to_point_in_grid(px, py, pz, ix - 1, iy - 1, iz + 1, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy - 1, iz + 1, c),
					distance_to_point_in_grid(px, py, pz, ix - 1, iy + 1, iz + 1, c),
					distance_to_point_in_grid(px, py, pz, ix + 1, iy + 1, iz + 1, c),
					});
				max_dist = std::max(distance, max_dist);

				// assume we only use integer types as T
				// max distance is 1.5 in x and y

				T val = static_cast <T> (distance / std::sqrt(3) * std::numeric_limits<T>::max());
				//std::cerr << distance <<"," << (unsigned long)val << std::endl;
				img_data[gen_img_idx(x, y, z, c)] = val;
				if (WORLEY_NR_CHANNELS == 1) {
					std::cout << (unsigned long)val << ' ' << (unsigned long)val << ' ' << (unsigned long)val << std::endl;
				}
				else if (WORLEY_NR_CHANNELS == 3) {
					if (c == 2) {
						std::cout << (unsigned long)val << std::endl;
					}
					else {
						std::cout << (unsigned long)val << ' ';
					}
				}
			}
		}
	}

	std::cerr << "Assumed max distance: " << std::sqrt(3) << "; Actual max: " << max_dist << std::endl;
}

template<class T>
double Worley3D<T>::distance_to_point_in_grid(double px, double py, double pz, int ix, int iy, int iz, uint ic) const
{
	uint g_x = std::get<0>(grid_res[ic]);
	uint g_y = std::get<1>(grid_res[ic]);
	uint g_z = std::get<2>(grid_res[ic]);

	std::tuple<double, double, double> gp = grid_points.at({ ix % g_x, iy % g_y, iz % g_z, ic });

	double off_x = 0.0;
	double off_y = 0.0;
	double off_z = 0.0;

	if (ix < 0) {
		off_x = (double)g_x;
	}
	else if (ix >= g_x) {
		off_x = -((double)g_y);
	}

	if (iy < 0) {
		off_y = (double)g_y;
	}
	else if (iy >= g_y) {
		off_y = -((double)g_x);
	}

	if (iz < 0) {
		off_z = (double)g_z;
	}
	else if (iz >= g_z) {
		off_z = -((double)g_z);
	}

	return std::sqrt(std::pow(px + off_x - std::get<0>(gp), 2) + std::pow(py + off_y - std::get<1>(gp), 2) + (std::pow(pz + off_z - std::get<2>(gp), 2)));
}

// assume we only use integer types as T
template class Worley3D<unsigned char>;