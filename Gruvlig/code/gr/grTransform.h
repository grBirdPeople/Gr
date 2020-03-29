#ifndef		_GRTRANSFORM_H_
#define		_GRTRANSFORM_H_


template<typename T>
class grTransform
{
public:

	grTransform<T>( T* pOwner );
	~grTransform( void );

	T& GetOwner( void );

private:

	T*	m_pOwner;

};

#endif	// _GRTRANSFORM_H_
