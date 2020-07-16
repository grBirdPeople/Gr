#ifndef		_GRSTRUCTS_H_
#define		_GRSTRUCTS_H_

#include	<memory>
#include	<assert.h>
#include	<chrono>
#include	<mutex>

#include	"grCommon.h"


namespace grStruct
{
	// grArr // Dynamic array // // Thread safe // Not safe for move copy and move assign
	//////////////////////////////////////////////////
	template <typename T>
	class grArr
	{
	public:
		grArr()
			: m_upArr( std::make_unique<T[]>( 0 ) )
			, m_Size( 0 )
			, m_Count( 0 )
		{}
		grArr( const size_t size )
			: m_upArr( std::make_unique<T[]>( size ) )
			, m_Size( size )
			, m_Count( 0 )
		{}
		grArr( const grArr<T>& rArr )
		{
			Cpy( rArr );
		}
		grArr<T>& operator=( const grArr<T>& rArr )
		{
			Cpy( rArr );
			return *this;
		}
		inline void Reset( const size_t size )
		{
			std::lock_guard<std::mutex> lock( m_Lock );
			m_upArr.reset( new T[ size ] );
			m_Size = 0;
			m_Count = 0;
		}
		inline void Push( const T& value )
		{
			assert( m_Size > m_Count && "grArr::Push: Array full" );
			std::lock_guard<std::mutex> lock( m_Lock );
			m_upArr[ m_Count++ ] = value;
		}
		inline void Set( const size_t idx, const T& value )
		{
			assert( idx < m_Size && "grArr::Set: Idx out of range" );
			std::lock_guard<std::mutex> lock( m_Lock );
			m_upArr[ m_Count++ ] = value;
		}
		inline const T& Get( const size_t idx ) const
		{
			assert( idx < m_Count && "grArr::Get: Idx out of range" );
			return m_upArr[ idx ];
		}
		inline const size_t Size() const
		{
			return m_Size;
		}
		inline const size_t Count() const
		{
			return m_Count;
		}

	private:
		inline void Cpy( const grArr<T>& rArr )
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			m_upArr.reset( new T[ m_Size ] );
			for ( size_t i = 0; i < rArr.m_Count; ++i )
				m_upArr[ i ] = rArr.m_upArr[ i ];

			m_Size = rArr.m_Size;
			m_Count = rArr.m_Count;
		}

		std::unique_ptr<T[]> m_upArr;
		std::mutex m_Lock;
		size_t m_Size, m_Count;
	};

	// STimerOneShot // Simply create an instance at the start of a block
	//////////////////////////////////////////////////
	struct grSTimerOneShot
	{
		enum class ETimer
		{
			NS = 0,
			MS,
			S
		};

		grSTimerOneShot( ETimer timeType = ETimer::MS )
			: Start		( std::chrono::high_resolution_clock::now() )
			, End		( Start )
			, Duration	( 0.0f )
			, TimeType	( timeType )
		{}

		~grSTimerOneShot( void )
		{
			End = std::chrono::high_resolution_clock::now();
			Duration = End - Start;
			switch( TimeType )
			{
				case ETimer::NS: printf( "Time (ns): %g \n", Duration.count() * 1000000000.0f ); break;
				case ETimer::MS: printf( "Time (ms): %g \n", Duration.count() * 1000.0f ); break;
				case ETimer::S: printf( "Time (s): %g \n", Duration.count() ); break;
			}
		}

		grSTimerOneShot( const grSTimerOneShot& ) = delete;
		grSTimerOneShot& operator=( const grSTimerOneShot& ) = delete;

	private:
		std::chrono::time_point<std::chrono::steady_clock> Start,End;
		std::chrono::duration<float> Duration;
		ETimer TimeType;
	};

	// grLoopQue // First in/First out // Thread safe // Not move safe
	//////////////////////////////////////////////////
	template<typename T>
	class grLoopQue
	{
	public:
		grLoopQue()
			: m_puArr	( std::make_unique<T[]>( 0 ) )
			, m_Size	( 0 )
			, m_StrtIdx	( 0 )
			, m_Count	( 0 )
		{}
		grLoopQue( const intU size )
			: m_puArr	( std::make_unique<T[]>( size ) )
			, m_Size	( size )
			, m_StrtIdx	( 0 )
			, m_Count	( 0 )
		{}
		grLoopQue( const grLoopQue& que )
		{
			Cpy( que );
		}
		grLoopQue& operator=( const grLoopQue& que )
		{
			Cpy( que );
			return *this;
		}

		inline const intU Size( void ) const
		{
			return m_Size;
		}
		inline const intU Count( void ) const
		{
			return m_Count;
		}
		inline void Push( const T& rT )
		{
#ifdef DEBUG
			assert( m_Count != m_Size && "grLoopQue::Push() : Size maxed out." );
#endif // DEBUG

			intU insrtIdx = m_StrtIdx + m_Count;

			std::lock_guard<std::mutex> lock( m_Lock );

			if ( m_Size - 1 < insrtIdx )
				insrtIdx = insrtIdx - m_Size;

			m_puArr[ insrtIdx ] = rT;
			++m_Count;
		}
		inline T Pull( void )	// Last element
		{
#ifdef DEBUG
			assert( ( m_Count > 0 ) && "grLoopQue::Pull(): Que is empty" );
#endif // DEBUG

			sizeT idx = m_StrtIdx;

			std::unique_lock<std::mutex> lock( m_Lock );

			++m_StrtIdx;
			if ( m_StrtIdx > m_Size - 1 )
				m_StrtIdx = m_StrtIdx - m_Size;

			--m_Count;

			lock.unlock();

			return m_puArr[ idx ];
		}
		inline void Reset( const sizeT size )
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			m_puArr.reset( new T[ size ] );
			m_Size = size;
			m_StrtIdx = 0;
			m_Count = 0;
		}

	private:
		inline void Cpy( const grLoopQue<T>& que )
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			m_puArr.reset( new T[ que.m_Size ] );
			for( sizeT i = 0; i < que.m_Size; ++i )
				m_puArr[ i ] = que.m_puArr[ i ];

			m_Size = que.m_Size;
			m_StrtIdx = que.m_StrtIdx;
			m_Count = que.m_Count;
		}

		pU<T[]> m_puArr;
		std::mutex m_Lock;
		sizeT m_Size, m_StrtIdx, m_Count;
	};


	// grLinearActivity // Experimental and not really tested
	//////////////////////////////////////////////////
	template<typename T>
	class grLinearActivity
	{
	public:

		grLinearActivity( const intU capacity )
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
#ifdef DEBUG
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
#ifdef DEBUG
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
#ifdef DEBUG
			if ( m_Active == m_Size )
			{
				std::puts( "grStruct::grActivateArray::Activate(): All elements already active. Ref to the last element in array returned" );
				return Arr[ m_Active - 1 ];
			}
#endif	// DEBUG

			++m_Active;
			return Arr[ m_Active - 1 ];
		}
		void Deactivate( const intU idx )
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
		pU<T[]> Arr;

		sizeT	m_Capacity,
				m_Size,
				m_Active;
	};


	// HashNode
	//////////////////////////////////////////////////
	template<typename T>
	struct grHashNode
	{
		grHashNode	( const intU key = -1, T value = nullptr )
			: m_Key		( key )
			, m_Value	( value )
		{}

		intS	m_Key;
		T		m_Value;
	};


	// grHashMap // Experimental and not really tested
	// More like a hash set but eh // Uses buckets // Open-adress where only unique unsigned int's are allowed as keys
	//////////////////////////////////////////////////
	template<typename T>
	class grHashMap
	{
	public:
		// TODO: Linera probing is used for search (if non unique keys where allowed).
		// Maybe add alts. dependent of map size like Plus 3 Rehash, Qaudratic Probing (failed)^2 or Double Hashing
		grHashMap( const intU maxSize )
			: m_MaxSize	( maxSize )
			, m_NowSize	( 0 )
		{
			m_vecNode.reserve( m_MaxSize );
			for ( intU i = 0; i < m_MaxSize; ++i )
				m_vecNode.push_back( new grHashNode<T>() );

			m_vecUsedKeys.reserve( m_MaxSize );
			for ( intU i = 0; i < m_MaxSize; ++i )
				m_vecUsedKeys.push_back( -1 );
		}
		~grHashMap( void ) {}

		inline intU Size( void )
		{
			return m_NowSize;
		}

		inline bool Exists( const intU key )
		{
			return ( HashKey( key ) == true ) ? true : false;
		}

		inline std::vector<intU> UsedKeys( void )
		{
			std::vector<intU> keys;
			for ( intU i = 0; i < m_vecUsedKeys.size(); ++i )
			{
				if ( m_vecUsedKeys[ i ] > -1 )
				{
					keys.push_back( ( intU )m_vecUsedKeys[ i ] );
				}
			}

			return keys;
		}

		inline void Put( const intU key, const T value )
		{
			if ( m_NowSize == m_MaxSize )
			{
#ifdef DEBUG
				std::cerr << "grHashMap::Insert(): HashMap is full" << std::endl;
#endif // DEBUG
				return;
			}

			intS hashIdx = HashKey( key );
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

		inline T Get( const intU key )
		{
			grHashNode<T>* pNode = Find( key );
			assert( pNode != nullptr && "grHashMap::Get(): Key does not exist" );
			return pNode->m_Value;
		}

		inline void Del( const intU key )
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

	private:
		inline intS HashKey( const intU key )
		{
			return key % m_MaxSize;
		}

		inline grHashNode<T>* Find( const intU key )
		{
			intU count = 0;
			intU hashIdx = HashKey( key );
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

		std::vector<grHashNode<T>*>	m_vecNode;
		std::vector<intS>			m_vecUsedKeys;

		intU	m_MaxSize,
				m_NowSize;
	};
}

#endif	// _GRSTRUCTS_H_
