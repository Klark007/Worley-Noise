#include "Worley.h"

#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>

#include "stb_image.h"
#include "math_helper.h"

template<class T>
Worley<T>::Worley(uint res_x, uint res_y, uint grid_x, uint grid_y)
{
	img_data = new T[res_x * res_y * WORLEY_NR_CHANNELS];
	r_x = res_x;
	r_y = res_y;

	g_x = grid_x;
	g_y = grid_y;

	generate_points1();

	int j = 1000;
	auto begin1 = std::chrono::steady_clock::now();
	for (int i = 0; i < j; i++) {
		generate_points1();
	}
	auto end1 = std::chrono::steady_clock::now();
	auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1).count();
	std::cout << "Duration with better random: " << t1/j << std::endl;

	generate_points2();

	auto begin2 = std::chrono::steady_clock::now();
	for (int i = 0; i < j; i++) {
		generate_points2();
	}
	auto end2 = std::chrono::steady_clock::now();
	auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count();
	std::cout << "Duration with other random: " << t2/j << std::endl;

}

template<class T>
Worley<T>::~Worley()
{
	delete[] img_data;
}

template<class T>
inline void Worley<T>::generate_points()
{
	std::random_device rd;
	std::uniform_real_distribution<> dis(0.0, 1.0);

	for (uint x = 0; x < g_x; x++) {
		for (uint y = 0; y < g_y; y++) {
			for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
				double d1 = dis(rd);
				double d2 = dis(rd);
				grid_points.insert({ {x,y,c}, {d1,d2} });
			}
		}
	}
}

template<class T>
inline void Worley<T>::generate_points1()
{
	std::random_device rd;
	std::uniform_real_distribution<> dis(0.0, 1.0);

	for (uint x = 0; x < g_x; x++) {
		for (uint y = 0; y < g_y; y++) {
			for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
				double d1 = dis(rd);
				double d2 = dis(rd);
				grid_points.insert({ {x,y,c}, {d1,d2} });
			}
		}
	}
}

template<class T>
inline void Worley<T>::generate_points2()
{
	for (uint x = 0; x < g_x; x++) {
		for (uint y = 0; y < g_y; y++) {
			for (uint c = 0; c < WORLEY_NR_CHANNELS; c++) {
				double d1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
				double d2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
				grid_points.insert({ {x,y,c}, {d1,d2} });
			}
		}
	}
}

template class Worley<unsigned char>;