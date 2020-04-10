#ifndef		_H_GRHASHMAP_
#define		_H_GRHASHMAP_

#include	"grCommon.h"


// HashNode
//////////////////////////////////////////////////
template<typename T>
struct HashNode
{
	HashNode	( const uInt key = -1, T value = nullptr )
		: m_Key		( key )
		, m_Value	( value )
	{}

	sInt	m_Key;
	T		m_Value;
};


// grHashMap // Uses buckets / open-adress where only unique unsigned int's are allowed
//////////////////////////////////////////////////
template<typename T>
class grHashMap
{
public:	// TODO: Linera probing is used for search (if non unique keys where allowed). Perhaps add alts. dependent of map size like Plus 3 Rehash, Qaudratic Probing (failed)^2 or Double Hashing

	// cTor
	//////////////////////////////////////////////////
	grHashMap	( const uInt maxSize )
		: m_MaxSize	( maxSize )
		, m_NowSize	( 0 )
	{
		m_vecNode.reserve( m_MaxSize );
		for ( uInt i = 0; i < m_MaxSize; ++i )
		{
			m_vecNode.push_back( new HashNode<T>() );
		}

		m_vecUsedKeys.reserve( m_MaxSize );
		for ( uInt i = 0; i < m_MaxSize; ++i )
		{
			m_vecUsedKeys.push_back( -1 );
		}
	}


	// dTor
	//////////////////////////////////////////////////
	~grHashMap	( void )	{}


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
	Put			( const uInt key, const T value )
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

		HashNode<T>* pNode = m_vecNode[ hashIdx ];
		//while ( pNode->m_Key != -1 )
		//{
		//	++hashIdx;
		//	hashIdx %= m_MaxSize;
		//	pNode = m_vecNode[ hashIdx ];
		//}

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
		HashNode<T>* pNode = Find( key );
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
		HashNode<T>* pNode = Find( key );
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
	inline	HashNode<T>*
	Find		( const uInt key )
	{
		uInt count = 0;
		uInt hashIdx = HashKey( key );
		HashNode<T>* pNode = m_vecNode[ hashIdx ];
		while ( pNode->m_Key == -1 || pNode->m_Key != key )
		{
			++count;
			if ( count == m_MaxSize )
			{
				return nullptr;
			}

			hashIdx %= m_MaxSize;
			pNode = m_vecNode[ hashIdx ];
			++hashIdx;
		}

		return pNode;
	}

	//////////////////////////////////////////////////

	std::vector<HashNode<T>*>	m_vecNode;
	std::vector<sInt>			m_vecUsedKeys;

	uInt						m_MaxSize,
								m_NowSize;

};

#endif		// _H_GRHASHMAP_
