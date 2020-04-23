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

template<typename Q> using uP = std::unique_ptr<Q>;
template<typename W> using sP = std::shared_ptr<W>;

template<typename E> using vE = std::vector<E>;

using str = std::string;

using sizeT = std::size_t;
using sInt = int64_t;
using uInt = uint64_t;

//////////////////////////////////////////////////

#endif		//_GRCOMMON_H_
