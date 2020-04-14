#ifndef		_GRALGO_H_
#define		_GRALGO_H_

#include	"grCommon.h"


// grAlgo
//////////////////////////////////////////////////
namespace grAlgo
{
	// InsrtSort
	//////////////////////////////////////////////////
	template<typename T>
	inline void
	InsrtSort( T* pArr, const uInt size, const bool minMax = true )
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
			
			( minMax == true ) ? minToMax() : maxToMin();
			
			pArr[ indexBefore + 1 ] = currentNum;
		}
	}

	template<typename T>
	inline void
	InsrtSort( std::vector<T>& rVec, const bool minMax = true )
	{
		T currentNum;
		uInt indexBefore;

		auto minToMax = [ & ]()
		{
			while ( ( currentNum < rVec[ indexBefore ] ) && ( indexBefore >= 0 ) )
			{
				rVec[ indexBefore + 1 ] = rVec[ indexBefore ];
				indexBefore = indexBefore - 1;
			}
		};

		auto maxToMin = [ & ]()
		{
			while ( ( currentNum > rVec[ indexBefore ] ) && ( indexBefore >= 0 ) )
			{
				rVec[ indexBefore + 1 ] = rVec[ indexBefore ];
				indexBefore = indexBefore - 1;
			}
		};

		for ( uInt i = 1; i < rVec.size(); ++i )
		{
			currentNum = rVec[ i ];
			indexBefore = i - 1;

			( minMax == true ) ? minToMax() : maxToMin();

			rVec[ indexBefore + 1 ] = currentNum;
		}
	}
}

#endif	//_GRALGO_H_
