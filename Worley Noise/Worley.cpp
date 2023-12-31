#include "Worley.h"

#include<cmath> // for pow, sqrt
#include <stdlib.h> // for rand
#include <algorithm> // for min
#include <limits>
#include <iostream>

#include "stb_image.h"
#include "math_helper.h"

#define WN_OUTPUT_TO_CONSOLE false

template<class T>
Worley<T>::Worley(uint res_x, uint res_y, std::vector<std::pair<uint, uint> > grid_res)
	: Noise<T>(res_x, res_y, WORLEY_NR_CHANNELS), grid_res {grid_res}
{
	gen_img();
}

template<class T>
void Worley<T>::print()
{
	std::cout << "P3\n" << this->res_x << ' ' << this->res_y << "\n255\n";
	for (uint y = 0; y < this->res_y; y++) {
		for (uint x = 0; x < this->res_x; x++) {
			for (uint c = 0; c < this->nr_chl; c++) {
				if (c == 2) {
					std::cout << (unsigned long)(this->img_data.at(this->idx(x, y, c))) << std::endl;
				}
				else {
					std::cout << (unsigned long)(this->img_data.at(this->idx(x, y, c))) << ' ';
				}
			}
		}
	}
}

template<class T>
inline void Worley<T>::generate_points()
{
	grid_points.clear();

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
void Worley<T>::gen_img()
{
	generate_points();

	std::cerr << "Size mult by: " << (unsigned long)std::numeric_limits<T>::max() << std::endl;

#if WN_OUTPUT_TO_CONSOLE
	std::cout << "P3\n" << this->res_x << ' ' << this->res_y << "\n255\n";
#endif

	double max_dist = 0.0;
	for (uint y = 0; y < this->res_y; y++) {
		for (uint x = 0; x < this->res_x; x++) {
			for (uint c = 0; c < this->nr_chl; c++) {
				uint g_x = grid_res[c].first;
				uint g_y = grid_res[c].second;

				double px = remap((double)x+0.5, 0.0, (double)this->res_x, 0.0, (double)g_x);
				double py = remap((double)y+0.5, 0.0, (double)this->res_y, 0.0, (double)g_y);

				//std::cerr << px << "," << py << ": ";


				int ix = (int) floor(px);
				int iy = (int) floor(py);

				double distance = std::min({
					distance_to_point_in_grid(px, py, ix - 1, iy - 1, c),
					distance_to_point_in_grid(px, py, ix,     iy - 1, c),
					distance_to_point_in_grid(px, py, ix + 1, iy - 1, c),
					
					distance_to_point_in_grid(px, py, ix - 1, iy, c),
					distance_to_point_in_grid(px, py, ix,     iy, c),
					distance_to_point_in_grid(px, py, ix + 1, iy, c),
					
					distance_to_point_in_grid(px, py, ix - 1, iy + 1, c),
					distance_to_point_in_grid(px, py, ix,     iy + 1, c),
					distance_to_point_in_grid(px, py, ix + 1, iy + 1, c),
				});
				max_dist = std::max(distance, max_dist);

				T val = this->distance_to_val(distance / std::sqrt(2));
				this->img_data.at(this->idx(x, y, c)) = val;
#if WN_OUTPUT_TO_CONSOLE
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
#endif
			}
		}
	}

	std::cerr << "Assumed max distance: " << std::sqrt(2) << "; Actual max: " << max_dist << std::endl;
}

template<class T>
double Worley<T>::distance_to_point_in_grid(double px, double py, int ix, int iy, uint ic) const
{
	int g_x = (int) grid_res[ic].first;
	int g_y = (int) grid_res[ic].second;
	
	double off_x = 0.0;
	double off_y = 0.0;

	if (ix < 0) {
		off_x = (double) g_x;
	} else if (ix >= g_x) {
		off_x = -((double) g_x);
	}

	if (iy < 0) {
		off_y = (double) g_y;
	} else if (iy >= g_y) {
		off_y = -((double) g_y);
	}
	
	std::pair<double, double> gp = grid_points.at({ (uint) (mod<int>(ix,g_x)), (uint)(mod<int>(iy,g_y))  , ic });

	return std::sqrt(std::pow(px + off_x - gp.first, 2) + std::pow(py + off_y - gp.second, 2));
}

template class Worley<unsigned char>;
template class Worley<float>;