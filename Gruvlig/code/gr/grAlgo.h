#ifndef		_GRALGO_H_
#define		_GRALGO_H_

#include	"grCommon.h"


// grAlgo
//////////////////////////////////////////////////
namespace grAlgo
{
	// InsrtSort // Min To Max or Max To Min
	//////////////////////////////////////////////////
	template<typename T>
	inline void
	InsrtSort( T* pArr, const uInt size, const bool minMax = true )
	{
		T nowNum;
		sizeT idxBefore;
		
		auto minToMax = [ & ]()
		{
			while( ( nowNum < pArr[ idxBefore ] ) && ( idxBefore >= 0 ) )
			{
				pArr[ idxBefore + 1 ]	= pArr[ idxBefore ];
				idxBefore				= idxBefore - 1;
			}
		};
		
		auto maxToMin = [ & ]()
		{
			while( ( nowNum > pArr[ idxBefore ] ) && ( idxBefore >= 0 ) )
			{
				pArr[ idxBefore + 1 ]	= pArr[ idxBefore ];
				idxBefore				= idxBefore - 1;
			}
		};
		
		for( uInt i = 1; i < size; ++i )
		{
			nowNum	= pArr[ i ];
			idxBefore	= i - 1;
			
			( minMax == true ) ? minToMax() : maxToMin();
			
			pArr[ idxBefore + 1 ] = nowNum;
		}
	}
}

#endif	//_GRALGO_H_
