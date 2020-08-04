#ifndef _H_GRBOID_
#define _H_GRBOID_

#include "grCommon.h"
#include "grStruct.h"

// TEST
#include "grBBox.h"
#include "grDebugManager.h"
// TEST

typedef grStruct::grArrDyn<grV2f> ArrV2f;
typedef grStruct::grArrDyn<sizeT> ArrSizeT;


struct grSBoidArr
{
	ArrV2f Alignment;
	ArrV2f Cohesion;
	ArrV2f Separation;
	ArrV2f Velocity;
	ArrV2f Position;

	ArrSizeT NeighbourFlag;
	ArrSizeT NeighbourCount;
};


struct grBoidData
{
	float AlignmentWeight = 1.0f;
	float CohesionWeight = 1.0f;
	float SeparationWeight = 1.0f;

	float BaseSpeed = 150.0f;
	float AreaDist = 100.0f;

	sizeT Alive = 0;
	sizeT Size = 0;
};


class grBoidSys
{
public:
	void Init( const sizeT size )
	{
		m_BoidData.Size = size;

		m_BoidArr.Alignment.Reset( size );
		m_BoidArr.Cohesion.Reset( size );
		m_BoidArr.Separation.Reset( size );
		m_BoidArr.Position.Reset( size );
		m_BoidArr.Velocity.Reset( size );

		m_BoidArr.NeighbourFlag.Reset( size * ( size - 1 ) );
		m_BoidArr.NeighbourCount.Reset( size );
	}

	void Add( const grV2f& rPos, const grV2f& rDir )
	{
		if( m_BoidData.Alive == m_BoidData.Size )
			return;

		m_BoidArr.Alignment.Push( { 0.0f, 0.0f } );
		m_BoidArr.Cohesion.Push( { 0.0f, 0.0f } );
		m_BoidArr.Separation.Push( { 0.0f, 0.0f } );
		m_BoidArr.Velocity.Push( { rDir } );
		m_BoidArr.Position.Push( { rPos } );

		sizeT strtIdx = m_BoidData.Alive * ( m_BoidData.Size - 1 );
		sizeT endIdx = ( m_BoidData.Size - 1 ) + strtIdx;
		for ( sizeT i = strtIdx; i < endIdx; ++i )
			m_BoidArr.NeighbourFlag.Push( 0 );

		m_BoidArr.NeighbourCount.Push( 0 );

		++m_BoidData.Alive;
	}

	void Update( const float dt )
	{
		FindlNeighbour();
		Alignment();
		Cohesion();
		Separation();
		Finalize( dt );
	}

private:
	void FindlNeighbour()
	{
		for ( sizeT i = 0; i < m_BoidData.Alive; ++i )
			m_BoidArr.NeighbourCount.Set( i, 0 );

		float areaDist{ m_BoidData.AreaDist };
		sizeT alive{ m_BoidData.Alive };
		sizeT neighbourSize{ alive - 1 };
		for ( sizeT i = 0; i < alive; ++i )
		{
			grV2f nowPos{ m_BoidArr.Position.Get( i ) };
			sizeT neighbourAcc{ 0 };
			for ( sizeT j = 0; j < alive; ++j )
			{
				if ( i == j )
					continue;

				grV2f& neighbourPos{ m_BoidArr.Position.Get( j ) };
				float dist{ neighbourPos.Between( nowPos ).Magnitude() };
				if ( dist < areaDist )
				{
					m_BoidArr.NeighbourFlag.Set( i * neighbourSize + neighbourAcc++, j );
					m_BoidArr.NeighbourCount.Set( i, neighbourAcc );
				}
			}
		}
	}

	void Alignment()
	{
		sizeT alive{ m_BoidData.Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			sizeT neighbourSize{ m_BoidArr.NeighbourCount.Get( i ) };
			sizeT neighbourAcc{ 0 };
			m_BoidArr.Alignment.Set( i, { 0.0f, 0.0f } );
			for ( sizeT j = 0; j < neighbourSize; ++j )
			{
				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag.Get( i * neighbourSize + neighbourAcc ) };
				m_BoidArr.Alignment.Set( i, m_BoidArr.Alignment.Get( i ) + m_BoidArr.Velocity.Get( neighbourIdx ) );
				++neighbourAcc;
			}

			if ( neighbourAcc == 0 || grMath::CmpV2f( m_BoidArr.Alignment.Get( i ), grV2f( 0.0f, 0.0f ) ) )
				continue;

			m_BoidArr.Alignment.Set( i, m_BoidArr.Alignment.Get( i ) / neighbourAcc );
			m_BoidArr.Alignment.Set( i, m_BoidArr.Alignment.Get( i ).Normalized() );
		}
	}

	void Cohesion()
	{
		sizeT alive{ m_BoidData.Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			sizeT neighbourSize{ m_BoidArr.NeighbourCount.Get( i ) };
			sizeT neighbourAcc{ 0 };
			m_BoidArr.Cohesion.Set( i, { 0.0f, 0.0f } );
			for ( sizeT j = 0; j < neighbourSize; ++j )
			{
				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag.Get( i * neighbourSize + neighbourAcc ) };
				m_BoidArr.Cohesion.Set( i, m_BoidArr.Cohesion.Get( i ) + m_BoidArr.Position.Get( neighbourIdx ) );
				++neighbourAcc;
			}

			if ( neighbourAcc == 0 || grMath::CmpV2f( m_BoidArr.Cohesion.Get( i ), grV2f( 0.0f, 0.0f ) ) )
				continue;

			m_BoidArr.Cohesion.Set( i, ( m_BoidArr.Cohesion.Get( i ) / neighbourAcc ) - m_BoidArr.Position.Get( i ) );
			m_BoidArr.Cohesion.Set( i, m_BoidArr.Cohesion.Get( i ).Normalized() );
		}
	}

	void Separation()
	{
		sizeT alive{ m_BoidData.Alive };
		for ( size_t i = 0; i < alive; ++i )
		{
			grV2f nowPos{ m_BoidArr.Position.Get( i ) };
			sizeT neighbourSize{ m_BoidArr.NeighbourCount.Get( i ) };
			sizeT neighbourAcc{ 0 };
			m_BoidArr.Separation.Set( i, { 0.0f, 0.0f } );
			for ( sizeT j = 0; j < neighbourSize; ++j )
			{
				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag.Get( i * neighbourSize + neighbourAcc ) };
				m_BoidArr.Separation.Set( i, m_BoidArr.Separation.Get( i ) + ( nowPos - m_BoidArr.Position.Get( neighbourIdx ) ).Magnitude() );
				++neighbourAcc;
			}

			if ( neighbourAcc == 0 || grMath::CmpV2f( m_BoidArr.Separation.Get( i ), grV2f( 0.0f, 0.0f ) ) )
				continue;

			m_BoidArr.Separation.Set( i, ( m_BoidArr.Separation.Get( i ) / neighbourAcc ) * -1.0f );
			m_BoidArr.Separation.Set( i, m_BoidArr.Separation.Get( i ).Normalized() );
		}
	}

	void Finalize( const float dt )
	{
		float fWeight{ m_BoidData.AlignmentWeight };
		float cWeight{ m_BoidData.CohesionWeight };
		float sWeight{ m_BoidData.SeparationWeight };
		float speed{ m_BoidData.BaseSpeed };

		sizeT alive{ m_BoidData.Alive };

		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ) + m_BoidArr.Alignment.Get( i ) * fWeight );

		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ) + m_BoidArr.Cohesion.Get( i ) * cWeight );

		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ) + m_BoidArr.Separation.Get( i ) * sWeight );

		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ).Normalized() );

		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Position.Set( i, m_BoidArr.Position.Get( i ) + m_BoidArr.Velocity.Get( i ) * speed * dt );


		// TEST
		const grV2u& winSize = grCore::Instance().GetWindowSize();
		for ( sizeT i = 0; i < alive; ++i )
		{
			if ( m_BoidArr.Position.Get( i ).x < 0.0f )
			{
				grV2f p{ m_BoidArr.Position.Get( i ) };
				m_BoidArr.Position.Set( i, { ( float )winSize.x, p.y } );
			}

			if ( m_BoidArr.Position.Get( i ).x > ( float )winSize.x )
			{
				grV2f p{ m_BoidArr.Position.Get( i ) };
				m_BoidArr.Position.Set( i, { 0.0f, p.y } );
			}

			if ( m_BoidArr.Position.Get( i ).y < 0.0f )
			{
				grV2f p{ m_BoidArr.Position.Get( i ) };
				m_BoidArr.Position.Set( i, { p.x, ( float )winSize.y } );
			}

			if ( m_BoidArr.Position.Get( i ).y > ( float )winSize.y )
			{
				grV2f p{ m_BoidArr.Position.Get( i ) };
				m_BoidArr.Position.Set( i, { p.x, 0.0f } );
			}
		}

		for ( sizeT i = 0; i < alive; ++i )
		{
			//std::cout << m_BoidArr.Position.Get( i ) << std::endl;

			grBBox box( { 10.0f, 10.0f }, { m_BoidArr.Position.Get( i ) } );
			grDebugManager::Instance().AddBBox( box, sf::Color::Yellow );			
		}
		// TEST
	}

	grSBoidArr m_BoidArr;
	grBoidData m_BoidData;
};

#endif // _H_GRBOID_