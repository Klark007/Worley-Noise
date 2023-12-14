#include "Noise.h"
#include <cassert>

template<class T>
Noise<T>::Noise(uint x, uint y, uint c)
  : res_x{ x }, res_y{ y }, res_z { 1 }, nr_chl{ c }
{
  img_data = std::vector<T>(x * y * c);
}

template<class T>
Noise<T>::Noise(uint x, uint y, uint z, uint c)
  : res_x {x}, res_y {y}, res_z {z}, nr_chl {c}
{
  img_data = std::vector<T>(x * y * z * c);
}

template<class T>
std::vector<T> Noise<T>::get_channel(uint c) const
{
	assert(0 <= c && c < nr_chl);
	std::vector<T> data = std::vector<T>(res_x*res_y*res_z);

	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				data.at(i) = img_data.at(idx(x, y, z, c));
			}
		}
	}

	return data;
}

template<class T>
void Noise<T>::set_channel(uint c, std::vector<T>& data)
{
	assert(0 <= c && c < nr_chl);
	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				img_data.at(idx(x, y, z, c)) = data.at(i);
			}
		}
	}
}


template class Noise<unsigned char>;