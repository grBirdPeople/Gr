#ifndef		_GRSTRUCTS_H_
#define		_GRSTRUCTS_H_

#include	<memory>
#include	<assert.h>
#include	<chrono>
#include	<mutex>

#include	"grCommon.h"


namespace grStruct
{
	// grArrayDyn // Dynamic array // Basic thread safety
	//////////////////////////////////////////////////
	template <typename T>
	class grArrDyn
	{
	public:
		grArrDyn()
			: m_upArr( std::make_unique<T[]>( 0 ) )
			, m_Capacity( 0 )
		{}

		grArrDyn( const sizeT capacity )
			: m_upArr( std::make_unique<T[]>( capacity ) )
			, m_Capacity( capacity )
		{}

		grArrDyn( const grArrDyn<T>& rArr )
		{
			Cpy( rArr );
		}

		grArrDyn<T>& operator=( const grArrDyn<T>& rArr )
		{
			Cpy( rArr );
			return *this;
		}

		grArrDyn( grArrDyn<T>&& rArr ) noexcept = delete;
		grArrDyn<T>& operator=( grArrDyn<T>&& rArr ) noexcept = delete;

		inline void Reset( const sizeT capacity )
		{
			std::lock_guard<std::mutex> lock( m_Lock );
			m_upArr.reset( new T[ capacity ] );
			m_Capacity = capacity;
		}

		inline T& Read( const sizeT idx ) const
		{
#ifdef DEBUG			
			assert( idx < m_Capacity && "grArr::Read: Idx out of range" );
#endif // DEBUG
			return m_upArr[ idx ];
		}

		inline void Write( const sizeT idx, const T& value )
		{
#ifdef DEBUG						
			assert( idx < m_Capacity && "grArr::Write: Idx out of range" );
#endif // DEBUG
			std::lock_guard<std::mutex> lock( m_Lock );
			m_upArr[ idx ] = value;
		}

		inline sizeT Capacity() const
		{
			return m_Capacity;
		}

	private:
		inline void Cpy( const grArrDyn<T>& rArr )
		{
			std::lock_guard<std::mutex> lock( m_Lock );

			m_upArr.reset( new T[ rArr.m_Capacity ] );
			for ( sizeT i = 0; i < rArr.m_Capacity; ++i )
				m_upArr[ i ] = rArr.m_upArr[ i ];

			m_Capacity = rArr.m_Capacity;
		}

		std::unique_ptr<T[]> m_upArr;
		std::mutex m_Lock;
		sizeT m_Capacity;
	};

	// grLoopQue // FIFO // Basic thread safety
	//////////////////////////////////////////////////
	template<typename T>
	class grLoopQue
	{
	public:
		grLoopQue()
			: m_puArr		( std::make_unique<T[]>( 0 ) )
			, m_Capacity	( 0 )
			, m_Size		( 0 )
			, m_StrtIdx		( 0 )
		{}

		grLoopQue( const sizeT capacity )
			: m_puArr		( std::make_unique<T[]>( capacity ) )
			, m_Capacity	( capacity )
			, m_Size		( 0 )
			, m_StrtIdx		( 0 )
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

		grLoopQue( grLoopQue&& que ) noexcept = delete;
		grLoopQue& operator=( grLoopQue&& que ) noexcept = delete;

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
		inline void Cpy( const grLoopQue<T>& que )
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

	// grSTimer // Create an instance at the start of a block and get time when this destroy
	//////////////////////////////////////////////////
	struct grSTimer
	{
		enum class ETimeType
		{
			NS = 0,
			MS,
			S
		};

		grSTimer( const char* name = "Timer", ETimeType timeType = ETimeType::MS )
			: Start		( std::chrono::high_resolution_clock::now() )
			, Name		( name )
			, TimeType	( timeType )
		{}

		~grSTimer()
		{
			std::chrono::high_resolution_clock::duration delta = std::chrono::high_resolution_clock::now() - Start;
			switch ( TimeType )
			{
				case ETimeType::NS: std::cout << Name << " (ns): " << std::chrono::duration_cast<std::chrono::nanoseconds>( delta ).count() << std::endl; break;
				case ETimeType::MS: std::cout << Name << " (ms): " << std::chrono::duration_cast<std::chrono::milliseconds>( delta ).count() << std::endl; break;
				case ETimeType::S: std::cout << Name << " (s): " << std::chrono::duration_cast<std::chrono::seconds>( delta ).count() << std::endl; break;
			}
		}

		grSTimer( const grSTimer& ) = delete;
		grSTimer( grSTimer&& ) noexcept = delete;
		grSTimer& operator=( const grSTimer& ) = delete;
		grSTimer& operator=( grSTimer&& ) noexcept = delete;

	private:
		std::chrono::steady_clock::time_point Start;
		const char* Name;
		ETimeType TimeType;
	};


	// Experimantal stuff
	////////////////////////////////////////////////////////////////////////////////////////////////////


	// grLinearActivity
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


	// grHashNode
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
