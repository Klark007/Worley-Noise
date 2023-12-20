#include "Noise.h"

#include <algorithm>
#include <limits>
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
void Noise<T>::gen_img()
{
	std::for_each(img_data.begin(), img_data.end(), [](T& v){v=0;} );
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

template<class T>
void Noise<T>::set_channel(uint c, std::vector<T>&& data)
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

template<class T>
void Noise<T>::offset_channel(uint c, float s)
{
	assert(0 <= c && c < nr_chl);
	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				img_data.at(idx(x, y, z, c)) = s + img_data.at(idx(x, y, z, c));
			}
		}
	}
}

template<class T>
void Noise<T>::scale_channel(uint c, float s) {
	assert(0 <= c && c < nr_chl);
	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				img_data.at(idx(x, y, z, c)) = s * img_data.at(idx(x, y, z, c));
			}
		}
	}
}

template<class T>
void Noise<T>::add_channel(uint c, std::vector<T>& data)
{
	assert(0 <= c && c < nr_chl);
	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				img_data.at(idx(x, y, z, c)) += data.at(i);
			}
		}
	}
}

template<class T>
void Noise<T>::add_channel(uint c, std::vector<T>&& data)
{
	assert(0 <= c && c < nr_chl);
	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				img_data.at(idx(x, y, z, c)) += data.at(i);
			}
		}
	}
}

template<class T>
void Noise<T>::multiply_channel(uint c, std::vector<T>& data)
{
	assert(0 <= c && c < nr_chl);
	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				img_data.at(idx(x, y, z, c)) *= data.at(i);
			}
		}
	}
}

template<class T>
void Noise<T>::multiply_channel(uint c, std::vector<T>&& data)
{
	assert(0 <= c && c < nr_chl);
	for (uint z = 0; z < res_z; z++) {
		for (uint y = 0; y < res_y; y++) {
			for (uint x = 0; x < res_x; x++) {
				uint i = (y + z * res_y) * res_x + x;
				img_data.at(idx(x, y, z, c)) *= data.at(i);
			}
		}
	}
}

template<class T>
void Noise<T>::invert() {
	auto inv = [](T& v) { v = std::numeric_limits<T>::max() - v; };
	std::for_each(img_data.begin(), img_data.end(), inv);
}

void Noise<float>::invert() {
	auto inv = [](float& v) { v = 1.0 - v; };
	std::for_each(img_data.begin(), img_data.end(), inv);
}

template<class T>
void Noise<T>::scale(float s) {
	auto scl = [&s](T& v) { v = v*s; };
	std::for_each(img_data.begin(), img_data.end(), scl);
}

template<class T>
void Noise<T>::add(std::vector<T>& data)
{
	int idx = 0;
	auto add = [&idx,&data](T& v) { 
		v += data.at(idx); 
		idx++; 
	};
	std::for_each(img_data.begin(), img_data.end(), add);
}

template<class T>
void Noise<T>::add(std::vector<T>&& data)
{
	int idx = 0;
	auto add = [&idx, &data](T& v) {
		v += data.at(idx);
		idx++;
	};
	std::for_each(img_data.begin(), img_data.end(), add);
}

template class Noise<unsigned char>;
template class Noise<float>;