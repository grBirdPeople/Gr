#ifndef _H_GRBOID_
#define _H_GRBOID_

#include "grCommon.h"
#include "grStruct.h"
#include "grRandom.h"

typedef grStruct::grArrayDyn<grV2f> ArrV2f;
typedef grStruct::grArrayDyn<sizeT> ArrSizeT;


struct grSBoidArr
{
	ArrV2f Alignment;
	ArrV2f Cohesion;
	ArrV2f Separation;
	ArrV2f Position;
	ArrV2f Velocity;

	ArrSizeT NeighbourFlag;
	ArrSizeT NeighbourCount;
};


struct grBoidData
{
	float AlignmentWeight = 4.0f;
	float CohesionWeight = 4.0f;
	float SeparationWeight = 4.0f;

	float BaseSpeed = 50.0f;
	float AreaDist = 100.0f;
};


class grBoidSys
{
public:
	void Init( const sizeT size )
	{
		m_Size = size;

		m_BoidArr.Alignment.Reset( m_Size );
		m_BoidArr.Cohesion.Reset( m_Size );
		m_BoidArr.Separation.Reset( m_Size );
		m_BoidArr.Position.Reset( m_Size );
		m_BoidArr.Velocity.Reset( m_Size );

		m_BoidArr.NeighbourFlag.Reset( m_Size * ( m_Size - 1 ) );
		m_BoidArr.NeighbourCount.Reset( m_Size );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Alignment.Push( { 0.0f, 0.0f } );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Cohesion.Push( { 0.0f, 0.0f } );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Separation.Push( { 0.0f, 0.0f } );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Position.Push( { 0.0f, 0.0f } );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Velocity.Push( { 0.0f, 0.0f } );

		for ( sizeT i = 0; i < m_Size * ( m_Size - 1 ); ++i )
			m_BoidArr.NeighbourFlag.Push( 0 );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.NeighbourCount.Push( 0 );


		// TEMP
		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Position.Set( i, { m_Rand.Float( 0.0f, 960.0f ), m_Rand.Float( 0.0f, 540.0f ) } );
		// TEMP
	}

	void Update()
	{
		FindlNeighbour();
		Alignment();
		Cohension();
		Separation();
		Finalize();
	}

private:
	void FindlNeighbour()
	{
		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.NeighbourCount.Set( i, 0 );

		float areaDist { m_BoidData.AreaDist };
		sizeT neighbourSize { m_Size - 1 };
		for ( sizeT i = 0; i < m_Size; ++i )
		{
			grV2f& currentPos { m_BoidArr.Position.Get( i ) };
			sizeT neighbourAcc { 0 };
			for ( sizeT j = 0; j < m_Size; ++j )
			{
				if ( i == j )
					continue;

				grV2f& neighbourPos { m_BoidArr.Position.Get( j ) };
				float dist { neighbourPos.Between( currentPos ).Magnitude() };
				if ( dist < areaDist )
				{
					m_BoidArr.NeighbourFlag.Set( i * neighbourSize + neighbourAcc++, j );
					m_BoidArr.NeighbourCount.Set( i, m_BoidArr.NeighbourCount.Get( i ) + 1 );
				}
			}
		}
	}

	void Alignment()
	{
		for ( sizeT i = 0; i < m_Size; ++i )
		{
			sizeT neighbourSize { m_BoidArr.NeighbourCount.Get( i ) };
			sizeT neighbourAcc { 0 };
			for ( sizeT j = 0; j < neighbourSize; ++j )
			{
				sizeT neighbourIdx { m_BoidArr.NeighbourFlag.Get( i * neighbourSize + neighbourAcc ) };
				m_BoidArr.Alignment.Set( i, m_BoidArr.Alignment.Get( i ) + m_BoidArr.Velocity.Get( neighbourIdx ) );
				++neighbourAcc;
			}

			m_BoidArr.Alignment.Set( i, m_BoidArr.Alignment.Get( i ) / neighbourAcc );
			m_BoidArr.Alignment.Set( i, m_BoidArr.Alignment.Get( i ).Normalized() );
		}
	}

	void Cohension()
	{
		for ( sizeT i = 0; i < m_Size; ++i )
		{
			sizeT neighbourSize { m_BoidArr.NeighbourCount.Get( i ) };
			sizeT neighbourAcc { 0 };
			for ( sizeT j = 0; j < neighbourSize; ++j )
			{
				sizeT neighbourIdx { m_BoidArr.NeighbourFlag.Get( i * neighbourSize + neighbourAcc ) };
				m_BoidArr.Cohesion.Set( i, m_BoidArr.Cohesion.Get( i ) + m_BoidArr.Position.Get( neighbourIdx ) );
				++neighbourAcc;
			}

			m_BoidArr.Cohesion.Set( i, ( m_BoidArr.Cohesion.Get( i ) / neighbourAcc ) - m_BoidArr.Position.Get( i ) );
			m_BoidArr.Cohesion.Set( i, m_BoidArr.Cohesion.Get( i ).Normalized() );
		}
	}

	void Separation()
	{
		for ( size_t i = 0; i < m_Size; ++i )
		{
			grV2f nowPos { m_BoidArr.Position.Get( i ) };
			sizeT neighbourSize { m_BoidArr.NeighbourCount.Get( i ) };
			sizeT neighbourAcc { 0 };
			for ( sizeT j = 0; j < neighbourSize; ++j )
			{
				sizeT neighbourIdx { m_BoidArr.NeighbourFlag.Get( i * neighbourSize + neighbourAcc ) };
				m_BoidArr.Separation.Set( i, m_BoidArr.Separation.Get( i ) + ( m_BoidArr.Position.Get( neighbourIdx ) - nowPos ) );
				++neighbourAcc;
			}

			m_BoidArr.Separation.Set( i, ( m_BoidArr.Separation.Get( i ) / neighbourAcc ) * -1.0f );
			m_BoidArr.Separation.Set( i, m_BoidArr.Separation.Get( i ).Normalized() );
		}
	}

	void Finalize()
	{
		float fWeight = m_BoidData.AlignmentWeight;
		float cWeight = m_BoidData.CohesionWeight;
		float sWeight = m_BoidData.SeparationWeight;
		float speed = m_BoidData.BaseSpeed;

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ) + m_BoidArr.Alignment.Get( i ) * fWeight );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ) + m_BoidArr.Cohesion.Get( i ) * cWeight );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ) + m_BoidArr.Separation.Get( i ) * sWeight );

		for ( sizeT i = 0; i < m_Size; ++i )
			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ).Normalized() * speed );
	}

	grSBoidArr m_BoidArr;
	grBoidData m_BoidData;
	sizeT m_Size;

	grRandom m_Rand;
};

#endif // _H_GRBOID_