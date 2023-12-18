#pragma once

#include <vector>
#include <limits>

typedef unsigned int uint;

template<class T>
class Noise
{
public:
  Noise(uint x, uint y, uint c);
  Noise(uint x, uint y, uint z, uint c);

  uint res_x;
  uint res_y;
  uint res_z;
  uint nr_chl;

  inline T distance_to_val(double distance) const;

protected:
  virtual void gen_img();

  std::vector<T> img_data;
  inline uint idx(uint x, uint y, uint c) const;
  inline uint idx(uint x, uint y, uint z, uint c) const;

public:
  inline const T* get_data() const;
  inline std::vector<T> get_vec();
  std::vector<T> get_channel(uint c) const;
  void set_channel(uint c, std::vector<T>& data);
  void set_channel(uint c, std::vector<T>&& data);

  // per channel operations
  void offset_channel(uint c, float s);
  void scale_channel(uint c, float s);

  void add_channel(uint c, std::vector<T>& data);
  void add_channel(uint c, std::vector<T>&& data);

  void multiply_channel(uint c, std::vector<T>& data);
  void multiply_channel(uint c, std::vector<T>&& data);

  // operators on all channels
  void invert();
  void scale(float s);
};


template<class T>
inline const T* Noise<T>::get_data() const
{
  return img_data.data();
}

template<class T>
inline std::vector<T> Noise<T>::get_vec()
{
  return img_data;
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

template<>
inline unsigned char Noise<unsigned char>::distance_to_val(double distance) const
{
  return static_cast <unsigned char> (distance * std::numeric_limits<unsigned char>::max());
}

template<>
inline float Noise<float>::distance_to_val(double distance) const
{
  return static_cast <float> (distance);
}
