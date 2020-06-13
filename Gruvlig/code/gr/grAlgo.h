#ifndef		_GRALGO_H_
#define		_GRALGO_H_

#include	"grCommon.h"


// grAlgo
//////////////////////////////////////////////////
namespace grAlgo
{
	template<typename T>
	inline void Swap( T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	template<typename T>
	inline void
	InsrtSort( T* pArr, const intU size, const bool minToMax = true )	// Min To Max or Max To Min
	{
		auto MinToMax = [ & ]( T* arr, const T& nowNum, intS& before )
		{
			while ( ( before >= 0 ) && ( nowNum < arr[ before ] ) )
			{
				arr[ before + 1 ] = arr[ before ];
				before = before - 1;
			}
		};

		auto MaxToMin = [ & ]( T* arr, const T& nowNum, intS& before )
		{
			while ( ( before >= 0 ) && ( nowNum > arr[ before ] ) )
			{
				arr[ before + 1 ] = arr[ before ];
				before = before - 1;
			}
		};

		T nowNum;
		intS idxBefore;
		for ( intU i = 1; i < size; ++i )
		{
			nowNum = pArr[ i ];
			idxBefore = i - 1;
			( minToMax == true ) ? MinToMax( pArr, nowNum, idxBefore ) : MaxToMin( pArr, nowNum, idxBefore );
			pArr[ idxBefore + 1 ] = nowNum;
		}
	}

	template<typename T>
	inline void
	InsrtSort( vE<T>& rVec, const bool minToMax = true )	// Min To Max or Max To Min
	{
		auto MinToMax = [ & ]( vE<T>& rV, const T& nowNum, intS& before )
		{
			while ( ( before >= 0 ) && ( nowNum < rV[ before ] ) )
			{
				rV[ before + 1 ] = rV[ before ];
				before = before - 1;
			}
		};

		auto MaxToMin = [ & ]( vE<T>& rV, const T& nowNum, intS& before )
		{
			while ( ( before >= 0 ) && ( nowNum > rV[ before ] ) )
			{
				rV[ before + 1 ] = rV[ before ];
				before = before - 1;
			}
		};

		T nowNum;
		intS idxBefore;
		for ( intU i = 1; i < rVec.size(); ++i )
		{
			nowNum = rVec[ i ];
			idxBefore = i - 1;
			( minToMax == true ) ? MinToMax( rVec, nowNum, idxBefore ) : MaxToMin( rVec, nowNum, idxBefore );
			rVec[ idxBefore + 1 ] = nowNum;
		}
	}

	template<typename T>
	inline sizeT
	BinarySrch( const T* pArr, const sizeT strtIdx, const sizeT endIdx, const T& rTargetNum )
	{
		if ( endIdx >= strtIdx )
		{
			sizeT mid = strtIdx + ( endIdx - strtIdx ) / 2;

			if ( pArr[ mid ] == rTargetNum )
				return mid;

			if ( pArr[ mid ] > rTargetNum )
				return BinarySrch( pArr, strtIdx, mid - 1, rTargetNum );

			return BinarySrch( pArr, mid + 1, endIdx, rTargetNum );
		}

		return -1;
	}

	template<typename T>
	inline sizeT
	BinarySrch( const vE<T>& rVec, const sizeT strtIdx, const sizeT endIdx, const T& rTargetNum )
	{
		if ( endIdx >= strtIdx )
		{
			sizeT mid = strtIdx + ( endIdx - strtIdx ) / 2;

			if ( rVec[ mid ] == rTargetNum )
				return mid;

			if ( rVec[ mid ] > rTargetNum )
				return BinarySrch( rVec, strtIdx, mid - 1, rTargetNum );

			return BinarySrch( rVec, mid + 1, endIdx, rTargetNum );
		}

		return -1;
	}
}

#endif	//_GRALGO_H_
