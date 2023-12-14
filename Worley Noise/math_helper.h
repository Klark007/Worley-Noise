#pragma once

template<class T>
inline T remap(const T v, const  T l_o, const  T h_o, const  T l_n, const  T h_n) {
	return (v - l_o) * (h_n - l_n) / (h_o - l_o) + l_n;
}

// % in cpp is remainder not modulo, https://stackoverflow.com/a/2581867
template<typename V>
V mod(const V& a, const V& b)
{
	return (a % b + b) % b;
}