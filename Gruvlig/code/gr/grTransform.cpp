#include "grTransform.h"

#include	"grDefine.h"


template<typename T>
inline grTransform<T>::grTransform( T* pOwner )
	: m_pOwner	( pOwner )
{}


template<typename T>
inline grTransform<T>::~grTransform( void )
{
	if ( m_pOwner != nullptr )
	{
		DELANDNULL( m_pOwner );
	}
}

template<typename T>
inline T& grTransform<T>::GetOwner( void )
{
	return *m_pOwner;
}
