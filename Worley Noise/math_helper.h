#pragma once

template<class T>
inline T remap(const T v, const  T l_o, const  T h_o, const  T l_n, const  T h_n) {
	return (v - l_o) * (h_n - l_n) / (h_o - l_o) + l_n;
}