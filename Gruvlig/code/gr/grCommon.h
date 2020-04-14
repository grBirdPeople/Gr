#ifndef		_GRCOMMON_H_
#define		_GRCOMMON_H_

//////////////////////////////////////////////////

#include	<assert.h>
#include	<iostream>
#include	<map>
#include    <math.h>
#include    <memory>
#include	<string>
#include	<vector>

//////////////////////////////////////////////////

template<typename U> using uPtr = std::unique_ptr<U>;
template<typename S> using sPtr = std::shared_ptr<S>;

using str = std::string;

using sizeT = std::size_t;
using sInt = int64_t;
using uInt = uint64_t;

//////////////////////////////////////////////////

#endif		//_GRCOMMON_H_
