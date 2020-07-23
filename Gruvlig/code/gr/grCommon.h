#ifndef		_GRCOMMON_H_
#define		_GRCOMMON_H_

//////////////////////////////////////////////////

#include	<array>
#include	<assert.h>
#include	<iostream>
#include	<map>
#include    <math.h>
#include    <memory>
#include	<string>
#include	<vector>

//////////////////////////////////////////////////

template<typename T> using pU = std::unique_ptr<T>;
template<typename T> using pS = std::shared_ptr<T>;
template<typename T> using pW = std::weak_ptr<T>;
template<typename T, typename Y> using pR = std::pair<T, Y>;
template<typename T> using vE = std::vector<T>;

using str = std::string;
using sizeT = uint64_t;	// std::size_t is a unknown unsigned integer so might not compile on everything
using intS = int64_t;
using intU = uint64_t;

//////////////////////////////////////////////////

#endif		//_GRCOMMON_H_
