#ifndef _H_GRPARTICLESYSTEM_
#define _H_GRPARTICLESYSTEM_

#include "grAlgo.h"
#include "grParticleData.h"

struct grSColorSystem;
struct grSScaleSystem;
struct grSPositionSystem;
typedef void( grSColorSystem::* ColGenOpt )( const sizeT startIdx, const sizeT endIdx );
typedef void( grSScaleSystem::* ScaGenOpt )( const sizeT startIdx, const sizeT endIdx );
typedef void( grSPositionSystem::* PosGenOpt )( const sizeT startIdx, const sizeT endIdx );


struct grSEmitSystem
{
	grSEmitData& rEmiData;

	grSEmitSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
	{}
	grSEmitSystem( const grSEmitSystem& ) = default;
	grSEmitSystem& operator=( const grSEmitSystem& ) = default;

	void Generate( const float dt )
	{
		rEmiData.Dt = dt;
		rEmiData.SpawnTimeAcc += rEmiData.Dt;
		rEmiData.EmitAcc = 0;
		while ( rEmiData.SpawnTimeAcc >= rEmiData.EmitRateMs )
		{
			rEmiData.SpawnTimeAcc -= rEmiData.EmitRateMs;
			rEmiData.EmitAcc += 1;
		}

		if ( rEmiData.EmitAcc > 0 )
		{
			sizeT last{ rEmiData.Size - 1 };
			rEmiData.StartIdx = rEmiData.Alive;
			rEmiData.EndIdx = grMath::Min<sizeT>( rEmiData.StartIdx + rEmiData.EmitAcc, last );
			if ( rEmiData.StartIdx == rEmiData.EndIdx )
				return;

			rEmiData.Alive += rEmiData.EndIdx - rEmiData.StartIdx;
		}
	}
};


struct grSBaseSystem
{
	void EqualCheck( grV2f& rMinMax, EEqualValue& rEqual )
	{
		rEqual = grMath::CmpF( rMinMax.x, rMinMax.y ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void EqualCheckColor( grColor::Rgba arrCol[], EEqualValue& rStartEqual, EEqualValue& rEndEqual, EEqualValue& rLerpEqual )
	{
		rStartEqual = arrCol[ 0 ].Cmp( arrCol[ 1 ] ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = arrCol[ 2 ].Cmp( arrCol[ 3 ] ) ? EEqualValue::YES : EEqualValue::NO;

		if ( rStartEqual == EEqualValue::YES && rEndEqual == EEqualValue::YES )
			rLerpEqual = arrCol[ 1 ].Cmp( arrCol[ 2 ] ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void EqualCheckScale( const grV2f arrV2f[], EEqualValue& rStartEqual, EEqualValue& rEndEqual )
	{
		rStartEqual = grMath::CmpV2f( arrV2f[ 0 ], arrV2f[ 1 ] ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = grMath::CmpV2f( arrV2f[ 2 ], arrV2f[ 3 ] ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void EqualCheck( const grV2f arrV2f[], EEqualValue& rEqualX, EEqualValue& rEqualY )
	{
		rEqualX = grMath::CmpF( arrV2f[ 0 ].x, arrV2f[ 1 ].x ) ? EEqualValue::YES : EEqualValue::NO;
		rEqualY = grMath::CmpF( arrV2f[ 0 ].y, arrV2f[ 1 ].y ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SwapCheckColor( grColor::Rgba arr[], const sizeT arrIdx1, const sizeT arrIdx2 )
	{
		if ( arr[ arrIdx1 ].R > arr[ arrIdx2 ].R )
			grAlgo::Swap<uint8_t>( arr[ arrIdx1 ].R, arr[ arrIdx2 ].R );

		if ( arr[ arrIdx1 ].G > arr[ arrIdx2 ].G )
			grAlgo::Swap<uint8_t>( arr[ arrIdx1 ].G, arr[ arrIdx2 ].G );

		if ( arr[ arrIdx1 ].B > arr[ arrIdx2 ].B )
			grAlgo::Swap<uint8_t>( arr[ arrIdx1 ].B, arr[ arrIdx2 ].B );

		if ( arr[ arrIdx1 ].A > arr[ arrIdx2 ].A )
			grAlgo::Swap<uint8_t>( arr[ arrIdx1 ].A, arr[ arrIdx2 ].A );
	}

	void SwapCheck( grV2f& rMinMax )
	{
		if ( rMinMax.x > rMinMax.y )
			grAlgo::Swap( rMinMax.x, rMinMax.y );
	}

	void SwapCheck( grV2f& rMin, grV2f& rMax )
	{
		if ( rMin.x > rMax.x )
			grAlgo::Swap( rMin.x, rMax.x );
		if ( rMin.y > rMax.y )
			grAlgo::Swap( rMin.y, rMax.y );
	}
};


struct grSColorSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSColorData& rColData;
	grSArrayData& rArrData;

	ColGenOpt GenOption;

	grSColorSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rColData( *rData.puColor )
		, rArrData( *rData.puArray )
		, GenOption( &grSColorSystem::GenOption3 )
	{}
	grSColorSystem( const grSColorSystem& ) = default;
	grSColorSystem& operator=( const grSColorSystem& ) = default;

	void Init( const grColor::Rgba& rStartMin, const grColor::Rgba& rStartMax, const grColor::Rgba& rEndMin, const grColor::Rgba& rEndMax, const bool hsv )
	{
		rColData.ArrMinMax[ 0 ] = rStartMin;
		rColData.ArrMinMax[ 1 ] = rStartMax;
		rColData.ArrMinMax[ 2 ] = rEndMin;
		rColData.ArrMinMax[ 3 ] = rEndMax;
		rColData.bHsv = hsv;
		SwapCheckColor( rColData.ArrMinMax, 0, 1 );
		SwapCheckColor( rColData.ArrMinMax, 2, 3 );
		rColData.LerpEqual = EEqualValue::NO; // A cheat if the default values set in the data struct would be used it doesn't trigger lerping
		EqualCheckColor( rColData.ArrMinMax, rColData.StartEqual, rColData.EndEqual, rColData.LerpEqual );

		// Dislike all below code but I don't wan't to bloat with states or more weird looking functions so it will do for now
		if ( rColData.StartEqual == EEqualValue::NO && rColData.EndEqual == EEqualValue::NO )
		{
			InitDist( 0, 0, 1 );
			InitDist( 4, 2, 3 );
			GenOption = &grSColorSystem::GenOption0;
			return;
		}

		if ( rColData.StartEqual == EEqualValue::NO && rColData.EndEqual == EEqualValue::YES )
		{
			InitDist( 0, 0, 1 );
			GenOption = &grSColorSystem::GenOption1;
			return;
		}

		if ( rColData.StartEqual == EEqualValue::YES && rColData.EndEqual == EEqualValue::NO )
		{
			InitDist( 4, 2, 3 );
			GenOption = &grSColorSystem::GenOption2;
			return;
		}

		GenOption = &grSColorSystem::GenOption3;
	}

	void InitDist( const sizeT arrDistIdx, const sizeT arrMinMaxIdx1, const sizeT arrMinMaxIdx2 )
	{
		rColData.ArrDist[ arrDistIdx ] = rColData.Rand.DistU( rColData.ArrMinMax[ arrMinMaxIdx1 ].R, rColData.ArrMinMax[ arrMinMaxIdx2 ].R );
		rColData.ArrDist[ arrDistIdx + 1 ] = rColData.Rand.DistU( rColData.ArrMinMax[ arrMinMaxIdx1 ].G, rColData.ArrMinMax[ arrMinMaxIdx2 ].G );
		rColData.ArrDist[ arrDistIdx + 2 ] = rColData.Rand.DistU( rColData.ArrMinMax[ arrMinMaxIdx1 ].B, rColData.ArrMinMax[ arrMinMaxIdx2 ].B );
		rColData.ArrDist[ arrDistIdx + 3 ] = rColData.Rand.DistU( rColData.ArrMinMax[ arrMinMaxIdx1 ].A, rColData.ArrMinMax[ arrMinMaxIdx2 ].A );
	}

	void InitColor( pU<grColor::Rgba[]>& rArr, const sizeT arrIdx, const sizeT distIdx )
	{
		rArr[ arrIdx ].R = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx ] ) );
		rArr[ arrIdx ].G = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx + 1 ] ) );
		rArr[ arrIdx ].B = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx + 2 ] ) );
		rArr[ arrIdx ].A = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx + 3 ] ) );
	}

	void GenOption0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorEnd, i, 4 );
	}

	void GenOption1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorEnd[ i ] = rColData.ArrMinMax[ 2 ];
	}

	void GenOption2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorStart[ i ] = rColData.ArrMinMax[ 0 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorEnd, i, 4 );
	}

	void GenOption3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorStart[ i ] = rColData.ArrMinMax[ 1 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorEnd[ i ] = rColData.ArrMinMax[ 2 ];
	}

	void HsvLerp()
	{
		// RGB -> HSV lerp -> RGB

		float dt{ rEmiData.Dt };
		sizeT alive{ rEmiData.Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			float lerpValue{ 1.0f / rArrData.Life[ i ] * dt };

			// Faster to first store localy and then lerp instead of passing to lerp directly by indexing the array // Measured with std::chronos
			grColor::Hsva start{ grColor::Rgba2Hsva( rArrData.ColorStart[ i ] ) };
			grColor::Hsva end{ grColor::Rgba2Hsva( rArrData.ColorEnd[ i ] ) };

			start.H = grMath::Lerp( start.H, end.H, lerpValue );
			start.S = grMath::Lerp( start.S, end.S, lerpValue );
			start.V = grMath::Lerp( start.V, end.V, lerpValue );
			start.A = grMath::Lerp( start.A, end.A, lerpValue );

			rArrData.ColorStart[ i ] = grColor::Hsva2Rgba( start );
		}
	}

	void RgbLerp()
	{
		float dt{ rEmiData.Dt };
		sizeT alive{ rEmiData.Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			float lerpValue{ 1.0f / rArrData.Life[ i ] * dt };

			grColor::Rgba start{ rArrData.ColorStart[ i ] };
			grColor::Rgba end{ rArrData.ColorEnd[ i ] };

			rArrData.ColorStart[ i ].R = ( uint8_t )grMath::Lerp( ( float )start.R, ( float )end.R, lerpValue );
			rArrData.ColorStart[ i ].G = ( uint8_t )grMath::Lerp( ( float )start.G, ( float )end.G, lerpValue );
			rArrData.ColorStart[ i ].B = ( uint8_t )grMath::Lerp( ( float )start.B, ( float )end.B, lerpValue );
			rArrData.ColorStart[ i ].A = ( uint8_t )grMath::Lerp( ( float )start.A, ( float )end.A, lerpValue );
		}
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		( this->*GenOption )( startIdx, endIdx );
	}

	void Update()
	{
		if ( rColData.LerpEqual == EEqualValue::NO )
			rColData.bHsv ? HsvLerp() : RgbLerp();
	}
};


struct grSScaleSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSScaleData& rScaData;
	grSArrayData& rArrData;

	ScaGenOpt GenOption;

	grSScaleSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rScaData( *rData.puScale )
		, rArrData( *rData.puArray )
		, GenOption( &grSScaleSystem::GenOption3 )
	{}
	grSScaleSystem( const grSScaleSystem& ) = default;
	grSScaleSystem& operator=( const grSScaleSystem& ) = default;

	void Init( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax )
	{
		rScaData.ArrMinMax[ 0 ] = rStartMin;
		rScaData.ArrMinMax[ 1 ] = rStartMax;
		rScaData.ArrMinMax[ 2 ] = rEndMin;
		rScaData.ArrMinMax[ 3 ] = rEndMax;
		EqualCheckScale( rScaData.ArrMinMax, rScaData.StartEqual, rScaData.EndEqual );

		if ( rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::NO ||
			 rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::YES ||
			 rScaData.StartEqual == EEqualValue::YES && rScaData.EndEqual == EEqualValue::NO )
		{
			rScaData.ArrDist[ 0 ] = InitDist( rScaData.ArrMinMax[ 0 ].x, rScaData.ArrMinMax[ 1 ].x );
			rScaData.ArrDist[ 1 ] = InitDist( rScaData.ArrMinMax[ 0 ].y, rScaData.ArrMinMax[ 1 ].y );
			rScaData.ArrDist[ 2 ] = InitDist( rScaData.ArrMinMax[ 2 ].x, rScaData.ArrMinMax[ 3 ].x );
			rScaData.ArrDist[ 3 ] = InitDist( rScaData.ArrMinMax[ 2 ].y, rScaData.ArrMinMax[ 3 ].y );
		}

		GenOption = rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::NO ?
			&grSScaleSystem::GenOption0 :
			rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::YES ?
			&grSScaleSystem::GenOption1 :
			rScaData.StartEqual == EEqualValue::YES && rScaData.EndEqual == EEqualValue::NO ?
			&grSScaleSystem::GenOption2 :
			&grSScaleSystem::GenOption3;
	}

	DistF InitDist( const float a, const float b )
	{
		return a < b ? rScaData.Rand.DistF( a, b ) : rScaData.Rand.DistF( b, a );
	}

	void GenOption0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 0 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 1 ] ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 2 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 3 ] ) };
	}

	void GenOption1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 0 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 1 ] ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = rScaData.ArrMinMax[ 2 ];
	}

	void GenOption2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = rScaData.ArrMinMax[ 1 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 2 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 3 ] ) };
	}

	void GenOption3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = rScaData.ArrMinMax[ 1 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = rScaData.ArrMinMax[ 2 ];
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		( this->*GenOption )( startIdx, endIdx );
	}

	void Update()
	{
		float dt{ rEmiData.Dt };
		for ( sizeT i = 0; i < rEmiData.Alive; ++i )
		{
			float step{ ( 1.0f / rArrData.Life[ i ] ) * dt };
			grV2f start{ rArrData.ScaleStart[ i ] };
			grV2f end{ rArrData.ScaleEnd[ i ] };
			rArrData.ScaleStart[ i ] = grMath::LerpV2f( start, end, step );
		}
	}
};


struct grSMassSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSMassData& rMasData;
	grSArrayData& rArrData;

	grSMassSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rMasData( *rData.puMass )
		, rArrData( *rData.puArray )
	{}
	grSMassSystem( const grSMassSystem& ) = default;
	grSMassSystem& operator=( const grSMassSystem& ) = default;

	void Init( const grV2f& rMinMax )
	{
		rMasData.MinMax.x = grMath::Max( rMinMax.x, 1.0f );
		rMasData.MinMax.y = grMath::Max( rMinMax.y, 1.0f );
		SwapCheck( rMasData.MinMax );
		EqualCheck( rMasData.MinMax, rMasData.Equal );
		rMasData.Dist = rMasData.Rand.DistF( rMasData.MinMax.x, rMasData.MinMax.y );
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		if ( rMasData.Equal == EEqualValue::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rArrData.Mass[ i ] = rMasData.Rand.Float( rMasData.Dist );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.Mass[ i ] = rMasData.MinMax.x;
	}
};


struct grSVelocitySystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSVelocityData& VelData;
	grSArrayData& rArrData;

	grSVelocitySystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, VelData( *rData.puVelocity )
		, rArrData( *rData.puArray )
	{}
	grSVelocitySystem( const grSVelocitySystem& ) = default;
	grSVelocitySystem& operator=( const grSVelocitySystem& ) = default;

	void Init( const grV2f& rDegreeMinMax, const grV2f& rForceMinMax )
	{
		VelData.DegreeMinMax = grV2f( grMath::Clamp<float>( rDegreeMinMax.x, 0.0f, 359.9f ), grMath::Clamp<float>( rDegreeMinMax.y, 0.0f, 359.9f ) );
		VelData.ForceMinMax = rForceMinMax;
		SwapCheck( VelData.ForceMinMax );
		EqualCheck( VelData.DegreeMinMax, VelData.DegreeEqual );
		EqualCheck( VelData.ForceMinMax, VelData.ForceEqual );
	}

	float FindDegrees()
	{
		if ( VelData.DegreeEqual == EEqualValue::NO )
		{
			if ( VelData.DegreeMinMax.x > VelData.DegreeMinMax.y )
			{
				float d{ 0.0f };
				float diff{ 359.9f - VelData.DegreeMinMax.x };
				VelData.Dist = VelData.Rand.DistF( 0.0f, diff + VelData.DegreeMinMax.y );
				d = VelData.Rand.Float( VelData.Dist ) - diff;
				d += d < 0.0f ? 359.9f : 0.0f;
				return d;
			}

			VelData.Dist = VelData.Rand.DistF( VelData.DegreeMinMax.x, VelData.DegreeMinMax.y );
			return VelData.Rand.Float( VelData.Dist );
		}

		return VelData.DegreeMinMax.x;
	}

	float FindForce()
	{
		if ( VelData.ForceEqual == EEqualValue::NO )
		{
			auto distForce{ VelData.Rand.DistF( VelData.ForceMinMax.x, VelData.ForceMinMax.y ) };
			return VelData.Rand.Float( distForce );
		}

		return VelData.ForceMinMax.x;
	}

	void Generate()
	{
		// This is retarded but I'm retarded and can't figure out how to do it without using 6 different loops which would be better but look more retarded
		// It shall remain retarded for the moment(s)

		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			float d{ FindDegrees() };
			float f{ FindForce() };
			grV2f v{ grMath::DegToVec( d ) * f };
			rArrData.Velocity[ i ] = v / rArrData.Mass[ i ];
		}
	}

	void Update()
	{
		float dt{ rEmiData.Dt };
		for ( sizeT i = 0; i < rEmiData.Alive; ++i )
			rArrData.Velocity[ i ] += rArrData.Acceleration[ i ].x * dt;
	}
};


// Position system doubles as spawn shape and position update
struct grSPositionSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSPositionData& rPosData;
	grSArrayData& rArrData;

	PosGenOpt GenOption;

	grSPositionSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rPosData( *rData.puPosition )
		, rArrData( *rData.puArray )
		, GenOption( &grSPositionSystem::BoxGenOption3 )
	{}
	grSPositionSystem( const grSPositionSystem& ) = default;
	grSPositionSystem& operator=( const grSPositionSystem& ) = default;

	void InitBox( const grV2f& rBoxOffsetMin, const grV2f& rBoxOffsetMax, const float radius ) // Radius 0.0f equals filled box // Radius !0.0f equals framed box
	{
		rPosData.ArrMinMax[ 0 ] = rBoxOffsetMin;
		rPosData.ArrMinMax[ 1 ] = rBoxOffsetMax;
		rPosData.BoxRadius = grMath::AbsF( radius );
		EqualCheck( rPosData.ArrMinMax, rPosData.EqualBoxX, rPosData.EqualBoxY );
		SwapCheck( rPosData.ArrMinMax[ 0 ], rPosData.ArrMinMax[ 1 ] );

		if ( rPosData.EqualBoxX == EEqualValue::NO && rPosData.EqualBoxY == EEqualValue::NO )
		{
			rPosData.ArrDist[ 0 ] = rPosData.Rand.DistF( rPosData.ArrMinMax[ 0 ].x, rPosData.ArrMinMax[ 1 ].x );
			rPosData.ArrDist[ 1 ] = rPosData.Rand.DistF( rPosData.ArrMinMax[ 0 ].y, rPosData.ArrMinMax[ 1 ].y );
			GenOption = &grSPositionSystem::BoxGenOption0;
			return;
		}

		if ( rPosData.EqualBoxX == EEqualValue::NO && rPosData.EqualBoxY == EEqualValue::YES )
		{
			rPosData.ArrDist[ 0 ] = rPosData.Rand.DistF( rPosData.ArrMinMax[ 0 ].x, rPosData.ArrMinMax[ 1 ].x );
			GenOption = &grSPositionSystem::BoxGenOption1;
			return;
		}

		if ( rPosData.EqualBoxX == EEqualValue::YES && rPosData.EqualBoxY == EEqualValue::NO )
		{
			rPosData.ArrDist[ 1 ] = rPosData.Rand.DistF( rPosData.ArrMinMax[ 0 ].y, rPosData.ArrMinMax[ 1 ].y );
			GenOption = &grSPositionSystem::BoxGenOption2;
			return;
		}

		GenOption = &grSPositionSystem::BoxGenOption3;
	}

	void InitCircle( const grV2f& rRadiusMinMax )
	{
		rPosData.ArrMinMax[ 0 ] = rRadiusMinMax;
		EqualCheck( rPosData.ArrMinMax[ 0 ], rPosData.EqualCircle );
		SwapCheck( rPosData.ArrMinMax[ 0 ] );

		GenOption = rPosData.EqualCircle == EEqualValue::NO ? &grSPositionSystem::CircleGenOption0 : &grSPositionSystem::CircleGenOption1;
	}

	void CircleGenOption0( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f sysPos{ rEmiData.SystemPosition };
		DistF distRad{ rPosData.Rand.DistF( rPosData.ArrMinMax[ 0 ] ) };
		DistF distDeg{ rPosData.Rand.DistF( 0.0f, 359.9f ) };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			grV2f v{ grMath::DegToVec( rPosData.Rand.Float( distDeg ) ) };
			v *= rPosData.Rand.Float( distRad );
			rArrData.Position[ i ] = v + sysPos;
		}
	}

	void CircleGenOption1( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f sysPos{ rEmiData.SystemPosition };
		DistF distDeg{ rPosData.Rand.DistF( 0.0f, 359.9f ) };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			grV2f v{ grMath::DegToVec( rPosData.Rand.Float( distDeg ) ) };
			v *= rPosData.ArrMinMax[ 0 ].x;
			rArrData.Position[ i ] = v + sysPos;
		}
	}

// position = Vector2( centerPos.x + ( radX * Mathf.Sin( Mathf.Deg2Rad * alpha ) ),
// 					   centerPos.y + ( radY * Mathf.Cos( Mathf.Deg2Rad * alpha ) ) );

// alpha += 5f;//can be used as speed

// position = Vector2( centerPos.x + ( radX * MCos( alpha ) * MCos( tilt ) ) - ( radY * MSin( alpha ) * MSin( tilt ) ),
// 					   centerPos.y + ( radX * MCos( alpha ) * MSin( tilt ) ) + ( radY * MSin( alpha ) * MCos( tilt ) ) );
// alpha += 5f;




	void BoxGenOption0( const sizeT startIdx, const sizeT endIdx )
	{
		// Filled box
		grV2f sysPos{ rEmiData.SystemPosition };
		if ( rPosData.BoxRadius == 0.0f )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f v{ rPosData.Rand.Float( rPosData.ArrDist[ 0 ] ), rPosData.Rand.Float( rPosData.ArrDist[ 1 ] ) };
				rArrData.Position[ i ] = v + sysPos;
			}
			return;
		}

		// Framed box
		float xRange = ( grMath::AbsF( rPosData.ArrMinMax[ 0 ].x ) + grMath::AbsF( rPosData.ArrMinMax[ 1 ].x ) ) * 0.5f;
		float yRange = ( grMath::AbsF( rPosData.ArrMinMax[ 0 ].y ) + grMath::AbsF( rPosData.ArrMinMax[ 1 ].y ) ) * 0.5f;
		DistF DistHori{ rPosData.Rand.DistF( -xRange, xRange ) };
		DistF DistVert{ rPosData.Rand.DistF( -yRange, yRange ) };

		grV2f vOrigoN = grMath::DegToVec( 0.0f );
		grV2f vAdjecentN = grMath::DegToVec( 90.0f );

		grV2f vOrigoE = grMath::DegToVec( 90.0f );
		grV2f vAdjecentE = grMath::DegToVec( 180.0f );

		grV2f vOrigoS = grMath::DegToVec( 180.0f );
		grV2f vAdjecentS = grMath::DegToVec( 270.0f );

		grV2f vOrigoW = grMath::DegToVec( 270.0f );
		grV2f vAdjecentW = grMath::DegToVec( 0.0f );

		float degAcc{ 360.0f };
		float flipFlop{ -1.0f };

		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			flipFlop *= -1.0f;
			degAcc = degAcc > 359.9f ? 0.0f : degAcc + 90.0f;

			grV2f v = 
				( degAcc == 90.0f ) ?
				grV2f( vOrigoE ) * grMath::AbsF( rPosData.ArrMinMax[ 1 ].x ) + grV2f( vAdjecentE ) * rPosData.Rand.Float( DistVert ) :
				( degAcc == 180.0f ) ?
				grV2f( vOrigoS ) * grMath::AbsF( rPosData.ArrMinMax[ 0 ].y ) + grV2f( vAdjecentS ) * rPosData.Rand.Float( DistHori ) :
				( degAcc == 270.0f ) ?
				grV2f( vOrigoW ) * grMath::AbsF( rPosData.ArrMinMax[ 0 ].x ) + grV2f( vAdjecentW ) * rPosData.Rand.Float( DistVert ) :
				grV2f( vOrigoN ) * grMath::AbsF( rPosData.ArrMinMax[ 1 ].y ) + grV2f( vAdjecentN ) * rPosData.Rand.Float( DistHori );

			rArrData.Position[ i ] = sysPos + v;
		}
	}

	void BoxGenOption1( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f v{ grV2f( 0.0f, rPosData.ArrMinMax[ 0 ].y ) + rEmiData.SystemPosition };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			v.x += rPosData.Rand.Float( rPosData.ArrDist[ 0 ] );
			rArrData.Position[ i ] = v;
		}
	}

	void BoxGenOption2( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f v{ grV2f( rPosData.ArrMinMax[ 0 ].x, 0.0f ) + rEmiData.SystemPosition };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			v.y += rPosData.Rand.Float( rPosData.ArrDist[ 1 ] );
			rArrData.Position[ i ] = v;
		}
	}

	void BoxGenOption3( const sizeT startIdx, const sizeT endIdx )
	{
		grV2f v{ grV2f( rPosData.ArrMinMax[ 0 ] ) + rEmiData.SystemPosition };
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.Position[ i ] = v;
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		( this->*GenOption )( startIdx, endIdx );
	}

	void Update()
	{
		float dt{ rEmiData.Dt };
		for ( sizeT i = 0; i < rEmiData.Alive; ++i )
			rArrData.Position[ i ] += rArrData.Velocity[ i ] * dt;
	}
};


struct grSLifeSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSLifeData& rLifData;
	grSArrayData& rArrdata;

	grSLifeSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rLifData( *rData.puLife )
		, rArrdata( *rData.puArray )
	{}
	grSLifeSystem( const grSLifeSystem& ) = default;
	grSLifeSystem& operator=( const grSLifeSystem& ) = default;

	void Init( const grV2f& rMinMax )
	{
		rLifData.MinMax = rMinMax;
		SwapCheck( rLifData.MinMax );
		EqualCheck( rLifData.MinMax, rLifData.Equal );
		rLifData.Dist = rLifData.Rand.DistF( rLifData.MinMax.x, rLifData.MinMax.y );
	}

	void Kill( const sizeT nowIdx, const sizeT last )
	{
		//grAlgo::Swap( Array.puVerts[ nowIdx ], Array.puVerts[ last ] );
		grAlgo::Swap<grColor::Rgba>( rArrdata.ColorStart[ nowIdx ], rArrdata.ColorStart[ last ] );
		grAlgo::Swap<grColor::Rgba>( rArrdata.ColorEnd[ nowIdx ], rArrdata.ColorEnd[ last ] );
		grAlgo::Swap<grV2f>( rArrdata.ScaleStart[ nowIdx ], rArrdata.ScaleStart[ last ] );
		grAlgo::Swap<grV2f>( rArrdata.ScaleEnd[ nowIdx ], rArrdata.ScaleEnd[ last ] );
		grAlgo::Swap<float>( rArrdata.Mass[ nowIdx ], rArrdata.Mass[ last ] );
		grAlgo::Swap<grV2f>( rArrdata.Acceleration[ nowIdx ], rArrdata.Acceleration[ last ] );
		grAlgo::Swap<grV2f>( rArrdata.Velocity[ nowIdx ], rArrdata.Velocity[ last ] );
		grAlgo::Swap<grV2f>( rArrdata.Position[ nowIdx ], rArrdata.Position[ last ] );
		grAlgo::Swap<float>( rArrdata.Life[ nowIdx ], rArrdata.Life[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting (all below) with the exception of position
		rArrdata.Acceleration[ last ] = { 0.0f, 0.0f };
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		if ( rLifData.Equal == EEqualValue::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rArrdata.Life[ i ] = rLifData.Rand.Float( rLifData.Dist );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrdata.Life[ i ] = rLifData.MinMax.x;
	}

	void Update()
	{
		for ( sizeT i = 0; i < rEmiData.Alive; ++i )
		{
			rArrdata.Life[ i ] -= rEmiData.Dt;;
			if ( rArrdata.Life[ i ] <= 0.0f )
				Kill( i, --rEmiData.Alive );
		}
	}
};


struct grSParticleSystem
{
	pU<grSEmitSystem> puEmit;
	pU<grSColorSystem> puColor;
	pU<grSScaleSystem> puScale;
	pU<grSMassSystem> puMass;
	pU<grSVelocitySystem> puVelocity;
	pU<grSPositionSystem> puPosition;
	pU<grSLifeSystem> puLife;

	grSParticleSystem() = default;
	grSParticleSystem( const grSParticleSystem& ) = delete;
	grSParticleSystem& operator=( const grSParticleSystem& ) = delete;
	grSParticleSystem( grSParticleSystem&& ) noexcept = delete;
	grSParticleSystem& operator=( grSParticleSystem&& ) noexcept = delete;

	void Init( const grSParticleData& rData )
	{
		puEmit = std::make_unique<grSEmitSystem>( rData );
		puColor = std::make_unique<grSColorSystem>( rData );
		puScale = std::make_unique<grSScaleSystem>( rData );
		puMass = std::make_unique<grSMassSystem>( rData );
		puVelocity = std::make_unique<grSVelocitySystem>( rData );
		puPosition = std::make_unique<grSPositionSystem>( rData );
		puLife = std::make_unique<grSLifeSystem>( rData );
	}

	void Generate( const float dt )
	{
		puEmit->Generate( dt );
		if ( puEmit->rEmiData.EmitAcc > 0 )
		{
			puColor->Generate();
			puScale->Generate();
			puMass->Generate();
			puVelocity->Generate();
			puPosition->Generate();
			puLife->Generate();
		}
	}

	void Update()
	{
		if ( puEmit->rEmiData.Alive > 0 )
		{
			puColor->Update();
			puScale->Update();
			puVelocity->Update();
			puPosition->Update();
			puLife->Update();
		}
	}

	void Run( const float dt )
	{
		Generate( dt );
		Update();
	}
};

#endif // _H_GRPARTICLESYSTEM_