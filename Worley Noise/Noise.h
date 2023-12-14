#pragma once

#include <vector>

typedef unsigned int uint;

template<class T>
class Noise
{
public:
  Noise(uint x, uint y, uint c);
  Noise(uint x, uint y, uint z, uint c);
protected:
  uint res_x;
  uint res_y;
  uint res_z;
  uint nr_chl;

  virtual void gen_img() = 0;

  std::vector<T> img_data;
  inline uint idx(uint x, uint y, uint c) const;
  inline uint idx(uint x, uint y, uint z, uint c) const;
public:
  inline const T* get_data() const;
  std::vector<T> get_channel(uint c) const;
  void set_channel(uint c, std::vector<T>& data);
};

template<class T>
inline const T* Noise<T>::get_data() const
{
  return img_data.data();
}

template<class T>
inline uint Noise<T>::idx(uint x, uint y, uint c) const
{
  return idx(x,y,0,c);
}

template<class T>
inline uint Noise<T>::idx(uint x, uint y, uint z, uint c) const
{
  return ((y + z * res_y) * res_x + x) * nr_chl + c;
}