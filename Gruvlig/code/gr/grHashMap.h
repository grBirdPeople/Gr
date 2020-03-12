#ifndef		_H_GRHASHMAP_
#define		_H_GRHASHMAP_

#include	"grCommon.h"


// HashNode
//////////////////////////////////////////////////
template<typename T>
struct grHashNode
{
	grHashNode	( const uInt key = -1, T value = nullptr )
		: m_Key		( key )
		, m_Value	( value )
	{}

	sInt	m_Key;
	T		m_Value;
};


// grHashMap // Open adress where only unsigned int's that are unique are allowed as keys
//////////////////////////////////////////////////
template<typename T>
class grHashMap
{
public:	// TODO: No search is used atm as unique keys are only allowed, but perhaps add alts. for Linera Probing dependent of map size like Plus 3 Rehash, Qaudratic Probing (failed)^2 or Double Hashing

	// cTor
	//////////////////////////////////////////////////
	grHashMap	( const uInt maxSize )
		: m_MaxSize	( maxSize )
		, m_NowSize	( 0 )
	{
		m_vecNode.reserve( m_MaxSize );
		for ( uInt i = 0; i < m_MaxSize; ++i )
		{
			m_vecNode.push_back( new grHashNode<T>() );
		}

		m_vecUsedKeys.reserve( m_MaxSize );
		for ( uInt i = 0; i < m_MaxSize; ++i )
		{
			m_vecUsedKeys.push_back( -1 );
		}
	}


	// dTor
	//////////////////////////////////////////////////
	~grHashMap	( void )
	{
		grHashNode<T>* pNode = nullptr;
		for ( uInt i = 0; i < m_vecNode.size(); ++i )
		{
			pNode = m_vecNode[ i ];
			if ( pNode->m_Key != -1 )
			{
				pNode->m_Value = nullptr;
			}
		}

		for ( uInt i = 0; i < m_vecNode.size(); ++i )
		{
			DEL_NULL( m_vecNode[ i ] );
		}
	}


	// Get / Set
	//////////////////////////////////////////////////
	inline	uInt
	Size		( void )
	{
		return m_NowSize;
	}


	inline	bool
	Exists		( const uInt key )
	{
		return ( HashKey( key ) == true ) ? true : false;
	}


	inline	std::vector<uInt>
	UsedKeys	( void )
	{
		std::vector<uInt> keys;
		for ( uInt i = 0; i < m_vecUsedKeys.size(); ++i )
		{
			if ( m_vecUsedKeys[ i ] > -1 )
			{
				keys.push_back( ( uInt )m_vecUsedKeys[ i ] );
			}
		}

		return keys;
	}


	// Put
	//////////////////////////////////////////////////
	inline	void
	Set			( const uInt key, const T value )
	{
		if ( m_NowSize == m_MaxSize )
		{
#ifdef DEBUG
			std::cerr << "grHashMap::Insert(): HashMap is full" << std::endl;
#endif // DEBUG
			return;
		}

		sInt hashIdx = HashKey( key );
		if ( m_vecUsedKeys[ hashIdx ] != -1 )
		{
#ifdef DEBUG
			std::cerr << "grHashMap::Insert(): Key already exists" << std::endl;
#endif // DEBUG
			return;
		}

		grHashNode<T>* pNode = m_vecNode[ hashIdx ];
		pNode->m_Key = key;
		pNode->m_Value = value;
		m_vecUsedKeys[ hashIdx ] = key;
		++m_NowSize;
	}


	// Get
	//////////////////////////////////////////////////
	inline	T
	Get			( const uInt key )
	{
		grHashNode<T>* pNode = Find( key );
		if ( pNode == nullptr )
		{
#ifdef DEBUG
			std::cerr << "grHashMap::Get(): Key does not exist" << std::endl;
#endif // DEBUG
			return nullptr;
		}

		return pNode->m_Value;
	}


	// Get
	//////////////////////////////////////////////////

	inline	void
	Del			( const uInt key )
	{
		grHashNode<T>* pNode = Find( key );
		if ( pNode == nullptr )
		{
#ifdef DEBUG
			std::cerr << "grHashMap::Del(): key does not exist" << std::endl;
#endif // DEBUG
			return;
		}

		pNode->m_Key = -1;
		pNode->m_Value = nullptr;
		m_vecUsedKeys[ HashKey( key ) ] = -1;
		--m_NowSize;
	}

	//////////////////////////////////////////////////

private:

	// HashKey
	//////////////////////////////////////////////////
	inline	sInt
	HashKey		( const uInt key )
	{
		return key % m_MaxSize;
	}

	// Find
	//////////////////////////////////////////////////
	inline	grHashNode<T>*
	Find		( const uInt key )
	{
		grHashNode<T>* pNode = m_vecNode[ HashKey( key ) ];
		if ( pNode->m_Key == -1 )
		{
			return nullptr;
		}

		return pNode;

		// Linear probing saved if non-unique keys would be implemented
		//uInt count = 0;
		//uInt hashIdx = HashKey( key );
		//HashNode<T>* pNode = m_vecNode[ hashIdx ];
		//while ( pNode->m_Key == -1 || pNode->m_Key != key )
		//{
		//	++count;
		//	if ( count == m_MaxSize )
		//	{
		//		return nullptr;
		//	}

		//	hashIdx %= m_MaxSize;
		//	pNode = m_vecNode[ hashIdx ];
		//	++hashIdx;
		//}

		//return pNode;
	}

	//////////////////////////////////////////////////

	std::vector<grHashNode<T>*>	m_vecNode;
	std::vector<sInt>			m_vecUsedKeys;

	uInt						m_MaxSize,
								m_NowSize;

};

#endif		// _H_GRHASHMAP_
