#ifndef _H_GRBOIDSYS_
#define _H_GRBOIDSYS_

// Example used found here: http://www.kfish.org/boids/pseudocode.html

#include "grCommon.h"
#include "grStruct.h"

typedef grStruct::grArrDyn<grV2f> ArrDynV2f;
typedef grStruct::grArrDyn<sizeT> ArrDynV2u;


struct grSBoidArr
{
	ArrDynV2f Cohesion;
	ArrDynV2f Separation;
	ArrDynV2f Alignment;

	ArrDynV2f Acceleration;
	ArrDynV2f Velocity;
	ArrDynV2f Position;
};


struct grSBoidVar
{
	float ViewDistCoh;
	float ViewDistSep;
	float ViewDistAli;
	float ViewFov;	// Dot -1.0f <-> 1.0f

	float WeightCoh;
	float WeightSep;
	float WeightAli;

	float LimitForce;

	float Radius;

	sizeT Alive;
	sizeT Capacity;
};


class grCBoidSys
{
public:

	grCBoidSys()
		: m_BoidVar( std::make_unique<grSBoidVar>() )
	{}

	inline void Init( const sizeT capacity )
	{
		// Arr
		m_BoidArr.Cohesion.Reset( capacity );
		m_BoidArr.Separation.Reset( capacity );
		m_BoidArr.Alignment.Reset( capacity );
		m_BoidArr.Acceleration.Reset( capacity );
		m_BoidArr.Velocity.Reset( capacity );
		m_BoidArr.Position.Reset( capacity );

		// Var
		m_BoidVar->ViewDistCoh = 100.0f;
		m_BoidVar->ViewDistSep = 50.0f;
		m_BoidVar->ViewDistAli = 50.0f;
		m_BoidVar->ViewFov = -0.5f;

		m_BoidVar->WeightCoh = 1.0f;
		m_BoidVar->WeightSep = 0.1f;
		m_BoidVar->WeightAli = 1.0f;

		m_BoidVar->LimitForce = 100.0f;

		m_BoidVar->Radius = 0.0f;

		m_BoidVar->Alive = 0;
		m_BoidVar->Capacity = capacity;
	}

	inline bool Spawn( const grV2f& pos, const grV2f& dir, const float rad )
	{
		grSBoidVar& var = *m_BoidVar;

		if( var.Alive == var.Capacity )
			return false;

		grV2f d{ dir };
		m_BoidArr.Velocity.Write( var.Alive, d * 100.0f );
		m_BoidArr.Position.Write( var.Alive, pos );

		var.Radius = rad;
		++var.Alive;

		return true;
	}

	inline void Update( const float dt )
	{
		Cohesion();
		Separation();
		Alignment();
		Result( dt );
		Reset();

		WinEdge();
	}

	inline void Render( sf::RenderWindow& rRenderWin )
	{
		grSBoidVar& vars = *m_BoidVar;
		float boxSize{ vars.Radius * 2.0f };
		sizeT alive{ vars.Alive };

		for ( sizeT i = 0; i < alive; ++i )
		{
			//std::cout << m_BoidArr.Position[ i ] << std::endl;
	
			sf::Color c( sf::Color::Yellow );
			//if( i == 0 )
			//	c = sf::Color::Green;

			grBBox box( { boxSize, boxSize }, { m_BoidArr.Position.Read( i ) } );
			grDebugManager::Instance().AddBBox( box, c );
		}
	}

private:
	inline void Cohesion()
	{
		grV2f v;
		float radMul{ m_BoidVar->Radius * 3.0f };
		float viewFOV{ m_BoidVar->ViewFov };
		float viewDist{ m_BoidVar->ViewDistCoh };
		sizeT alive{ m_BoidVar->Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			grV2f velNow{ m_BoidArr.Velocity.Read( i ) };
			grV2f posNow{ m_BoidArr.Position.Read( i ) };
			sizeT acc = 0;
			for ( sizeT j = 0; j < alive; ++j )
			{
				if( i == j )
					continue;
				
				grV2f posOther{ m_BoidArr.Position.Read( j ) };
				grV2f between{ posNow.Between( posOther ) };
				float len{ between.Magnitude() };
				float dot{ velNow.Normalized().Dot( between / len ) };
				if ( dot > viewFOV && len > 0.0f && len < viewDist )
				{
					v += posOther;
					++acc;
				}
			}

			if ( acc > 0 )
			{
				v /= acc;
				v = posNow.Between( v );
				v.Normalize();
				m_BoidArr.Cohesion.Write( i, v  );
			}
		}
	}

	inline void Separation()
	{
		grV2f v;
		float radMul{ m_BoidVar->Radius * 3.0f };
		float viewFOV{ m_BoidVar->ViewFov };
		float viewDist{ m_BoidVar->ViewDistSep };
		sizeT alive{ m_BoidVar->Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			grV2f velNow{ m_BoidArr.Velocity.Read( i ) };
			grV2f posNow{ m_BoidArr.Position.Read( i ) };
			sizeT acc = 0;
			for ( sizeT j = 0; j < alive; ++j )
			{
				if ( i == j )
					continue;

				grV2f posOther{ m_BoidArr.Position.Read( j ) };
				grV2f between{ posNow.Between( posOther ) };
				float len{ between.Magnitude() };
				float dot{ velNow.Normalized().Dot( between / len ) };
				//if ( dot > viewFOV && len > radMul && len < viewDist )
				if ( dot > viewFOV && len > 0.0f && len < viewDist )
				{
					v -= between;
					++acc;
				}
			}

			if ( acc > 0 )
			{
				//v.Normalize();
				m_BoidArr.Separation.Write( i, v );
			}
		}
	}

	inline void Alignment()
	{
		grV2f v;
		float radMul{ m_BoidVar->Radius * 3.0f };
		float viewFOV{ m_BoidVar->ViewFov };
		float viewDist{ m_BoidVar->ViewDistAli };
		sizeT alive{ m_BoidVar->Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			grV2f velNow{ m_BoidArr.Velocity.Read( i ) };
			grV2f posNow{ m_BoidArr.Position.Read( i ) };
			sizeT acc = 0;
			for ( sizeT j = 0; j < alive; ++j )
			{
				if( i == j )
					continue;

				grV2f posOther{ m_BoidArr.Position.Read( j ) };
				grV2f between{ posNow.Between( posOther ) };
				float len{ between.Magnitude() };
				float dot{ velNow.Normalized().Dot( between / len ) };
				if ( dot > viewFOV && len > 0.0f && len < viewDist )
				{
					v += m_BoidArr.Velocity.Read( j );
					++acc;
				}
			}

			if ( acc > 0 )
			{
				v /= acc;
				v = velNow.Between( v );
				v.Normalize();
				m_BoidArr.Alignment.Write( i, v );
			}
		}
	}

	inline void Result( const float dt )
	{
		float LimitForce{ m_BoidVar->LimitForce };
		float weightAli{ m_BoidVar->WeightAli };
		float weightSep{ m_BoidVar->WeightSep };
		float weightCoh{ m_BoidVar->WeightCoh };
		sizeT alive{ m_BoidVar->Alive };

		// Cohesion
		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Acceleration.Write( i, m_BoidArr.Acceleration.Read( i ) + m_BoidArr.Cohesion.Read( i ) * weightCoh  );

		// Separation
		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Acceleration.Write( i, m_BoidArr.Acceleration.Read( i ) + m_BoidArr.Separation.Read( i ) * weightSep );

		// Alignment
		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Acceleration.Write( i, m_BoidArr.Acceleration.Read( i ) + m_BoidArr.Alignment.Read( i ) * weightAli );

		// Velocity
		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Velocity.Write( i, ( m_BoidArr.Velocity.Read( i ) + m_BoidArr.Acceleration.Read( i ) ).LimitMax( LimitForce ) );

		// Position
		for ( sizeT i = 0; i < alive; ++i )
			m_BoidArr.Position.Write( i, m_BoidArr.Position.Read( i ) + m_BoidArr.Velocity.Read( i ) * dt );
	}

	inline void Reset()
	{
		grSBoidVar& var = *m_BoidVar;

		// Cohesion
		for ( sizeT i = 0; i < var.Alive; ++i )
			m_BoidArr.Cohesion.Write( i, { 0.0f, 0.0f } );

		// Separation
		for ( sizeT i = 0; i < var.Alive; ++i )
			m_BoidArr.Separation.Write( i, { 0.0f, 0.0f } );

		// Alignment
		for ( sizeT i = 0; i < var.Alive; ++i )
			m_BoidArr.Alignment.Write( i, { 0.0f, 0.0f } );

		// Acceleration
		for ( sizeT i = 0; i < var.Alive; ++i )
			m_BoidArr.Acceleration.Write( i, { 0.0f, 0.0f } );
	}

	inline void WinEdge()
	{
		grV2f winSize{ ( float )grCore::Instance().GetWindowSize().x, ( float )grCore::Instance().GetWindowSize().y };
		float radius{ m_BoidVar->Radius };
		sizeT alive{ m_BoidVar->Alive };

		// Teleport
		for ( sizeT i = 0; i < alive; ++i )
		{
			if ( m_BoidArr.Position.Read( i ).x < -radius )
				m_BoidArr.Position.Write( i, { winSize.x + radius, m_BoidArr.Position.Read( i ).y } );

			if ( m_BoidArr.Position.Read( i ).x > winSize.x + radius )
				m_BoidArr.Position.Write( i, { -radius, m_BoidArr.Position.Read( i ).y } );

			if ( m_BoidArr.Position.Read( i ).y < -radius )
				m_BoidArr.Position.Write( i, { m_BoidArr.Position.Read( i ).x ,winSize.y + radius } );

			if ( m_BoidArr.Position.Read( i ).y > winSize.y + radius )
				m_BoidArr.Position.Write( i, { m_BoidArr.Position.Read( i ).x, -radius } );
		}


		//// Bounce
		//for ( sizeT i = 0; i < alive; ++i )
		//{
		//	if ( m_BoidArr.Position.Read( i ).x < radius )
		//	{
		//		m_BoidArr.Position.Read( i ).x = radius;
		//		m_BoidArr.Velocity.Read( i ).x *= -1.0f;
		//	}

		//	if ( m_BoidArr.Position.Read( i ).x > winSize.x - radius )
		//	{
		//		m_BoidArr.Position.Read( i ).x = ( winSize.x - radius );
		//		m_BoidArr.Velocity.Read( i ).x *= -1.0f;
		//	}

		//	if ( m_BoidArr.Position.Read( i ).y < radius )
		//	{
		//		m_BoidArr.Position.Read( i ).y = radius;
		//		m_BoidArr.Velocity.Read( i ).y *= -1.0f;
		//	}

		//	if ( m_BoidArr.Position.Read( i ).y > winSize.y - radius )
		//	{
		//		m_BoidArr.Position.Read( i ).y = ( winSize.y - radius );
		//		m_BoidArr.Velocity.Read( i ).y *= -1.0f;
		//	}
		//}
	}

	grSBoidArr m_BoidArr;
	pU<grSBoidVar> m_BoidVar;
};


#endif // _H_GRBOIDSYS_



























//#ifndef _H_GRBOIDSYS_
//#define _H_GRBOIDSYS_
//
//#include "grCommon.h"
//#include "grStruct.h"
//
//// TEST
//#include <math.h>
//#include "grDebugManager.h"
//#include "grBBox.h"
//#include "grCore.h"
//// TEST
//
//
//// JA's C++ DOD version of an JavaScript? OOP implementaion found: https://processing.org/examples/flocking.html
//
//typedef grStruct::grArrDyn<grV2f> ArrDynV2f;
//typedef grStruct::grArrDyn<sizeT> ArrDynSizeT;
//
//
//struct grSBoidArr
//{
//	pU<grV2f[]> Separation;
//	pU<grV2f[]> Alignment;
//	pU<grV2f[]> Cohesion;
//
//	pU<grV2f[]> Acceleration;
//	pU<grV2f[]> Velocity;
//	pU<grV2f[]> Position;
//
//	pU<sizeT[]> NeighbourFlag;
//	pU<sizeT[]> NeighbourCount;
//};
//
//
//struct grSBoidVar
//{
//	float Radius;
//	float MaxForce;
//	float Speed;
//	float MaxSepDist;
//	float MaxAliCohDist;
//
//	float SepWeight;
//	float AliWeight;
//	float CohWeight;
//
//	sizeT Alive;
//	sizeT Capacity;
//};
//
//
//class grCBoidSys
//{
//public:
//	void Init( const sizeT capacity )
//	{
//		// Array struct
//		m_BoidArr.Separation = std::make_unique<grV2f[]>( capacity );
//		m_BoidArr.Alignment = std::make_unique<grV2f[]>( capacity );
//		m_BoidArr.Cohesion = std::make_unique<grV2f[]>( capacity );
//
//		m_BoidArr.Acceleration = std::make_unique<grV2f[]>( capacity );
//		m_BoidArr.Velocity = std::make_unique<grV2f[]>( capacity );
//		m_BoidArr.Position = std::make_unique<grV2f[]>( capacity );
//
//		m_BoidArr.NeighbourFlag = std::make_unique<sizeT[]>( capacity * ( capacity - 1 ) );
//		m_BoidArr.NeighbourCount = std::make_unique<sizeT[]>( capacity );
//
//		// Data struct
//		m_BoidVar.Radius = { 2.0f };
//		m_BoidVar.MaxForce = { 2.0f };
//		m_BoidVar.Speed = { 100.0f };
//		m_BoidVar.MaxSepDist = { 20.0f };
//		m_BoidVar.MaxAliCohDist = { 100.0f };
//
//		m_BoidVar.SepWeight = { 2.0f };
//		m_BoidVar.AliWeight = { 1.25f };
//		m_BoidVar.CohWeight = { 1.0f };
//
//		m_BoidVar.Alive = { 0 };
//		m_BoidVar.Capacity = { capacity };
//	}
//
//	void SpawnBoid( const grV2f& rPos, const grV2f& rDir )
//	{
//		if ( m_BoidVar.Alive == m_BoidVar.Capacity )
//			return;
//
//		m_BoidArr.Velocity[ m_BoidVar.Alive ] = grV2f( rDir ) * m_BoidVar.Speed;
//		m_BoidArr.Position[ m_BoidVar.Alive ] = { rPos };
//
//		++m_BoidVar.Alive;
//	}
//
//	void Update( const float dt )
//	{
//		// FlagNeighbour func Twice or double up with arrays
//		// Should go for 2x arrays as calling the function twice counters the whole point if DOD
//
//		FlagNeighbour( m_BoidVar.MaxSepDist );
//		Separation();
//		FlagNeighbour( m_BoidVar.MaxAliCohDist );
//		Alignment();
//		Cohesion();
//		Finalize( dt );
//		Reset();
//
//		// TEST
//		WinEdge();
//		// TEST
//	}
//
//	void Render( sf::RenderWindow& rRenderWin )
//	{
//		float boxSize{ m_BoidVar.Radius * 2.0f };
//		sizeT alive{ m_BoidVar.Alive };
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			//std::cout << m_BoidArr.Position[ i ] << std::endl;
//
//			grBBox box( { boxSize, boxSize }, { m_BoidArr.Position[ i ] } );
//			grDebugManager::Instance().AddBBox( box, sf::Color::Yellow );
//		}
//	}
//
//private:
//	void FlagNeighbour( const float maxDist )
//	{
//		// Cache some things
//		sizeT alive{ m_BoidVar.Alive };
//		sizeT neighbourMax{ m_BoidVar.Capacity - 1 };	// changed
//
//		// Reset neighbour count
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.NeighbourCount[ i ] = 0;
//
//		// Iterate all boids and for each flag which other boids
//		// will be relevant for separation, alignment and cohesion calc
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f nowPos{ m_BoidArr.Position[ i ] };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < alive; ++j )
//			{
//				if ( i == j )
//					continue;
//
//				grV2f neighbourPos{ m_BoidArr.Position[ j ] };
//				float dist{ neighbourPos.Between( nowPos ).Magnitude() };
//				if ( dist > 0 && dist < maxDist )
//				{
//					m_BoidArr.NeighbourFlag[ i * neighbourMax + neighbourAcc ] = j;
//					m_BoidArr.NeighbourCount[ i ] = neighbourAcc;
//					++neighbourAcc;
//				}
//			}
//		}
//	}
//
//	void Separation()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//		for ( size_t i = 0; i < alive; ++i )
//		{
//			grV2f v;
//			grV2f nowPos{ m_BoidArr.Position[ i ] };
//			sizeT neighbourCount{ m_BoidArr.NeighbourCount[ i ] };
//			sizeT neighbourMax{ m_BoidVar.Capacity - 1 };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < neighbourCount; ++j )
//			{
//				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag[ i * neighbourMax + j ] };
//				grV2f between{ m_BoidArr.Position[ neighbourIdx ].Between( nowPos ) };
//				between.Normalize();
//				between /= between.Magnitude();
//				v += between;
//				++neighbourAcc;
//			}
//
//			if ( neighbourAcc > 0 )
//				v /= ( float )neighbourAcc;
//
//			if ( v.Magnitude() > 0.0f ) // MagnitudeSqr should work
//			{
//				float maxForce{ m_BoidVar.MaxForce };
//				grV2f vel{ m_BoidArr.Velocity[ i ] };
//				float Speed{ m_BoidVar.Speed };
//				v.Normalize();
//				v *= Speed;
//				v -= vel;
//				v.LimitMax( maxForce );
//			}
//
//			m_BoidArr.Separation[ i ] = v;
//		}
//	}
//
//	void Alignment()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v;
//			sizeT neighbourCount{ m_BoidArr.NeighbourCount[ i ] };
//			sizeT neighbourMax{ m_BoidVar.Capacity - 1 };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < neighbourCount; ++j )
//			{
//				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag[ i * neighbourMax + j ] };
//				grV2f other{ m_BoidArr.Velocity[ neighbourIdx ] };
//				v.Set( v + other );
//				++neighbourAcc;
//			}
//
//			if ( neighbourAcc > 0 )
//			{
//				float maxForce{ m_BoidVar.MaxForce };
//				grV2f velocity{ m_BoidArr.Velocity[ i ] };
//				float maxSpeed{ m_BoidVar.Speed };
//
//				v /= ( float )neighbourAcc;
//				v.Normalize();
//				v *= maxSpeed;
//				v = v / velocity;
//				v.LimitMax( maxForce );
//
//				m_BoidArr.Alignment[ i ] = v;
//			}
//		}
//	}
//
//	void Cohesion()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v;
//			sizeT neighbourCount{ m_BoidArr.NeighbourCount[ i ] };
//			sizeT neighbourMax{ m_BoidVar.Capacity - 1 };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < neighbourCount; ++j )
//			{
//				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag[ i * neighbourMax + j ] };
//				v += m_BoidArr.Position[ neighbourIdx ];
//				++neighbourAcc;
//			}
//
//			if ( neighbourAcc > 0 )
//			{
//				float maxForce{ m_BoidVar.MaxForce };
//				grV2f vel{ m_BoidArr.Velocity[ i ] };
//				float maxSpeed{ m_BoidVar.Speed };
//				grV2f pos{ m_BoidArr.Position[ i ] };
//
//				v /= ( float )neighbourAcc;
//				v = pos.Between( v );
//				v.Normalize();
//				v *= maxSpeed;
//				v = vel.Between( v );
//				v.LimitMax( maxForce );
//
//				m_BoidArr.Cohesion[ i ] = v;
//			}
//		}
//	}
//
//	void Finalize( const float dt )
//	{
//		float sepWeight{ m_BoidVar.SepWeight };
//		float aliWeight{ m_BoidVar.AliWeight };
//		float cohWeight{ m_BoidVar.CohWeight };
//		float maxForce{ m_BoidVar.MaxForce };
//
//		sizeT alive{ m_BoidVar.Alive };
//
//		// Build acceleration
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Acceleration[ i ] += m_BoidArr.Separation[ i ] * sepWeight;
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Acceleration[ i ] += m_BoidArr.Alignment[ i ] * aliWeight;
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Acceleration[ i ] += m_BoidArr.Cohesion[ i ] * cohWeight;
//
//		// Create velocity
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			m_BoidArr.Acceleration[ i ].LimitMax( maxForce );
//			m_BoidArr.Velocity[ i ] += m_BoidArr.Acceleration[ i ];
//		}
//
//		// Update position
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Position[ i ] += m_BoidArr.Velocity[ i ] * dt;
//	}
//
//	void Reset()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Separation[ i ] = { 0.0f, 0.0f };
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Alignment[ i ] = { 0.0f, 0.0f };
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Cohesion[ i ] = { 0.0f, 0.0f };
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Acceleration[ i ] = { 0.0f, 0.0f };
//	}
//
//	void WinEdge()
//	{
//		grV2f winSize{ ( float )grCore::Instance().GetWindowSize().x, ( float )grCore::Instance().GetWindowSize().y };
//		float radius{ m_BoidVar.Radius };
//		sizeT alive{ m_BoidVar.Alive };
//
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			if ( m_BoidArr.Position[ i ].x < radius )
//			{
//				m_BoidArr.Position[ i ].x = radius + 1.0f;
//				m_BoidArr.Velocity[ i ].x *= -1.0f;
//			}
//
//			if ( m_BoidArr.Position[ i ].x > winSize.x - radius )
//			{
//				m_BoidArr.Position[ i ].x = ( winSize.x - radius ) - 2.0f;
//				m_BoidArr.Velocity[ i ].x *= -1.0f;
//			}
//
//			if ( m_BoidArr.Position[ i ].y < radius )
//			{
//				m_BoidArr.Position[ i ].y = radius + 1.0f;
//				m_BoidArr.Velocity[ i ].y *= -1.0f;
//			}
//
//			if ( m_BoidArr.Position[ i ].y > winSize.y - radius )
//			{
//				m_BoidArr.Position[ i ].y = ( winSize.y - radius ) - 2.0f;
//				m_BoidArr.Velocity[ i ].y *= -1.0f;
//			}
//		}
//
//
//		//for ( sizeT i = 0; i < alive; ++i )
//		//{
//		//	if ( m_BoidArr.Position[ i ].x < -radius )
//		//		m_BoidArr.Position[ i ].x = winSize.x + radius;
//
//		//	if ( m_BoidArr.Position[ i ].x > winSize.x + radius )
//		//		m_BoidArr.Position[ i ].x = -radius;
//
//		//	if ( m_BoidArr.Position[ i ].y < -radius )
//		//		m_BoidArr.Position[ i ].y = winSize.y + radius;
//
//		//	if ( m_BoidArr.Position[ i ].y > winSize.y + radius )
//		//		m_BoidArr.Position[ i ].y = -radius;
//		//}
//	}
//
//	grSBoidArr m_BoidArr;
//	grSBoidVar m_BoidVar;
//};
//
//#endif // _H_GRBOIDSYS_



























// Uses thread safe array

//#ifndef _H_GRBOIDSYS_
//#define _H_GRBOIDSYS_
//
//#include "grCommon.h"
//#include "grStruct.h"
//
//// TEST
//#include <math.h>
//#include "grDebugManager.h"
//#include "grBBox.h"
//#include "grCore.h"
//// TEST
//
//
//// JA's C++ DOD version of an JavaScript? OOP implementaion found: https://processing.org/examples/flocking.html
//
//typedef grStruct::grArrDyn<grV2f> ArrDynV2f;
//typedef grStruct::grArrDyn<sizeT> ArrDynSizeT;
//
//
//struct grSBoidArr
//{
//	ArrDynV2f Separation;
//	ArrDynV2f Alignment;
//	ArrDynV2f Cohesion;
//
//	ArrDynV2f Acceleration;
//	ArrDynV2f Velocity;
//	ArrDynV2f Position;
//
//	ArrDynSizeT NeighbourFlag;
//	ArrDynSizeT NeighbourCount;
//};
//
//
//struct grSBoidVar
//{
//	float Radius;
//	float MaxForce;
//	float MaxSpeed;
//	float MaxSepDist;
//	float MaxAliCohDist;
//
//	float SepWeight;
//	float AliWeight;
//	float CohWeight;
//
//	sizeT Alive;
//	sizeT Capacity;
//};
//
//
//class grCBoidSys
//{
//public:
//	void Init( const sizeT capacity )
//	{
//		// Array struct
//		m_BoidArr.Separation.Reset( capacity );
//		m_BoidArr.Alignment.Reset( capacity );
//		m_BoidArr.Cohesion.Reset( capacity );
//
//		m_BoidArr.Acceleration.Reset( capacity );
//		m_BoidArr.Velocity.Reset( capacity );
//		m_BoidArr.Position.Reset( capacity );
//
//		m_BoidArr.NeighbourFlag.Reset( capacity * ( capacity - 1 ) );
//		m_BoidArr.NeighbourCount.Reset( capacity );
//
//		// Data struct
//		m_BoidVar.Radius = { 5.0f };
//		m_BoidVar.MaxForce = { 0.03f };
//		m_BoidVar.MaxSpeed = { 2.0f };
//		m_BoidVar.MaxSepDist = { 20.0f };
//		m_BoidVar.MaxAliCohDist = { 50.0f };
//
//		m_BoidVar.SepWeight = { 1.0f };
//		m_BoidVar.AliWeight = { 1.0f };
//		m_BoidVar.CohWeight = { 1.0f };
//
//		m_BoidVar.Alive = { 0 };
//		m_BoidVar.Capacity = { capacity };
//	}
//
//	void SpawnBoid( const grV2f& rPos, const grV2f& rDir )
//	{
//		if ( m_BoidVar.Alive == m_BoidVar.Capacity )
//			return;
//
//		m_BoidArr.Separation.Push( { 0.0f, 0.0f } );
//		m_BoidArr.Alignment.Push( { 0.0f, 0.0f } );
//		m_BoidArr.Cohesion.Push( { 0.0f, 0.0f } );
//
//		m_BoidArr.Acceleration.Push( { 0.0f, 0.0f } );
//		m_BoidArr.Velocity.Push( { rDir } );
//		m_BoidArr.Position.Push( { rPos } );
//
//		sizeT strtIdx = m_BoidVar.Alive * ( m_BoidVar.Capacity - 1 );
//		sizeT endIdx = ( m_BoidVar.Capacity - 1 ) + strtIdx;
//		for ( sizeT i = strtIdx; i < endIdx; ++i )
//			m_BoidArr.NeighbourFlag.Push( 0 );
//
//		m_BoidArr.NeighbourCount.Push( 0 );
//
//		++m_BoidVar.Alive;
//	}
//
//	void Update( const float dt )
//	{
//		// FlagNeighbour func Twice or double up with arrays
//		// Should go for 2x arrays as calling the function twice counters the whole point if DOD
//
//		FlagNeighbour( m_BoidVar.MaxSepDist );
//		Separation();
//		FlagNeighbour( m_BoidVar.MaxAliCohDist );
//		Alignment();
//		Cohesion();
//		Finalize( dt );
//		Reset();
//
//		// TEST
//		WinEdge();
//		// TEST
//	}
//
//	void Render( sf::RenderWindow& rRenderWin )
//	{
//		sizeT alive{ m_BoidVar.Alive };
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			std::cout << m_BoidArr.Position.Get( i ) << std::endl;
//
//			grBBox box( { 10.0f, 10.0f }, { m_BoidArr.Position.Get( i ) } );
//			grDebugManager::Instance().AddBBox( box, sf::Color::Yellow );
//		}
//	}
//
//private:
//	void FlagNeighbour( const float maxDist )
//	{
//		// Cache some things
//		sizeT alive{ m_BoidVar.Alive };
//		sizeT neighbourMax{ m_BoidVar.Capacity - 1 };
//
//		// Reset neighbour count
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.NeighbourCount.Set( i, 0 );
//
//		// Iterate all boids and for each flag which other boids
//		// will be relevant for separation, alignment and cohesion calc
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f nowPos{ m_BoidArr.Position.Get( i ) };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < alive; ++j )
//			{
//				if ( i == j )
//					continue;
//
//				grV2f neighbourPos{ m_BoidArr.Position.Get( j ) };
//				float dist{ neighbourPos.Between( nowPos ).Magnitude() };
//				if ( dist > 0 && dist < maxDist )
//				{
//					m_BoidArr.NeighbourFlag.Set( i * neighbourMax + neighbourAcc, j );
//					m_BoidArr.NeighbourCount.Set( i, neighbourAcc );
//					++neighbourAcc;
//				}
//			}
//		}
//	}
//
//	void Separation()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//		for ( size_t i = 0; i < alive; ++i )
//		{
//			grV2f v;
//			grV2f nowPos{ m_BoidArr.Position.Get( i ) };
//			sizeT neighbourCount{ m_BoidArr.NeighbourCount.Get( i ) };
//			sizeT neighbourMax{ m_BoidVar.Capacity - 1 };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < neighbourCount; ++j )
//			{
//
//				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag.Get( i * neighbourMax + j ) };
//
//				if ( isnan( m_BoidArr.Position.Get( neighbourIdx ).x ) )
//					int k = 0;
//
//				grV2f between{ m_BoidArr.Position.Get( neighbourIdx ).Between( nowPos ) };
//				between.Normalize();
//				between /= between.Magnitude();
//				v += between;
//				++neighbourAcc;
//			}
//
//			if ( neighbourAcc > 0 )
//				v /= ( float )neighbourAcc;
//
//			if ( v.Magnitude() > 0.0f ) // MagnitudeSqr should work
//			{
//				float maxForce{ m_BoidVar.MaxForce };
//				grV2f vel{ m_BoidArr.Velocity.Get( i ) };
//				float maxSpeed{ m_BoidVar.MaxSpeed };
//				v.Normalize();
//				v *= maxSpeed;
//				v -= vel;
//				v.LimitMax( maxForce );
//			}
//
//			m_BoidArr.Separation.Set( i, v );
//		}
//	}
//
//	void Alignment()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v;
//			sizeT neighbourCount{ m_BoidArr.NeighbourCount.Get( i ) };
//			sizeT neighbourMax{ m_BoidVar.Capacity - 1 };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < neighbourCount; ++j )
//			{
//				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag.Get( i * neighbourMax + j ) };
//				grV2f other{ m_BoidArr.Velocity.Get( neighbourIdx ) };
//				v.Set( v + other );
//				++neighbourAcc;
//			}
//
//			if ( neighbourAcc > 0 )
//			{
//				float maxForce{ m_BoidVar.MaxForce };
//				grV2f velocity{ m_BoidArr.Velocity.Get( i ) };
//				float maxSpeed{ m_BoidVar.MaxSpeed };
//
//				v /= ( float )neighbourAcc;
//				v.Normalize();
//				v *= maxSpeed;
//				v = v / velocity;
//				v.LimitMax( maxForce );
//
//				m_BoidArr.Alignment.Set( i, v );
//			}
//		}
//	}
//
//	void Cohesion()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v;
//			sizeT neighbourCount{ m_BoidArr.NeighbourCount.Get( i ) };
//			sizeT neighbourMax{ m_BoidVar.Capacity - 1 };
//			sizeT neighbourAcc{ 0 };
//			for ( sizeT j = 0; j < neighbourCount; ++j )
//			{
//				sizeT neighbourIdx{ m_BoidArr.NeighbourFlag.Get( i * neighbourMax + j ) };
//				v += m_BoidArr.Position.Get( neighbourIdx );
//				++neighbourAcc;
//			}
//
//			if ( neighbourAcc > 0 )
//			{
//				float maxForce{ m_BoidVar.MaxForce };
//				grV2f vel{ m_BoidArr.Velocity.Get( i ) };
//				float maxSpeed{ m_BoidVar.MaxSpeed };
//				grV2f pos{ m_BoidArr.Position.Get( i ) };
//
//				v /= ( float )neighbourAcc;
//				v = pos.Between( v );
//				v.Normalize();
//				v *= maxSpeed;
//				v = vel.Between( v );
//				v.LimitMax( maxForce );
//
//				m_BoidArr.Cohesion.Set( i, v );
//			}
//		}
//	}
//
//	void Finalize( const float dt )
//	{
//		float sepWeight{ m_BoidVar.SepWeight };
//		float aliWeight{ m_BoidVar.AliWeight };
//		float cohWeight{ m_BoidVar.CohWeight };
//		float maxForce{ m_BoidVar.MaxForce };
//
//		sizeT alive{ m_BoidVar.Alive };
//
//		// Build acceleration
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v{ m_BoidArr.Separation.Get( i ) * sepWeight };
//			m_BoidArr.Acceleration.Set( i, m_BoidArr.Acceleration.Get( i ) + v );
//		}
//
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v{ m_BoidArr.Alignment.Get( i ) * aliWeight };
//			m_BoidArr.Acceleration.Set( i, m_BoidArr.Acceleration.Get( i ) + v );
//		}
//
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v{ m_BoidArr.Cohesion.Get( i ) * cohWeight };
//			m_BoidArr.Acceleration.Set( i, m_BoidArr.Acceleration.Get( i ) + v );
//		}
//
//		// Create velocity
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v{ m_BoidArr.Acceleration.Get( i ) };
//			v.LimitMax( maxForce );
//			m_BoidArr.Velocity.Set( i, m_BoidArr.Velocity.Get( i ) + v );
//		}
//
//		// Update position
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			grV2f v{ m_BoidArr.Velocity.Get( i ) };
//			m_BoidArr.Position.Set( i, m_BoidArr.Position.Get( i ) + v );
//		}
//	}
//
//	void Reset()
//	{
//		sizeT alive{ m_BoidVar.Alive };
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Separation.Set( i, { 0.0f, 0.0f } );
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Alignment.Set( i, { 0.0f, 0.0f } );
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Cohesion.Set( i, { 0.0f, 0.0f } );
//
//		for ( sizeT i = 0; i < alive; ++i )
//			m_BoidArr.Acceleration.Set( i, { 0.0f, 0.0f } );
//	}
//
//	void WinEdge()
//	{
//		const grV2u& winSize = grCore::Instance().GetWindowSize();
//		float r{ m_BoidVar.Radius };
//		sizeT alive{ m_BoidVar.Alive };
//
//		for ( sizeT i = 0; i < alive; ++i )
//		{
//			if ( m_BoidArr.Position.Get( i ).x < -r )
//			{
//				float y{ m_BoidArr.Position.Get( i ).y };
//				m_BoidArr.Position.Set( i, { ( float )winSize.x + r, y } );
//			}
//
//			if ( m_BoidArr.Position.Get( i ).x > ( float )winSize.x )
//			{
//				float y{ m_BoidArr.Position.Get( i ).y };
//				m_BoidArr.Position.Set( i, { -r, y } );
//			}
//
//			if ( m_BoidArr.Position.Get( i ).y < -r )
//			{
//				float x{ m_BoidArr.Position.Get( i ).x };
//				m_BoidArr.Position.Set( i, { x, ( float )winSize.y + r } );
//			}
//
//			if ( m_BoidArr.Position.Get( i ).y > ( float )winSize.y )
//			{
//				float x{ m_BoidArr.Position.Get( i ).x };
//				m_BoidArr.Position.Set( i, { x, -r } );
//			}
//		}
//	}
//
//	grSBoidArr m_BoidArr;
//	grSBoidVar m_BoidVar;
//};
//
//#endif // _H_GRBOIDSYS_
