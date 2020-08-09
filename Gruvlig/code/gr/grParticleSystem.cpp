#include	"grParticleSystem.h"

#include	"grParticleEmitter.h"
#include	"grSPartAttribute.h"
#include	"grParticle.h"
#include	"grMath.h"
#include	"grRandom.h"


grCParticleSystem::grCParticleSystem( const intU id, const sizeT partMax )
	: m_uPRand	( new grRandom() )
	, m_Id		( id )
	, m_PartMax	( partMax )
{}


grCParticleSystem::~grCParticleSystem( void )
{
	if ( m_uPRand != nullptr )
		m_uPRand.reset();
}


void
grCParticleSystem::Copy( vE<intU>& rVeActvEmitr,
						 pU<pU<grCParticleEmitter>[]>& rArEmitr,
						 pU<pU<grSParticleAttribute>[]>& rArAtt )
{
	if ( rVeActvEmitr.size() > 0 )
	{
		sizeT actvEmitrSize = rVeActvEmitr.size();
		for ( sizeT i = 0; i < actvEmitrSize; ++i )
		{
			if ( rArEmitr[ i ]->m_UsrMods.any() == true )
			{
				vE<sizeT> usrMods;
				usrMods.reserve( ( sizeT )grCParticleEmitter::EUsrMods::SIZE );
				float x = 0.0f, y = 0.0f;
				intU id = rVeActvEmitr[ i ];
				grSParticleAttribute& rEmitrAtt = *rArEmitr[ id ]->m_uPAtt.get();
				grSParticleAttribute& rSysAtt = *rArAtt[ id ].get();
				usrMods = rArEmitr[ id ]->GetUsrModsSortd();
				for ( sizeT i = 0; i < usrMods.size(); ++i )
				{
					switch ( usrMods[ i ] )
					{
						case ( sizeT )grCParticleEmitter::EUsrMods::POS:
						rSysAtt.EmitrPos = rEmitrAtt.EmitrPos;
						rSysAtt.PartRadiusPosOffset = grMath::Abs( rEmitrAtt.PartRadiusPosOffset );
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::DIR_EMITR:
						rEmitrAtt.EmitrRotInDeg =
							( rEmitrAtt.EmitrRotInDeg > 360.0f ) ? rEmitrAtt.EmitrRotInDeg - 360.0f
							: ( rEmitrAtt.EmitrRotInDeg < 0.0f ) ? 360.0f + rEmitrAtt.EmitrRotInDeg
							: rEmitrAtt.EmitrRotInDeg;

						rEmitrAtt.EmitrDir.Set( 0.0f, -1.0f );
						grMath::RotatePoint( &rEmitrAtt.EmitrDir, rEmitrAtt.EmitrRotInDeg * grMath::Deg2RadConv );

						rSysAtt.EmitrDir = rEmitrAtt.EmitrDir;
						rSysAtt.EmitrRotInDeg = rEmitrAtt.EmitrRotInDeg;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::DIR_PART:
						rEmitrAtt.PartRotInDegMinMax.x =
							( rEmitrAtt.PartRotInDegMinMax.x > 360.0f ) ? rEmitrAtt.PartRotInDegMinMax.x - 360.0f
							: ( rEmitrAtt.PartRotInDegMinMax.x < 0.0f ) ? 360.0f + rEmitrAtt.PartRotInDegMinMax.x
							: rEmitrAtt.PartRotInDegMinMax.x;

						rEmitrAtt.PartRotInDegMinMax.y =
							( rEmitrAtt.PartRotInDegMinMax.y > 360.0f ) ? rEmitrAtt.PartRotInDegMinMax.y - 360.0f
							: ( rEmitrAtt.PartRotInDegMinMax.y < 0.0f ) ? 360.0f + rEmitrAtt.PartRotInDegMinMax.y
							: rEmitrAtt.PartRotInDegMinMax.y;

						rSysAtt.PartRotInDegMinMax = rEmitrAtt.PartRotInDegMinMax;
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::SPD:
						rSysAtt.PartSpdMinMax.x = grMath::Clamp( rEmitrAtt.PartSpdMinMax.x, rEmitrAtt.PartSpdMinMax.x, rEmitrAtt.PartSpdMinMax.y );
						rSysAtt.PartSpdMinMax.y = grMath::Clamp( rEmitrAtt.PartSpdMinMax.y, rEmitrAtt.PartSpdMinMax.x, rEmitrAtt.PartSpdMinMax.y );
						if ( rEmitrAtt.PartSpdModMinMax.x != 0.0f || rEmitrAtt.PartSpdModMinMax.y != 0.0f )
						{
							rSysAtt.PartSpdModMinMax.x = grMath::Clamp( rEmitrAtt.PartSpdModMinMax.x, rEmitrAtt.PartSpdModMinMax.x, rEmitrAtt.PartSpdModMinMax.y );
							rSysAtt.PartSpdModMinMax.y = grMath::Clamp( rEmitrAtt.PartSpdModMinMax.y, rEmitrAtt.PartSpdModMinMax.x, rEmitrAtt.PartSpdModMinMax.y );
						}
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::LIFE:
						rSysAtt.PartLifeMinMax.x = grMath::Clamp( rEmitrAtt.PartLifeMinMax.x, 0.1f, rEmitrAtt.PartLifeMinMax.x );
						rSysAtt.PartLifeMinMax.y = grMath::Clamp( rEmitrAtt.PartLifeMinMax.y, rEmitrAtt.PartLifeMinMax.x, rEmitrAtt.PartLifeMinMax.y );
						break;

						case ( sizeT )grCParticleEmitter::EUsrMods::COL:
						rSysAtt.PartColMinMax.From.R = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.From.R, 0, 255 );
						rSysAtt.PartColMinMax.From.G = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.From.G, 0, 255 );
						rSysAtt.PartColMinMax.From.B = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.From.B, 0, 255 );
						rSysAtt.PartColMinMax.From.A = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.From.A, 0, 255 );

						rSysAtt.PartColMinMax.Too.R = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.Too.R, 0, 255 );
						rSysAtt.PartColMinMax.Too.G = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.Too.G, 0, 255 );
						rSysAtt.PartColMinMax.Too.B = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.Too.B, 0, 255 );
						rSysAtt.PartColMinMax.Too.A = grMath::Clamp( ( intU )rSysAtt.PartColMinMax.Too.A, 0, 255 );
						break;

						default:
						break;
					}
				}
			}
		}
	}
}


void
grCParticleSystem::QueSpawn( pU<pU<grCParticleEmitter>[]>& rArEmitr,
									  vE<intU>& rVeActiveEmitr,
									  vE<intU>& rVeActivateQue,
									  const float deltaT )
{
	if ( rVeActiveEmitr.size() > 0 )
	{
		sizeT actvEmitrSize = rVeActiveEmitr.size();
		for ( sizeT i = 0; i < actvEmitrSize; ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeActiveEmitr[ i ] ].get();
			rEmitr.m_SpawnTimerMs -= deltaT;
			while ( rEmitr.m_SpawnTimerMs < 0.0f )
			{
				if ( rEmitr.m_PartActive == m_PartMax )
				{
					rEmitr.m_SpawnTimerMs = 0.0f;
					return;
				}

				rEmitr.m_SpawnTimerMs += rEmitr.m_SpawnRateMs;
				rVeActivateQue.push_back( rEmitr.m_Id );
			}
		}
	}
}


void
grCParticleSystem::Activate( vE<intU>& rVeActivateQue,
							 pU<pU<grCParticleEmitter>[]>& rArEmitr,
							 pU<pU<grSParticleAttribute>[]>& rArAtt,
							 grSParticleArr** pAr2DPart )
{
	if ( rVeActivateQue.size() > 0 )
	{
		sizeT queSize = rVeActivateQue.size();
		for ( sizeT i = 0; i < queSize; ++i )
		{
			grCParticleEmitter& rEmitr = *rArEmitr[ rVeActivateQue[ i ] ].get();
			grSParticleAttribute& rAtt = *rArAtt[ rEmitr.m_Id ].get();
			grSParticleArr& rPart = pAr2DPart[ rEmitr.m_Id ][ rEmitr.m_PartActive ];

			++rEmitr.m_PartActive;

			ActvPosition( rAtt, rPart );
			ActvVelocity( rAtt, rPart );
			ActvLife( rAtt, rPart );
			ActvColor( rAtt, rPart );
		}

		rVeActivateQue.clear();
	}

	//printf( "%s \t %d \n", "Particles Active", rArEmitr[ 0 ]->m_PartActive );
}


void
grCParticleSystem::Update( vE<std::pair<intU, intU>>& rVeDeactivateQue,
						   vE<intU>& rVeActiveEmitr,
						   pU<pU<grCParticleEmitter>[]>& rArEmitr,
						   pU<pU<grSParticleAttribute>[]>& rArAtt,
						   grSParticleArr** pAr2DPart,
						   const float deltaT )
{
	sizeT actvEmitrSize = rVeActiveEmitr.size();
	for ( sizeT emitrIdx = 0; emitrIdx < actvEmitrSize; ++emitrIdx )
	{
		sizeT emitrId = rVeActiveEmitr[ emitrIdx ];
		sizeT partSize = rArEmitr[ emitrId ]->m_PartActive;
		grSParticleAttribute& rAtt = *rArAtt[ emitrIdx ].get();
		for ( sizeT partIdx = 0; partIdx < partSize; ++partIdx )
		{
			grSParticleArr& rPart = pAr2DPart[ emitrId ][ partIdx ];

			// Speed
			if( rPart.bSpdOsc == true )
			{
				if ( rPart.SpdMod > 0.0f && rAtt.PartSpdMinMax.y < rPart.Spd )
					rPart.SpdMod *= -1.0f;
				else if ( rPart.SpdMod < 0.0f && rAtt.PartSpdMinMax.x > rPart.Spd )
					rPart.SpdMod *= -1.0f;

			}
			rPart.Spd = grMath::Lerp( rPart.Spd, rPart.Spd + rPart.SpdMod, 0.25f );

			// Accel
			grV2f accel = rPart.Dir * rPart.Spd;

			// Position
			rPart.Pos += ( accel ) * deltaT;

			// Lifetime
			rPart.Life -= deltaT;
			if ( rPart.Life < 0.0f )
				rVeDeactivateQue.push_back( pR<intU, intU>( emitrId, partIdx ) );

			// Color
				//a.r + ( b.r - a.r ) * t,
				//a.g + ( b.g - a.g ) * t,
				//a.b + ( b.b - a.b ) * t,
				//a.a + ( b.a - a.a ) * t

			//float target = ( float )rAtt.PartColMinMax.Too.G;
			//float now = ( float )rPart.Color.g;
			//float result = ( target - now ) * ( rPart.ColRate.G * deltaT ) + target;

			//rPart.Color.g = result;

			//printf( "%g \n", result );

			//rPart.ColAccumulator.R += rPart.ColRate.R * deltaT;
			//rPart.ColAccumulator.G += rPart.ColRate.G * deltaT;
			//rPart.ColAccumulator.B += rPart.ColRate.B * deltaT;
			//rPart.ColAccumulator.A += rPart.ColRate.A * deltaT;

			//if ( rPart.ColAccumulator.R > 1.0f || rPart.ColAccumulator.R < -1.0f )
			//{
			//	rPart.Color.r += ( rPart.ColAccumulator.R > 1.0f ) ? 1 : -1;
			//	rPart.ColAccumulator.R = 0.0f;
			//}
			//if ( rPart.ColAccumulator.G > 1.0f || rPart.ColAccumulator.G < -1.0f )
			//{
			//	rPart.Color.g += ( rPart.ColAccumulator.G > 1.0f ) ? 1 : -1;
			//	rPart.ColAccumulator.G = 0.0f;
			//}
			//if ( rPart.ColAccumulator.B > 1.0f || rPart.ColAccumulator.B < -1.0f )
			//{
			//	rPart.Color.b += ( rPart.ColAccumulator.B > 1.0f ) ? 1 : -1;
			//	rPart.ColAccumulator.B = 0.0f;
			//}
			//if ( rPart.ColAccumulator.A > 1.0f || rPart.ColAccumulator.A < -1.0f )
			//{
			//	rPart.Color.a += ( rPart.ColAccumulator.A > 1.0f ) ? 1 : -1;
			//	rPart.ColAccumulator.A = 0.0f;
			//}

			//rPart.Color.r += ( sf::Uint8 )rPart.ColMod.R * deltaT;
			//rPart.Color.g += ( sf::Uint8 )rPart.ColMod.G * deltaT;
			//rPart.Color.b += ( sf::Uint8 )rPart.ColMod.B * deltaT;
			//rPart.Color.a += ( sf::Uint8 )rPart.ColMod.A * deltaT;
		}
	}
}


void grCParticleSystem::Deactivate( vE<std::pair<intU, intU>>& rVeDeactivateQue,
									pU<pU<grCParticleEmitter>[]>& rArEmitr,
									grSParticleArr** pAr2DPart )
{
	if ( rVeDeactivateQue.size() > 0 )
	{
		sizeT queSize = rVeDeactivateQue.size();
		for ( sizeT idx = 0; idx < queSize; ++idx )
		{
			intU emitrId = rVeDeactivateQue[ idx ].first;
			intU partIdx = rVeDeactivateQue[ idx ].second;
			sizeT partsActive = rArEmitr[ emitrId ]->m_PartActive;

			--rArEmitr[ emitrId ]->m_PartActive;

			grSParticleArr& rTooPart = pAr2DPart[ emitrId ][ partIdx ];
			grSParticleArr& rFromPart = pAr2DPart[ emitrId ][ partsActive - 1 ];
			rTooPart = rFromPart;
		}

		rVeDeactivateQue.clear();
	}
}


void
grCParticleSystem::ActvPosition( grSParticleAttribute& rAtt, grSParticleArr& rPart )
{
	rPart.Pos = rAtt.EmitrPos;
	if ( grMath::CmpFloat( rAtt.PartRadiusPosOffset, 0.0f ) != true )
	{
		grV2f dir = grV2f( m_uPRand->Float( -1.0f, 1.0f ), m_uPRand->Float( -1.0f, 1.0f ) );
		float rad = m_uPRand->Float( 0.0f, 359.9f ) * grMath::Deg2RadConv;
		float dist = m_uPRand->Float( 0.0f, rAtt.PartRadiusPosOffset );

		grMath::RotatePoint( &dir, rad );
		rPart.Pos += dir * dist;
	}
}


void
grCParticleSystem::ActvVelocity( grSParticleAttribute& rAtt, grSParticleArr& rPart )
{
	// Direction
	float deg = 0.0f;
	if ( grMath::CmpFloat( rAtt.PartRotInDegMinMax.x, rAtt.PartRotInDegMinMax.y ) != true )
	{
		if ( rAtt.PartRotInDegMinMax.x > rAtt.PartRotInDegMinMax.y )	
		{
			float diff = ( 360.0f - rAtt.PartRotInDegMinMax.x );
			deg = m_uPRand->Float( 0.0f, diff + rAtt.PartRotInDegMinMax.y );
			deg -= diff;
			if ( 0.0f > deg )
				deg = 360.0f + deg;
		}
		else
		{
			deg = m_uPRand->Float( rAtt.PartRotInDegMinMax.x, rAtt.PartRotInDegMinMax.y );
		}
	}
	else
	{
		deg = rAtt.PartRotInDegMinMax.x;
	}

	rPart.Dir = rAtt.EmitrDir;
	grMath::RotatePoint( &rPart.Dir, deg * grMath::Deg2RadConv );

	// Speed
	if ( rAtt.PartSpdMinMax.x > rAtt.PartSpdMinMax.y )
		rAtt.PartSpdMinMax.x = rAtt.PartSpdMinMax.y;

	rAtt.PartSpdMinMax.x = grMath::Clamp( rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.y );

	rPart.Spd = ( grMath::CmpFloat( rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.y ) != true )
		? m_uPRand->Float( rAtt.PartSpdMinMax.x, rAtt.PartSpdMinMax.y )
		: rAtt.PartSpdMinMax.x;

	if ( rAtt.PartSpdModMinMax.x != rAtt.PartSpdModMinMax.y )
	{
		rPart.SpdMod = m_uPRand->Float( rAtt.PartSpdModMinMax.x, rAtt.PartSpdModMinMax.y );
		rPart.bSpdOsc = true;
	}
	else
	{
		rPart.SpdMod = rAtt.PartSpdModMinMax.x;
		rPart.bSpdOsc = false;
	}

	
	// Weight
	// This combined with scale could work as weight
	//float weight = m_uPRand->Float( 1.0f, 1.5f );
	//rPart.Spd /= weight;



	//grV2f dir = grV2f( 0.0f, -1.0f );	
	//grMath::RotatePoint( &dir, deg * grMath::DegToRad );

	//float speed = ( grMath::CmpFloat( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y ) != true )
	//	? m_uPRand->Float( rAtt.MinMaxSpeed.x, rAtt.MinMaxSpeed.y )
	//	: rAtt.MinMaxSpeed.x;

	//rPart.Velocity = dir * speed;
}


void
grCParticleSystem::ActvLife( grSParticleAttribute& rAtt, grSParticleArr& rPart )
{
	rPart.Life = ( grMath::CmpFloat( rAtt.PartLifeMinMax.x, rAtt.PartLifeMinMax.y ) != true )
		? m_uPRand->Float( rAtt.PartLifeMinMax.x, rAtt.PartLifeMinMax.y )
		: rAtt.PartLifeMinMax.x;
}


void
grCParticleSystem::ActvColor( grSParticleAttribute& rAtt, grSParticleArr& rPart )
{
	rPart.Color.r = rAtt.PartColMinMax.From.R;
	rPart.Color.g = rAtt.PartColMinMax.From.G;
	rPart.Color.b = rAtt.PartColMinMax.From.B;
	rPart.Color.a = rAtt.PartColMinMax.From.A;

	if ( rAtt.PartColMinMax.From != rAtt.PartColMinMax.Too )
	{
		float r = ( float )rAtt.PartColMinMax.Too.R - ( float )rAtt.PartColMinMax.From.R;
		float g = ( float )rAtt.PartColMinMax.Too.G - ( float )rAtt.PartColMinMax.From.G;
		float b = ( float )rAtt.PartColMinMax.Too.B - ( float )rAtt.PartColMinMax.From.B;
		float a = ( float )rAtt.PartColMinMax.Too.A - ( float )rAtt.PartColMinMax.From.A;

		float t = rPart.Life * 60.0f;
		rPart.ColRate.R = !grMath::CmpFloat( r, 0.0f ) ? r / t : 0.0f;
		rPart.ColRate.G = !grMath::CmpFloat( g, 0.0f ) ? g / t : 0.0f;
		rPart.ColRate.B = !grMath::CmpFloat( b, 0.0f ) ? b / t : 0.0f;
		rPart.ColRate.A = !grMath::CmpFloat( a, 0.0f ) ? a / t : 0.0f;
	}
}
