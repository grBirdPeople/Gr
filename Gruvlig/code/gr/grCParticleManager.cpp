#include	"grCParticleManager.h"

#include	"grParticleSystem.h"
#include	"grSPartAttribute.h"
#include	"grParticle.h"
#include	"grAlgo.h"

// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW


grCParticleManager::grCParticleManager( void )
	: m_aREmitr			( new pU<grCParticleEmitter>[ PART_EMITR_MAX ] )
	, m_aRAtt			( new pU<grSParticleAttribute>[ PART_EMITR_MAX ] )
	, m_aR2DPart		( new grSParticleArr*[ PART_EMITR_MAX ] )

	, m_uPSys			( new grCParticleSystem( 0, PART_PART_MAX ) )

	, m_CreateCount		( 0 )
	, m_cPartPerEmitr	( PART_PART_MAX )
{
	m_vEActvEmitr.reserve( PART_EMITR_MAX );
	m_vEActvQue.reserve( PART_EMITR_MAX );
	m_vEDeactvQue.reserve( PART_EMITR_MAX );
}


grCParticleManager::~grCParticleManager( void )
{
	for ( sizeT i = 0; i < PART_EMITR_MAX; ++i )
	{
		if( m_aREmitr[ i ].get() != nullptr )
			m_aREmitr[ i ].reset();
	}
	delete[] m_aREmitr.release();

	for ( sizeT i = 0; i < PART_EMITR_MAX; ++i )
	{
		if ( m_aRAtt[ i ].get() != nullptr )
			m_aRAtt[ i ].reset();
	}
	delete[] m_aRAtt.release();

	for ( sizeT i = 0; i < PART_EMITR_MAX; ++i )
	{
		if ( m_aR2DPart[ i ] != nullptr )
			delete[] m_aR2DPart[ i ];
	}
	delete[] m_aR2DPart;

	delete m_uPSys.release();
}


grCParticleEmitter&
grCParticleManager::Create( void )
{
	m_vEActvEmitr.push_back( m_CreateCount );

	m_aREmitr[ m_CreateCount ] = std::make_unique<grCParticleEmitter>( m_CreateCount );
	m_aRAtt[ m_CreateCount ] = std::make_unique<grSParticleAttribute>();
	m_aR2DPart[ m_CreateCount ] = new grSParticleArr[ m_cPartPerEmitr ];

	++m_CreateCount;

	return *m_aREmitr[ m_CreateCount - 1 ].get();
}


void
grCParticleManager::Update( const float deltaT )
{
	m_uPSys->Copy( m_vEActvEmitr, m_aREmitr, m_aRAtt );
	m_uPSys->QueSpawn( m_aREmitr, m_vEActvEmitr, m_vEActvQue, deltaT );
	m_uPSys->Activate( m_vEActvQue, m_aREmitr, m_aRAtt, m_aR2DPart );
	m_uPSys->Update( m_vEDeactvQue, m_vEActvEmitr, m_aREmitr, m_aRAtt, m_aR2DPart, deltaT );
	m_uPSys->Deactivate( m_vEDeactvQue, m_aREmitr, m_aR2DPart );

	// TEST DRAW
	for ( sizeT id = 0; id < m_vEActvEmitr.size(); ++id )
	{
		intU active = m_aREmitr[ id ]->m_PartActive;
		for ( sizeT part = 0; part < active; ++part )
		{
			grBBox box( grV2f( 5.5f, 5.5f ), m_aR2DPart[ m_vEActvEmitr[ id ] ][ part ].Pos );
			grDebugManager::Instance().AddBBox( box, m_aR2DPart[ m_vEActvEmitr[ id ] ][ part ].Color );
		}
	}
	// TEST DRAW
}
