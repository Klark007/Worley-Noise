#pragma once

#include "Worley3D.h"

template<class T>
class Worley3DFBM : public Noise<T>
{
public:
    Worley3DFBM(uint res_x, uint res_y, uint res_z, std::vector<std::tuple<uint, uint, uint> > grid_res, uint nr_oct);
private:
    Worley3D<T> w;

    std::vector<std::tuple<uint, uint, uint> > grid_res;
    uint nr_oct;

    void gen_img() override;
};

template<class T>
inline Worley3DFBM<T>::Worley3DFBM(uint res_x, uint res_y, uint res_z, std::vector<std::tuple<uint, uint, uint> > grid_res, uint nr_oct)
    : Noise<T>(res_x, res_y, res_z, WORLEY_NR_CHANNELS), grid_res{ grid_res }, nr_oct{ nr_oct }, w{ Worley3D<T>(res_x,res_y,res_z,grid_res) }
{
    gen_img();
}

template<class T>
inline void Worley3DFBM<T>::gen_img()
{
    this->img_data = w.get_vec();

    float a = 0.25;
    float f = 2.0;

    for (uint i = 1; i < nr_oct; i++) {
        for (auto& t : grid_res) {
            std::get<0>(t) *= f;
            std::get<1>(t) *= f;
            std::get<2>(t) *= f;
        }
        w.regenerate(grid_res);

        w.scale(a);
        this->add(w.get_vec());
        
        a *= 0.25;
    }
}

template class Worley3DFBM<float>;
