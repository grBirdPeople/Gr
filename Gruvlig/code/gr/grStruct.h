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


	// grLinearActivity
	//////////////////////////////////////////////////
	template<typename T>
	class grLinearActivity
	{
	public:

		grLinearActivity( const uInt capacity )
			: Arr			( new T[ capacity ]() )
			, m_Capacity	( capacity )
			, m_Size		( 0 )
			, m_Active		( 0 )
		{}
		~grLinearActivity( void )
		{
			delete[] Arr.release();
		}
		grLinearActivity( const grLinearActivity & ) = delete;
		grLinearActivity& operator=( const grLinearActivity& ) = delete;
		T& operator[]( const sizeT idx )
		{
#ifdef	DEBUG
			assert( idx >= 0 && "grStruct::grReuseArray::[]: Idx was less then zero" );
			assert( idx < m_Capacity && "grStruct::grReuseArray::[]: Idx was out of range" );
#endif	// DEBUG

			return Arr[ idx ];
		}

		const sizeT Size( void ) const
		{
			return m_Size;
		}
		const sizeT Active( void ) const
		{
			return m_Active;
		}
		void Push( T value )
		{
#ifdef	DEBUG
			if ( m_Size >= m_Capacity )
			{
				std::puts( "grStruct::grActivateArray::Push(): Max elements reached" );
				return;
			}
#endif	// DEBUG

			Arr[ m_Size ] = value;
			++m_Size;
		}
		T& Activate( void )
		{
#ifdef	DEBUG
			if ( m_Active == m_Size )
			{
				std::puts( "grStruct::grActivateArray::Activate(): All elements already active. Ref to the last element in array returned" );
				return Arr[ m_Active - 1 ];
			}
#endif	// DEBUG

			++m_Active;
			return Arr[ m_Active - 1 ];
		}
		void Deactivate( const uInt idx )
		{
#ifdef	DEBUG
			if ( idx < 0 || idx >= m_Active )
			{
				std::puts( "grStruct::grActivateArray::Activate(): Idx was out of range" );
				return;
			}
#endif	// DEBUG

			Arr[ idx ] = Arr[ m_Active - 1 ];
			--m_Active;
		}

	private:

		uP<T[]> Arr;

		sizeT	m_Capacity,
				m_Size,
				m_Active;
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


	// grHashMap // More like a hash set but eh // Uses buckets // Open-adress where only unique unsigned int's are allowed as keys // Never tested for reals for reals
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
