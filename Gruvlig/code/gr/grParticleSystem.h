#ifndef _H_GRPARTICLESYSTEM_
#define _H_GRPARTICLESYSTEM_

#include "grAlgo.h"
#include "grParticleData.h"

struct grSColorSystem;
struct grSScaleSystem;
typedef void( grSColorSystem::* ColorFunc )( const sizeT startIdx, const sizeT endIdx );
typedef void( grSScaleSystem::* ScaleFunc )( const sizeT startIdx, const sizeT endIdx );


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
	void SetStartEnd( grColor::Rgba arrCol[], EEqualValue& rStartEqual, EEqualValue& rEndEqual, EEqualValue& rLerpEqual )
	{
		for ( sizeT i = 0; i < 4; ++i )
			arrCol[ i ] = ClampColor( arrCol[ i ] );

		rStartEqual = arrCol[ 0 ].Cmp( arrCol[ 1 ] ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = arrCol[ 2 ].Cmp( arrCol[ 3 ] ) ? EEqualValue::YES : EEqualValue::NO;

		if ( rStartEqual == EEqualValue::YES && rEndEqual == EEqualValue::YES )
			rLerpEqual = arrCol[ 1 ].Cmp( arrCol[ 2 ] ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SetStartEnd( const grV2f arrV2f[], EEqualValue& rStartEqual, EEqualValue& rEndEqual )
	{
		rStartEqual = grMath::CmpV2f( arrV2f[ 0 ], arrV2f[ 1 ] ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = grMath::CmpV2f( arrV2f[ 2 ], arrV2f[ 3 ] ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SetMinMax( grV2f& rMinMax, EEqualValue& rEqual, const bool swap = true )
	{
		if ( swap )
		{
			if ( rMinMax.x > rMinMax.y )
				grAlgo::Swap( rMinMax.x, rMinMax.y );
		}
		rEqual = grMath::CmpF( rMinMax.x, rMinMax.y ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SetMinMax( grV2f& rMin, grV2f& rMax, EEqualValue& rEqual, const bool swap = true )
	{
		if ( swap )
		{
			if ( rMin.x > rMax.x )
				grAlgo::Swap( rMin.x, rMax.x );
			if ( rMin.y > rMax.y )
				grAlgo::Swap( rMin.y, rMax.y );
		}
		rEqual = grMath::CmpV2f( rMin, rMax ) ? EEqualValue::YES : EEqualValue::NO;
	}

private:
	grColor::Rgba ClampColor( const grColor::Rgba& rColor )
	{
		return {
			grMath::Clamp<uint8_t>( rColor.R, 0, 255 ),
			grMath::Clamp<uint8_t>( rColor.G, 0, 255 ),
			grMath::Clamp<uint8_t>( rColor.B, 0, 255 ),
			grMath::Clamp<uint8_t>( rColor.A, 0, 255 )
		};
	}
};


struct grSColorSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSColorData& rColData;
	grSArrayData& rArrData;

	ColorFunc GenerateOption;

	grSColorSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rColData( *rData.puColor )
		, rArrData( *rData.puArray )
		, GenerateOption( &grSColorSystem::GenerateOption0 )
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
		rColData.LerpEqual = EEqualValue::NO; // A cheat if the default values set in the data struct would be used it doesn't trigger lerping
		SetStartEnd( rColData.ArrMinMax, rColData.StartEqual, rColData.EndEqual, rColData.LerpEqual );

		// Dislike all below code but I don't wan't to bloat with states or more weird looking functions so it will do for now
		if ( rColData.StartEqual == EEqualValue::NO && rColData.EndEqual == EEqualValue::NO )
		{
			InitDist( 0, 0 );
			InitDist( 4, 2 );
			GenerateOption = &grSColorSystem::GenerateOption0;
			return;
		}

		if ( rColData.StartEqual == EEqualValue::NO && rColData.EndEqual == EEqualValue::YES )
		{
			InitDist( 0, 0 );
			GenerateOption = &grSColorSystem::GenerateOption1;
			return;
		}

		if ( rColData.StartEqual == EEqualValue::YES && rColData.EndEqual == EEqualValue::NO )
		{
			InitDist( 4, 2 );
			GenerateOption = &grSColorSystem::GenerateOption2;
			return;
		}

		GenerateOption = &grSColorSystem::GenerateOption3;
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		( this->*GenerateOption )( startIdx, endIdx );
	}

	void Update()
	{
		if ( rColData.LerpEqual == EEqualValue::NO )
			rColData.bHsv ? LerpHsv() : LerpRgb();
	}

	IntUDist RangeDist( const uint16_t a, const uint16_t b )
	{
		return a < b ? rColData.Rand.DistU( a, b ) : rColData.Rand.DistU( b, a );
	}

	void InitDist( const sizeT arrStartIdx, const sizeT arrMinIdx )
	{
		rColData.ArrDist[ arrStartIdx ] = RangeDist( rColData.ArrMinMax[ arrMinIdx ].R, rColData.ArrMinMax[ arrMinIdx + 1 ].R );
		rColData.ArrDist[ arrStartIdx + 1 ] = RangeDist( rColData.ArrMinMax[ arrMinIdx ].G, rColData.ArrMinMax[ arrMinIdx + 1 ].G );
		rColData.ArrDist[ arrStartIdx + 2 ] = RangeDist( rColData.ArrMinMax[ arrMinIdx ].B, rColData.ArrMinMax[ arrMinIdx + 1 ].B );
		rColData.ArrDist[ arrStartIdx + 3 ] = RangeDist( rColData.ArrMinMax[ arrMinIdx ].A, rColData.ArrMinMax[ arrMinIdx + 1 ].A );
	}

	void InitColor( pU<grColor::Rgba[]>& rArr, const sizeT arrIdx, const sizeT distIdx )
	{
		rArr[ arrIdx ].R = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx ] ) );
		rArr[ arrIdx ].G = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx + 1 ] ) );
		rArr[ arrIdx ].B = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx + 2 ] ) );
		rArr[ arrIdx ].A = ( uint8_t )( rColData.Rand.IntU( rColData.ArrDist[ distIdx + 3 ] ) );
	}

	void GenerateOption0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorEnd, i, 4 );
	}

	void GenerateOption1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorEnd[ i ] = rColData.ArrMinMax[ 2 ];
	}

	void GenerateOption2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorStart[ i ] = rColData.ArrMinMax[ 0 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorEnd, i, 4 );
	}

	void GenerateOption3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorStart[ i ] = rColData.ArrMinMax[ 1 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorEnd[ i ] = rColData.ArrMinMax[ 2 ];
	}

	void LerpHsv()
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

	void LerpRgb()
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
};


struct grSScaleSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSScaleData& rScaData;
	grSArrayData& rArrData;

	ScaleFunc GnerateOption;

	grSScaleSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rScaData( *rData.puScale )
		, rArrData( *rData.puArray )
		, GnerateOption( &grSScaleSystem::GnerateOption0 )
	{}
	grSScaleSystem( const grSScaleSystem& ) = default;
	grSScaleSystem& operator=( const grSScaleSystem& ) = default;

	void Init( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax )
	{
		rScaData.ArrMinMax[ 0 ] = rStartMin;
		rScaData.ArrMinMax[ 1 ] = rStartMax;
		rScaData.ArrMinMax[ 2 ] = rEndMin;
		rScaData.ArrMinMax[ 3 ] = rEndMax;
		SetStartEnd( rScaData.ArrMinMax, rScaData.StartEqual, rScaData.EndEqual );

		if ( rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::NO ||
			 rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::YES ||
			 rScaData.StartEqual == EEqualValue::YES && rScaData.EndEqual == EEqualValue::NO )
		{
			rScaData.ArrDist[ 0 ] = InitDist( rScaData.ArrMinMax[ 0 ].x, rScaData.ArrMinMax[ 1 ].x );
			rScaData.ArrDist[ 1 ] = InitDist( rScaData.ArrMinMax[ 0 ].y, rScaData.ArrMinMax[ 1 ].y );
			rScaData.ArrDist[ 2 ] = InitDist( rScaData.ArrMinMax[ 2 ].x, rScaData.ArrMinMax[ 3 ].x );
			rScaData.ArrDist[ 3 ] = InitDist( rScaData.ArrMinMax[ 2 ].y, rScaData.ArrMinMax[ 3 ].y );
		}

		GnerateOption = rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::NO ?
			&grSScaleSystem::GnerateOption0 :
			rScaData.StartEqual == EEqualValue::NO && rScaData.EndEqual == EEqualValue::YES ?
			&grSScaleSystem::GnerateOption1 :
			rScaData.StartEqual == EEqualValue::YES && rScaData.EndEqual == EEqualValue::NO ?
			&grSScaleSystem::GnerateOption2 :
			&grSScaleSystem::GnerateOption3;
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		( this->*GnerateOption )( startIdx, endIdx );
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

	FloatDist InitDist( const float a, const float b )
	{
		return a < b ? rScaData.Rand.DistF( a, b ) : rScaData.Rand.DistF( b, a );
	}

	void GnerateOption0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 0 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 1 ] ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 2 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 3 ] ) };
	}

	void GnerateOption1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 0 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 1 ] ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = rScaData.ArrMinMax[ 2 ];
	}

	void GnerateOption2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = rScaData.ArrMinMax[ 1 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = { rScaData.Rand.Float( rScaData.ArrDist[ 2 ] ), rScaData.Rand.Float( rScaData.ArrDist[ 3 ] ) };
	}

	void GnerateOption3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = rScaData.ArrMinMax[ 1 ];

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = rScaData.ArrMinMax[ 2 ];
	}
};


struct grSMassSystem : public grSBaseSystem
{
	typedef std::uniform_real_distribution<float> MassDist;

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
		SetMinMax( rMasData.MinMax, rMasData.Equal );
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
		SetMinMax( VelData.DegreeMinMax, VelData.DegreeEqual, false );
		SetMinMax( VelData.ForceMinMax, VelData.ForceEqual );
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
				if ( d < 0.0f )
					d += 359.9f;

				return d;
			}

			auto distDeg{ VelData.Rand.DistF( VelData.DegreeMinMax.x, VelData.DegreeMinMax.y ) };
			return VelData.Rand.Float( distDeg );
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
};


struct grSPositionSystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSPositionData& rPosData;
	grSArrayData& rArrData;

	grSPositionSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rPosData( *rData.puPosition )
		, rArrData( *rData.puArray )
	{}
	grSPositionSystem( const grSPositionSystem& ) = default;
	grSPositionSystem& operator=( const grSPositionSystem& ) = default;

	void Init( const EPositionType positionType, const grV2f& rOffsetMin, const grV2f& rOffsetMax )
	{
		rPosData.PositionOffsetMin = rOffsetMin;
		rPosData.PositionOffsetMax = rOffsetMax;
		rPosData.PositionType = positionType;
		SetMinMax( rPosData.PositionOffsetMin, rPosData.PositionOffsetMax, rPosData.PositionEqual );

		rPosData.DistX = rPosData.Rand.DistF( rPosData.PositionOffsetMin.x, rPosData.PositionOffsetMax.x );
		rPosData.DistY = rPosData.Rand.DistF( rPosData.PositionOffsetMin.y, rPosData.PositionOffsetMax.y );
	}

	void Init( const EPositionType positionType, const grV2f& rRadiusMin, const grV2f& rRadiusMax, const grV2f& rStepMinMax, const grV2f& rTiltMinMax )
	{
		rPosData.EllipseRadiusMin = rRadiusMin;
		rPosData.EllipseRadiusMax = rRadiusMax;
		rPosData.EllipseStepMinMax = rStepMinMax;
		rPosData.EllipseTiltMinMax = rTiltMinMax;
		rPosData.PositionType = positionType;

		// position = Vector2( centerPos.x + ( radX * Mathf.Sin( Mathf.Deg2Rad * alpha ) ),
		// 					   centerPos.y + ( radY * Mathf.Cos( Mathf.Deg2Rad * alpha ) ) );
		   
		// alpha += 5f;//can be used as speed

		// position = Vector2( centerPos.x + ( radX * MCos( alpha ) * MCos( tilt ) ) - ( radY * MSin( alpha ) * MSin( tilt ) ),
		// 					   centerPos.y + ( radX * MCos( alpha ) * MSin( tilt ) ) + ( radY * MSin( alpha ) * MCos( tilt ) ) );
		// alpha += 5f;
	}

	void Generate()
	{
		//// Ellipse
		//grV2f v;
		//float radX{ 50.0f };
		//float radY{ 25.0f };

		//// Circular path pos
		//rPosition.EllipseStepCount += 4.0f * rEmit.Dt;
		//if( rPosition.EllipseStepCount > rPosition.Ellipse360 )
		//	rPosition.EllipseStepCount = 0.0f;

		//// Random circulr pos
		////auto distAlpha{ rPosition.Rand.DistF( 0.0f, rPosition.Ellipse360 ) };
		////rPosition.EllipseStepCount = rPosition.Rand.Float( distAlpha );

		//// Tilt
		//rPosition.EllipseTiltCount += grMath::DegToRad( 50.0f * rEmit.Dt );
		//if( rPosition.EllipseTiltCount > 360.0f )
		//	rPosition.EllipseTiltCount = 0.0f;
		//if( rPosition.EllipseTiltCount < 0.0f )
		//	rPosition.EllipseTiltCount = 360.0f;



		//// Regular
		//v.x = rEmit.SystemPosition.x + ( radX * std::sinf( rPosition.EllipseStepCount ) );
		//v.y = rEmit.SystemPosition.y + ( radY * std::cosf( rPosition.EllipseStepCount ) );

		//// Tilt
		////v.x = rEmit.SystemPosition.x + ( radX * std::cosf( rPosition.EllipseStepCount ) * std::cosf( rPosition.EllipseTiltCount ) ) -
		////	( radY * std::sinf( rPosition.EllipseStepCount ) * std::sinf( rPosition.EllipseTiltCount ) );

		////v.y = rEmit.SystemPosition.y + ( radX * std::cosf( rPosition.EllipseStepCount ) * std::sinf( rPosition.EllipseTiltCount ) ) +
		////	( radY * std::sinf( rPosition.EllipseStepCount ) * std::cosf( rPosition.EllipseTiltCount ) );

		//for ( sizeT i = rEmit.StartIdx; i < rEmit.EndIdx; ++i )
		//	rArray.Position[ i ] = v;



		// Box
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		if ( rPosData.PositionEqual == EEqualValue::NO )
		{
			if ( rPosData.PositionType == EPositionType::BOX )
			{
				PositionNotEqualBox();
				return;
			}
			//PositionNotEqualEllipse( rSystemPosition, startIdx, endIdx, dt );
			return;
		}

		if ( rPosData.PositionType == EPositionType::BOX )
		{
			PositionEqualBox();
			return;
		}
		//PositionEqualEllipse( rSystemPosition, startIdx, endIdx );
	}

	void Update()
	{
		float dt{ rEmiData.Dt };
		for ( sizeT i = 0; i < rEmiData.Alive; ++i )
			rArrData.Position[ i ] += rArrData.Velocity[ i ] * dt;
	}

	void PositionNotEqualBox()
	{
		for ( sizeT i = rEmiData.StartIdx; i < rEmiData.EndIdx; ++i )
		{
			grV2f v{ rPosData.Rand.Float( rPosData.DistX ), rPosData.Rand.Float( rPosData.DistY ) };
			rArrData.Position[ i ] = v + rEmiData.SystemPosition;
		}
	}

	void PositionNotEqualEllipse()
	{

	}

	void PositionEqualBox()
	{
		grV2f v{ rPosData.PositionOffsetMin };
		for ( sizeT i = rEmiData.StartIdx; i < rEmiData.EndIdx; ++i )
			rArrData.Position[ i ] = v + rEmiData.SystemPosition;
	}

	void PositionEqualEllipse()
	{

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
		SetMinMax( rLifData.MinMax, rLifData.Equal );
		rLifData.Dist = rLifData.Rand.DistF( rLifData.MinMax.x, rLifData.MinMax.y );
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

	void Kill( const sizeT nowIdx, const sizeT last )
	{
		//grAlgo::Swap( Array.puVerts[ nowIdx ], Array.puVerts[ last ] );
		grAlgo::Swap( rArrdata.ColorStart[ nowIdx ], rArrdata.ColorStart[ last ] );
		grAlgo::Swap( rArrdata.ColorEnd[ nowIdx ], rArrdata.ColorEnd[ last ] );
		grAlgo::Swap( rArrdata.ScaleStart[ nowIdx ], rArrdata.ScaleStart[ last ] );
		grAlgo::Swap( rArrdata.ScaleEnd[ nowIdx ], rArrdata.ScaleEnd[ last ] );
		grAlgo::Swap( rArrdata.Mass[ nowIdx ], rArrdata.Mass[ last ] );
		grAlgo::Swap( rArrdata.Acceleration[ nowIdx ], rArrdata.Acceleration[ last ] );
		grAlgo::Swap( rArrdata.Velocity[ nowIdx ], rArrdata.Velocity[ last ] );
		grAlgo::Swap( rArrdata.Position[ nowIdx ], rArrdata.Position[ last ] );
		grAlgo::Swap( rArrdata.Life[ nowIdx ], rArrdata.Life[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting obviously (all below)
		rArrdata.Acceleration[ last ] = { 0.0f, 0.0f };
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