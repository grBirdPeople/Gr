#ifndef		_H_GRHASHMAP_
#define		_H_GRHASHMAP_

#include	"grCommon.h"



// HashNode
//////////////////////////////////////////////////
template<typename T>
struct HashNode
{
	HashNode	( const uInt key, T value )
		: m_Key		( key )
		, m_Value	( value )
	{}

	sInt	m_Key;
	T		m_Value;
};


// grHashMap
//////////////////////////////////////////////////
template<typename T>
class grHashMap
{
public:

	grHashMap( const uInt maxSize )
		: m_MaxSize	( maxSize )
		, m_NowSize	( 0 )
	{
		m_vecNode.reserve( m_MaxSize );
		for ( uInt i = 0; i < m_MaxSize; ++i )
		{
			m_vecNode.push_back( new HashNode<T>( -1, nullptr ) );
		}

		m_vecUsedKeys.reserve( m_MaxSize );
	}

	~grHashMap( void ) {}

	void	Put	( const uInt key, const T value )
	{
		if ( m_NowSize == m_MaxSize )
		{
#ifdef DEBUG
			std::cerr << "grHashMap::Insert(): HashMap is full" << std::endl;
#endif // DEBUG
			return;
		}

		sInt hashIdx = HashKey( key );
		HashNode<T>* pNode = m_vecNode[ hashIdx ];

		while ( pNode->m_Key != -1 )
		{
			if ( pNode->m_Key == key )
			{
#ifdef DEBUG
				std::cerr << "grHashMap::Insert(): Key already exists" << std::endl;
#endif // DEBUG
				return;
			}

			++hashIdx;
			hashIdx %= m_MaxSize;
			pNode = m_vecNode[ hashIdx ];
		}

		m_vecNode[ hashIdx ] = new HashNode<T>( key, value );
		m_vecUsedKeys.push_back( key );
	}

	T		Get	( const uInt key )
	{
		uInt count = 0;
		sInt hashIdx = HashKey( key );
		HashNode<T>* pNode = m_vecNode[ hashIdx ];
		while ( pNode->m_Key == -1 || pNode->m_Key != key )
		{
			++count;
			if ( count == m_MaxSize )
			{
#ifdef DEBUG
				std::cerr << "grHashMap::Get(): Key does not exist" << std::endl;
#endif // DEBUG
				return nullptr;
			}

			++hashIdx;
			hashIdx %= m_MaxSize;
			pNode = m_vecNode[ hashIdx ];
		}

		return pNode->m_Value;
	}

	//////////////////////////////////////////////////

private:



	//////////////////////////////////////////////////

	uInt HashKey( const uInt key )
	{
		return key % m_MaxSize;
	}

	//////////////////////////////////////////////////

	uInt				m_MaxSize,
						m_NowSize;

	std::vector<HashNode<T>*>	m_vecNode;
	std::vector<uInt>			m_vecUsedKeys;

};

#endif		// _H_GRHASHMAP_
