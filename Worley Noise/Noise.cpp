#include "Noise.h"

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
Noise<T>::~Noise()
{
  //delete[] img_data;
}

template class Noise<unsigned char>;