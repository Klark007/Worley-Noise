#pragma once

#include "Worley.h"

template<class T>
class WorleyFBM : public Noise<T>
{
public:
  WorleyFBM(uint res_x, uint res_y, std::vector<std::pair<uint, uint> > grid_res, uint nr_oct);
private:
  Worley<T> w;
  
  std::vector<std::pair<uint, uint> > grid_res;
  uint nr_oct;

  void gen_img() override;
};

template<class T>
inline WorleyFBM<T>::WorleyFBM(uint res_x, uint res_y, std::vector<std::pair<uint, uint>> grid_res, uint nr_oct)
  : Noise<T>(res_x, res_y, WORLEY_NR_CHANNELS), grid_res {grid_res}, nr_oct {nr_oct}, w {Worley<T>(res_x,res_y,grid_res)}
{
  gen_img();
}

template<class T>
inline void WorleyFBM<T>::gen_img()
{
  this->img_data = w.get_vec();

  float a = 0.25;
  float f = 2.0;

  for (uint i = 1; i < nr_oct; i++) {
    for (auto& p : grid_res) {
      p.first  *= f;
      p.second *= f;
    }
    w.regenerate(grid_res);

    w.scale(a);
    this->add(w.get_vec());

    a *= 0.25;
  }
}

template class WorleyFBM<float>;
