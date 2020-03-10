#ifndef		_GRALGO_H_
#define		_GRALGO_H_

#include	"grCommon.h"


// grAlgo
//////////////////////////////////////////////////
namespace grAlgo
{
	// InsertionSort
	//////////////////////////////////////////////////
	template<typename T>
	inline void
	InsertSort( T* pArr, const uInt size, const uInt sortType = 0 )
	{
		T currentNum;
		uInt indexBefore;
		
		auto minToMax = [ & ]()
		{
			while( ( currentNum < pArr[ indexBefore ] ) && ( indexBefore >= 0 ) )
			{
				pArr[ indexBefore + 1 ]	= pArr[ indexBefore ];
				indexBefore				= indexBefore - 1;
			}
		};
		
		auto maxToMin = [ & ]()
		{
			while( ( currentNum > pArr[ indexBefore ] ) && ( indexBefore >= 0 ) )
			{
				pArr[ indexBefore + 1 ]	= pArr[ indexBefore ];
				indexBefore				= indexBefore - 1;
			}
		};
		
		for( uInt i = 1; i < size; ++i )
		{
			currentNum	= pArr[ i ];
			indexBefore	= i - 1;
			
			( sortType == 0 ) ? minToMax() : maxToMin();
			
			pArr[ indexBefore + 1 ] = currentNum;
		}
	}
}

#endif	//_GRALGO_H_
