#include "Worley.h"

#include<cmath> // for pow, sqrt
#include <stdlib.h> // for rand
#include <algorithm> // for min
#include <limits>
#include <iostream>

#include "stb_image.h"
#include "math_helper.h"

template<class T>
Worley<T>::Worley(uint res_x, uint res_y, std::vector<std::pair<uint, uint> > grid_res)
	: grid_res {grid_res}
{
	img_data = new T[res_x * res_y * WORLEY_NR_CHANNELS];
	r_x = res_x;
	r_y = res_y;
	
	generate_points();
	generate_img();
}

template<class T>
Worley<T>::~Worley()
{
	delete[] img_data;
}

template<class T>
inline void Worley<T>::generate_points()
{
	for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
		uint g_x = grid_res[c].first;
		uint g_y = grid_res[c].second;

		for (uint x = 0; x < g_x; x++) {
			for (uint y = 0; y < g_y; y++) {
				double d1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				double d2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				grid_points.insert({ {x,y,c}, {x+d1,y+d2} });
			}
		}
	}
}

template<class T>
void Worley<T>::generate_img()
{
	std::cerr << "Size mult by: " << (unsigned long)std::numeric_limits<T>::max() << std::endl;

	std::cout << "P3\n" << r_x << ' ' << r_y << "\n255\n";

	double max_dist = 0.0;
	for (uint y = 0; y < r_y; y++) {
		for (uint x = 0; x < r_x; x++) {
			for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
				uint g_x = grid_res[c].first;
				uint g_y = grid_res[c].second;

				double px = remap((double)x+0.5, 0.0, (double)r_x, 0.0, (double)g_x);
				double py = remap((double)y+0.5, 0.0, (double)r_y, 0.0, (double)g_y);

				//std::cerr << px << "," << py << ": ";


				int ix = (int) floor(px);
				int iy = (int) floor(py);

				double distance = std::min({
					distance_to_point_in_grid(px, py, ix, iy, c),
					distance_to_point_in_grid(px, py, ix - 1, iy, c),
					distance_to_point_in_grid(px, py, ix + 1, iy, c),
					distance_to_point_in_grid(px, py, ix, iy - 1, c),
					distance_to_point_in_grid(px, py, ix, iy + 1, c),

					distance_to_point_in_grid(px, py, ix - 1, iy - 1, c),
					distance_to_point_in_grid(px, py, ix + 1, iy - 1, c),
					distance_to_point_in_grid(px, py, ix - 1, iy + 1, c),
					distance_to_point_in_grid(px, py, ix + 1, iy + 1, c)
				});
				max_dist = std::max(distance, max_dist);

				// assume we only use integer types as T
				// max distance is 1.5 in x and y

				T val = static_cast <T> (distance / std::sqrt(2) * std::numeric_limits<T>::max());
				//std::cerr << distance <<"," << (unsigned long)val << std::endl;
				img_data[gen_img_idx(x, y, c)] = val;
				if (WORLEY_NR_CHANNELS == 1) {
					std::cout << (unsigned long)val << ' ' << (unsigned long)val << ' ' << (unsigned long)val << std::endl;
				} else if (WORLEY_NR_CHANNELS == 3) {
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

	std::cerr << "Assumed max distance: " << std::sqrt(2) << "; Actual max: " << max_dist << std::endl;
}

template<class T>
double Worley<T>::distance_to_point_in_grid(double px, double py, int ix, int iy, uint ic) const
{
	uint g_x = grid_res[ic].first;
	uint g_y = grid_res[ic].second;

	std::pair<double, double> gp = grid_points.at({ ix % g_x, iy % g_y, ic });
	
	double off_x = 0.0;
	double off_y = 0.0;

	if (ix < 0) {
		off_x = (double) g_x;
	} else if (ix >= g_x) {
		off_x = -((double) g_y);
	}

	if (iy < 0) {
		off_y = (double) g_y;
	} else if (iy >= g_y) {
		off_y = -((double) g_x);
	}

	return std::sqrt(std::pow(px + off_x - gp.first, 2) + std::pow(py + off_y - gp.second, 2));
}

// assume we only use integer types as T
template class Worley<unsigned char>;