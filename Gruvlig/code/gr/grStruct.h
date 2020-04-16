#ifndef		_GRSTRUCTS_H_
#define		_GRSTRUCTS_H_

#include	"grCommon.h"


namespace grStruct
{
	// grLoopQue // Dynamic array which copies the data // Push/Pull // First in/First out
	//////////////////////////////////////////////////
	template<typename T>
	class grLoopQue
	{
	public:

		grLoopQue( const uInt size )
			: arrT		( new T[ size ] )
			, Size		( size )
			, StrtIdx	( 0 )
			, Active	( 0 )
		{}
		~grLoopQue( void )
		{
			if ( arrT != nullptr )
				delete[] arrT;
		}

		grLoopQue( grLoopQue const& ) = delete;
		grLoopQue& operator=( grLoopQue const& ) = delete;

		inline const uInt Quantity( void ) const
		{
			return Active;
		}
		inline void Push( const T& rT )
		{
			if ( Active == Size )
			{
#ifdef DEBUG
				std::puts( "grLoopQue::Push(): Size maxed out. No element was added.\n" );
#endif // DEBUG
				return;
			}

			uInt insrtIdx = StrtIdx + Active;
			if ( Size - 1 < insrtIdx )
				insrtIdx = insrtIdx - Size;

			arrT[ insrtIdx ] = rT;
			++Active;
		}
		inline T Pull( void )
		{
			assert( ( Active > 0 ) && "grLoopQue::Pull(): Que is empty" );

			sizeT idx = StrtIdx;
			++StrtIdx;
			if ( Size - 1 < StrtIdx )
				StrtIdx = StrtIdx - Size;

			--Active;
			return arrT[ idx ];
		}
		inline T Pop( void )
		{
			assert( ( Active > 0 ) && "grLoopQue::Pop(): Que is empty" );

			sizeT idx = StrtIdx + ( Size - 1 );
			if ( idx > ( Size - 1 ) )
				idx =idx - Size;

			--Active;
			return arrT[ idx ];
		}
		inline void Reset( void )
		{
			StrtIdx = 0;
			Active = 0;
		}

	private:

		T*	arrT;

		sizeT	Size,
				StrtIdx,
				Active;
	};


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


	// TODO: This should be updated to use ptr arrays instead of vectors
	// grHashMap // Uses buckets / Open-adress where only unique unsigned int's are allowed as keys // Never tested for reals for reals
	//////////////////////////////////////////////////
	template<typename T>
	class grHashMap
	{
	public:	// TODO: Linera probing is used for search (if non unique keys where allowed). Perhaps add alts. dependent of map size like Plus 3 Rehash, Qaudratic Probing (failed)^2 or Double Hashing

		grHashMap( const uInt maxSize )
			: m_MaxSize	( maxSize )
			, m_NowSize	( 0 )
		{
			m_vecNode.reserve( m_MaxSize );
			for ( uInt i = 0; i < m_MaxSize; ++i )
				m_vecNode.push_back( new grHashNode<T>() );

			m_vecUsedKeys.reserve( m_MaxSize );
			for ( uInt i = 0; i < m_MaxSize; ++i )
				m_vecUsedKeys.push_back( -1 );
		}
		~grHashMap( void ) {}

		inline uInt Size( void )
		{
			return m_NowSize;
		}
		inline bool Exists( const uInt key )
		{
			return ( HashKey( key ) == true ) ? true : false;
		}
		inline std::vector<uInt> UsedKeys( void )
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
		inline void Put( const uInt key, const T value )
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
		inline T Get( const uInt key )
		{
			grHashNode<T>* pNode = Find( key );
			assert( pNode != nullptr && "grHashMap::Get(): Key does not exist" );
			return pNode->m_Value;
		}
		inline void Del( const uInt key )
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

		inline sInt HashKey( const uInt key )
		{
			return key % m_MaxSize;
		}
		inline grHashNode<T>* Find( const uInt key )
		{
			uInt count = 0;
			uInt hashIdx = HashKey( key );
			grHashNode<T>* pNode = m_vecNode[ hashIdx ];
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

		std::vector<grHashNode<T>*>	m_vecNode;
		std::vector<sInt>			m_vecUsedKeys;

		uInt	m_MaxSize,
				m_NowSize;

	};
}


#endif	// _GRSTRUCTS_H_
