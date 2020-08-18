#ifndef _H_GRPARTICLESYSTEM_
#define _H_GRPARTICLESYSTEM_

#include "grAlgo.h"
#include "grParticleData.h"


struct grSBaseSystem
{
	void SetStartEnd( grColor::Rgba& rStartMin, grColor::Rgba& rStartMax, grColor::Rgba& rEndMin, grColor::Rgba& rEndMax, EEqualValue& rStartEqual, EEqualValue& rEndEqual )
	{
		rStartMin = ClampColor( rStartMin );
		rStartMax = ClampColor( rStartMax );
		rEndMin = ClampColor( rEndMin );
		rEndMax = ClampColor( rEndMax );

		rStartEqual = rStartMin.Cmp( rStartMax ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = rEndMin.Cmp( rEndMax ) ? EEqualValue::YES : EEqualValue::NO;
	}

	void SetStartEnd( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax, EEqualValue& rStartEqual, EEqualValue& rEndEqual )
	{
		rStartEqual = grMath::CmpV2f( rStartMin, rStartMax ) ? EEqualValue::YES : EEqualValue::NO;
		rEndEqual = grMath::CmpV2f( rEndMin, rEndMax ) ? EEqualValue::YES : EEqualValue::NO;
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
	typedef void( grSColorSystem::*ColorOption )( const sizeT startIdx, const sizeT endIdx );

	grSEmitData& rEmiData;
	grSColorData& rColData;
	grSArrayData& rArrData;

	ColorOption Option;

	grSColorSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rColData( *rData.puColor )
		, rArrData( *rData.puArray )
		, Option( &grSColorSystem::Option0 )
	{}
	grSColorSystem( const grSColorSystem& ) = default;
	grSColorSystem& operator=( const grSColorSystem& ) = default;

	void Init( const grColor::Rgba& rStartMin, const grColor::Rgba& rStartMax, const grColor::Rgba& rEndMin, const grColor::Rgba& rEndMax, const bool hsv )
	{
		rColData.ColorStartMin = rStartMin;
		rColData.ColorStartMax = rStartMax;
		rColData.ColorEndMin = rEndMin;
		rColData.ColorEndMax = rEndMax;
		rColData.bHsv = hsv;
		SetStartEnd( rColData.ColorStartMin, rColData.ColorStartMax, rColData.ColorEndMin, rColData.ColorEndMax, rColData.ColorStartEqual, rColData.ColorEndEqual );

		rColData.puDistArr[ 0 ] = InitDist( rColData.ColorStartMin.R, rColData.ColorStartMax.R );
		rColData.puDistArr[ 1 ] = InitDist( rColData.ColorStartMin.G, rColData.ColorStartMax.G );
		rColData.puDistArr[ 2 ] = InitDist( rColData.ColorStartMin.B, rColData.ColorStartMax.B );
		rColData.puDistArr[ 3 ] = InitDist( rColData.ColorStartMin.A, rColData.ColorStartMax.A );

		rColData.puDistArr[ 4 ] = InitDist( rColData.ColorEndMin.R, rColData.ColorEndMax.R );
		rColData.puDistArr[ 5 ] = InitDist( rColData.ColorEndMin.G, rColData.ColorEndMax.G );
		rColData.puDistArr[ 6 ] = InitDist( rColData.ColorEndMin.B, rColData.ColorEndMax.B );
		rColData.puDistArr[ 7 ] = InitDist( rColData.ColorEndMin.A, rColData.ColorEndMax.A );

		Option = rColData.ColorStartEqual == EEqualValue::NO && rColData.ColorEndEqual == EEqualValue::NO ?
			&grSColorSystem::Option0 :
			rColData.ColorStartEqual == EEqualValue::NO && rColData.ColorEndEqual == EEqualValue::YES ?
			&grSColorSystem::Option1 :
			rColData.ColorStartEqual == EEqualValue::YES && rColData.ColorEndEqual == EEqualValue::NO ?
			&grSColorSystem::Option2 :
			&grSColorSystem::Option3;
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		( this->*Option )( startIdx, endIdx );
	}

	void Update()
	{
		// Should be a conditional here if both start and end colors are the same or not

		// RGB -> HSV lerp -> RGB
		rColData.bHsv ? LerpHsv() : LerpRgb();
	}

	IntUDist InitDist( const uint16_t a, const uint16_t b )
	{
		return a < b ? rColData.Rand.DistU( a, b ) : rColData.Rand.DistU( b, a );
	}

	void InitColor( pU<grColor::Rgba[]>& rArr, const sizeT arrIdx, const sizeT distIdx )
	{
		rArr[ arrIdx ].R = ( uint8_t )( rColData.Rand.IntU( rColData.puDistArr[ distIdx ] ) );
		rArr[ arrIdx ].G = ( uint8_t )( rColData.Rand.IntU( rColData.puDistArr[ distIdx + 1 ] ) );
		rArr[ arrIdx ].B = ( uint8_t )( rColData.Rand.IntU( rColData.puDistArr[ distIdx + 2 ] ) );
		rArr[ arrIdx ].A = ( uint8_t )( rColData.Rand.IntU( rColData.puDistArr[ distIdx + 3 ] ) );
	}

	void Option0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorEnd, i, 4 );
	}

	void Option1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorStart, i, 0 );

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorEnd[ i ] = rColData.ColorEndMin;
	}

	void Option2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorStart[ i ] = rColData.ColorStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			InitColor( rArrData.ColorEnd, i, 4 );
	}

	void Option3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorStart[ i ] = rColData.ColorStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ColorEnd[ i ] = rColData.ColorEndMin;
	}

	void LerpHsv()
	{
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
	typedef void( grSScaleSystem::*ScaleOption )( const sizeT startIdx, const sizeT endIdx );

	grSEmitData& rEmiData;
	grSScaleData& rScaData;
	grSArrayData& rArrData;

	ScaleOption Option;

	grSScaleSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rScaData( *rData.puScale )
		, rArrData( *rData.puArray )
		, Option( &grSScaleSystem::Option0 )
	{}
	grSScaleSystem( const grSScaleSystem& ) = default;
	grSScaleSystem& operator=( const grSScaleSystem& ) = default;

	void Init( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax )
	{		
		rScaData.ScaleStartMin = rStartMin;
		rScaData.ScaleStartMax = rStartMax;
		rScaData.ScaleEndMin = rEndMin;
		rScaData.ScaleEndMax = rEndMax;
		SetStartEnd( rScaData.ScaleStartMin, rScaData.ScaleStartMax, rScaData.ScaleEndMin, rScaData.ScaleEndMax, rScaData.ScaleStartEqual, rScaData.ScaleEndEqual );

		rScaData.puDistArr[ 0 ] = InitDist( rScaData.ScaleStartMin.x, rScaData.ScaleStartMax.x );
		rScaData.puDistArr[ 1 ] = InitDist( rScaData.ScaleStartMin.y, rScaData.ScaleStartMax.y );
		rScaData.puDistArr[ 2 ] = InitDist( rScaData.ScaleEndMin.x, rScaData.ScaleEndMax.x );
		rScaData.puDistArr[ 3 ] = InitDist( rScaData.ScaleEndMin.y, rScaData.ScaleEndMax.y );

		Option = rScaData.ScaleStartEqual == EEqualValue::NO && rScaData.ScaleEndEqual == EEqualValue::NO ?
			&grSScaleSystem::Option0 :
			rScaData.ScaleStartEqual == EEqualValue::NO && rScaData.ScaleEndEqual == EEqualValue::YES ?
			&grSScaleSystem::Option1 :
			rScaData.ScaleStartEqual == EEqualValue::YES && rScaData.ScaleEndEqual == EEqualValue::NO ?
			&grSScaleSystem::Option2 :
			&grSScaleSystem::Option3;
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		( this->*Option )( startIdx, endIdx );
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

	void Option0( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = { rScaData.Rand.Float( rScaData.puDistArr[ 0 ] ), rScaData.Rand.Float( rScaData.puDistArr[ 1 ] ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = { rScaData.Rand.Float( rScaData.puDistArr[ 2 ] ), rScaData.Rand.Float( rScaData.puDistArr[ 3 ] ) };
	}

	void Option1( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = { rScaData.Rand.Float( rScaData.puDistArr[ 0 ] ), rScaData.Rand.Float( rScaData.puDistArr[ 1 ] ) };

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = rScaData.ScaleEndMin;
	}

	void Option2( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = rScaData.ScaleStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = { rScaData.Rand.Float( rScaData.puDistArr[ 2 ] ), rScaData.Rand.Float( rScaData.puDistArr[ 3 ] ) };
	}

	void Option3( const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleStart[ i ] = rScaData.ScaleStartMin;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.ScaleEnd[ i ] = rScaData.ScaleEndMin;
	}
};


struct grSMassSystem : public grSBaseSystem
{
	typedef std::uniform_real_distribution<float> MassDist;

	grSEmitData& rEmiData;
	grSMassData& rMasData;
	grSArrayData& rArrData;

	FloatDist DistMass;

	grSMassSystem( const grSParticleData& rData )
		: rEmiData( *rData.puEmit )
		, rMasData( *rData.puMass )
		, rArrData( *rData.puArray )
	{}
	grSMassSystem( const grSMassSystem& ) = default;
	grSMassSystem& operator=( const grSMassSystem& ) = default;

	void Init( const grV2f& rMinMax )
	{
		rMasData.MassMinMax.x = grMath::Max( rMinMax.x, 1.0f );
		rMasData.MassMinMax.y = grMath::Max( rMinMax.y, 1.0f );
		SetMinMax( rMasData.MassMinMax, rMasData.EqualMass );
		DistMass = rMasData.Rand.DistF( rMasData.MassMinMax.x, rMasData.MassMinMax.y );
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		if ( rMasData.EqualMass == EEqualValue::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rArrData.Mass[ i ] = rMasData.Rand.Float( DistMass );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrData.Mass[ i ] = rMasData.MassMinMax.x;
	}
};


struct grSVelocitySystem : public grSBaseSystem
{
	grSEmitData& rEmiData;
	grSVelocityData& VelData;
	grSArrayData& rArrData;

	FloatDist VelDist;

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
				VelDist = VelData.Rand.DistF( 0.0f, diff + VelData.DegreeMinMax.y );
				d = VelData.Rand.Float( VelDist ) - diff;
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

	FloatDist DistPosX;
	FloatDist DistPosY;

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

		DistPosX = rPosData.Rand.DistF( rPosData.PositionOffsetMin.x, rPosData.PositionOffsetMax.x );
		DistPosY = rPosData.Rand.DistF( rPosData.PositionOffsetMin.y, rPosData.PositionOffsetMax.y );
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
			grV2f v{ rPosData.Rand.Float( DistPosX ), rPosData.Rand.Float( DistPosY ) };
			rArrData.Position[ i ] = v + rEmiData.SystemPosition;
		}
	}

	void PositionNotEqualEllipse()
	{

	}

	void PositionEqualBox()
	{
		grV2f v{ rPosData.PositionOffsetMin + rEmiData.SystemPosition };
		for ( sizeT i = rEmiData.StartIdx; i < rEmiData.EndIdx; ++i )
			rArrData.Position[ i ] = v;
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
		rLifData.LifeMinMax = rMinMax;
		SetMinMax( rLifData.LifeMinMax, rLifData.LifeEqual );
	}

	void Generate()
	{
		sizeT startIdx{ rEmiData.StartIdx }, endIdx{ rEmiData.EndIdx };
		if ( rLifData.LifeEqual == EEqualValue::NO )
		{
			auto dist{ rLifData.Rand.DistF( rLifData.LifeMinMax.x, rLifData.LifeMinMax.y ) };
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rArrdata.Life[ i ] = rLifData.Rand.Float( dist );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rArrdata.Life[ i ] = rLifData.LifeMinMax.x;
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
		puColor = std::make_unique<grSColorSystem>( rData );
		puScale = std::make_unique<grSScaleSystem>( rData );
		puMass = std::make_unique<grSMassSystem>( rData );
		puVelocity = std::make_unique<grSVelocitySystem>( rData );
		puPosition = std::make_unique<grSPositionSystem>( rData );
		puLife = std::make_unique<grSLifeSystem>( rData );
	}
};

#endif // _H_GRPARTICLESYSTEM_