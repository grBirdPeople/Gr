#ifndef		_GRSTRUCTS_H_
#define		_GRSTRUCTS_H_

#include	<memory>
#include	<assert.h>
#include	<chrono>
#include	<mutex>

#include	"grCommon.h"


namespace grStruct
{
	// Array // Dynamic array // Basic thread safety
	//////////////////////////////////////////////////
	template <typename T>
	class Array
	{
	public:
		Array()
			: m_upArr( std::make_unique<T[]>( 0 ) )
			, m_Size( 0 )
			, m_Count( 0 )
		{}
		Array( const size_t size )
			: m_upArr( std::make_unique<T[]>( size ) )
			, m_Size( size )
			, m_Count( 0 )
		{}
		Array( const Array<T>& rArr )
		{
			Cpy( rArr );
		}
		Array<T>& operator=( const Array<T>& rArr )
		{
			Cpy( rArr );
			return *this;
		}
		Array( Array<T>&& rArr ) noexcept = delete;
		Array<T>& operator=( Array<T>&& rArr ) noexcept = delete;

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
		inline void Cpy( const Array<T>& rArr )
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

	// LoopQue // FIFO // Basic thread safety
	//////////////////////////////////////////////////
	template<typename T>
	class LoopQue
	{
	public:
		LoopQue()
			: m_puArr		( std::make_unique<T[]>( 0 ) )
			, m_Capacity	( 0 )
			, m_Size		( 0 )
			, m_StrtIdx		( 0 )
		{}
		LoopQue( const sizeT capacity )
			: m_puArr		( std::make_unique<T[]>( capacity ) )
			, m_Capacity	( capacity )
			, m_Size		( 0 )
			, m_StrtIdx		( 0 )
		{}
		LoopQue( const LoopQue& que )
		{
			Cpy( que );
		}
		LoopQue& operator=( const LoopQue& que )
		{
			Cpy( que );
			return *this;
		}
		LoopQue( LoopQue&& que ) noexcept = delete;
		LoopQue& operator=( LoopQue&& que ) noexcept = delete;

		inline const sizeT Capacity( void ) const
		{
			return m_Capacity;
		}
		inline const sizeT Size( void ) const
		{
			return m_Size;
		}
		inline bool Push( const T& rT )
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			if ( m_Size == m_Capacity )
				return false;

			sizeT idx = m_Size + m_StrtIdx;
			if ( idx > m_Capacity - 1 )
				idx -= m_Capacity;

			++m_Size;
			m_puArr[ idx ] = rT;
			return true;
		}
		inline bool Pop( T& rT )	// Last element
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			if ( m_Size < 1 )
				return false;

			sizeT idx = m_StrtIdx++;
			if ( m_StrtIdx > m_Capacity - 1 )
				m_StrtIdx -= m_Capacity;

			--m_Size;
			rT = m_puArr[ idx ];
			return true;
		}
		inline void Reset( const sizeT size )
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			m_puArr.reset( new T[ size ] );
			m_Capacity = size;
			m_StrtIdx = 0;
			m_Size = 0;
		}

	private:
		inline void Cpy( const LoopQue<T>& que )
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			m_puArr.reset( new T[ que.m_Capacity ] );
			for( sizeT i = 0; i < que.m_Capacity; ++i )
				m_puArr[ i ] = que.m_puArr[ i ];

			m_Capacity = que.m_Capacity;
			m_StrtIdx = que.m_StrtIdx;
			m_Size = que.m_Size;
		}

		pU<T[]> m_puArr;
		std::mutex m_Lock;
		sizeT m_Capacity, m_Size, m_StrtIdx;
	};

	// STimer // Simply create an instance at the start of a block
	//////////////////////////////////////////////////
	struct STimer
	{
		enum class ETimeType
		{
			NS = 0,
			MS,
			S
		};

		STimer( const char* name = "Timer", ETimeType timeType = ETimeType::MS )
			: Start		( std::chrono::high_resolution_clock::now() )
			, Name		( name )
			, TimeType	( timeType )
		{}

		~STimer()
		{
			std::chrono::high_resolution_clock::duration delta = std::chrono::high_resolution_clock::now() - Start;
			switch ( TimeType )
			{
				case ETimeType::NS: std::cout << Name << " (ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>( delta ).count() << std::endl; break;
				case ETimeType::MS: std::cout << Name << " (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>( delta ).count() << std::endl; break;
				case ETimeType::S: std::cout << Name << " (s): " << std::chrono::duration_cast<std::chrono::seconds>( delta ).count() << std::endl; break;
			}
		}

		STimer( const STimer& ) = delete;
		STimer( STimer&& ) noexcept = delete;
		STimer& operator=( const STimer& ) = delete;
		STimer& operator=( STimer&& ) noexcept = delete;

	private:
		std::chrono::steady_clock::time_point Start;
		const char* Name;
		ETimeType TimeType;
	};


	// Experimantal stuff
	////////////////////////////////////////////////////////////////////////////////////////////////////


	// LinearActivity
	//////////////////////////////////////////////////
	template<typename T>
	class LinearActivity
	{
	public:

		LinearActivity( const intU capacity )
			: Arr			( new T[ capacity ]() )
			, m_Capacity	( capacity )
			, m_Size		( 0 )
			, m_Active		( 0 )
		{}
		~LinearActivity( void )
		{
			delete[] Arr.release();
		}
		LinearActivity( const LinearActivity & ) = delete;
		LinearActivity& operator=( const LinearActivity& ) = delete;
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
	struct HashNode
	{
		HashNode	( const intU key = -1, T value = nullptr )
			: m_Key		( key )
			, m_Value	( value )
		{}

		intS	m_Key;
		T		m_Value;
	};


	// HashMap // Experimental and not really tested
	// More like a hash set but eh // Uses buckets // Open-adress where only unique unsigned int's are allowed as keys
	//////////////////////////////////////////////////
	template<typename T>
	class HashMap
	{
	public:
		// TODO: Linera probing is used for search (if non unique keys where allowed).
		// Maybe add alts. dependent of map size like Plus 3 Rehash, Qaudratic Probing (failed)^2 or Double Hashing
		HashMap( const intU maxSize )
			: m_MaxSize	( maxSize )
			, m_NowSize	( 0 )
		{
			m_vecNode.reserve( m_MaxSize );
			for ( intU i = 0; i < m_MaxSize; ++i )
				m_vecNode.push_back( new HashNode<T>() );

			m_vecUsedKeys.reserve( m_MaxSize );
			for ( intU i = 0; i < m_MaxSize; ++i )
				m_vecUsedKeys.push_back( -1 );
		}
		~HashMap( void ) {}

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

		inline T Get( const intU key )
		{
			HashNode<T>* pNode = Find( key );
			assert( pNode != nullptr && "grHashMap::Get(): Key does not exist" );
			return pNode->m_Value;
		}

		inline void Del( const intU key )
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

	private:
		inline intS HashKey( const intU key )
		{
			return key % m_MaxSize;
		}

		inline HashNode<T>* Find( const intU key )
		{
			intU count = 0;
			intU hashIdx = HashKey( key );
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

		std::vector<HashNode<T>*>	m_vecNode;
		std::vector<intS>			m_vecUsedKeys;

		intU	m_MaxSize,
				m_NowSize;
	};
}

#endif	// _GRSTRUCTS_H_
