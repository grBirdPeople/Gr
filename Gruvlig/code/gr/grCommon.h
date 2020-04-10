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

typedef	signed int		sInt;
typedef	unsigned int	uInt;
typedef	std::string		str;

//////////////////////////////////////////////////

#endif		//_GRCOMMON_H_
