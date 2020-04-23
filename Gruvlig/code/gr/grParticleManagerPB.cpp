#include    "grParticleManagerPB.h"

#include    "grCore.h"


// cTor
//////////////////////////////////////////////////
grCParticleManagerPB::grCParticleManagerPB( void )
    : m_CreatedSystems      ( 0 )
    , m_CreatedEmitters     ( 0 )
    , m_TotalParticles      ( 0 )
{
    // I forget syntax for dynamic array of uPtr's so example
    {
        // uPtr<grParticlePB*[]>	m_upArrParticle;
        // uPtr<uPtr<grParticlePB>[]>	m_upupArrParticle;

        // m_upArrParticle       ( new grParticlePB*[ PARTICLE_PER_ATTRIBUTE ]() )
        // m_upupArrParticle     ( new uPtr<grParticlePB>[ PARTICLE_PER_ATTRIBUTE ]() )
    }

    m_VecUpEmitter.reserve( PARTICLE_EMITTERS );

    for ( sizeT i = 0; i < PARTICLE_SYTEMS; ++i )
    {
        m_VecUpSystem.push_back( std::make_unique<grSParticleSystemPB>( ( uInt )i, PARTICLE_PER_EMITTER ) );
        ++m_CreatedSystems;
    }
}


// dTor
//////////////////////////////////////////////////
grCParticleManagerPB::~grCParticleManagerPB( void )
{
    for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
    {
        if ( m_VecUpEmitter[ i ] != nullptr )
            delete m_VecUpEmitter[ i ].release();
    }
    m_VecUpEmitter.clear();

    for ( sizeT i = 0; i < m_CreatedSystems; ++i )
    {
        if ( m_VecUpSystem[ i ] != nullptr )
            delete m_VecUpSystem[ i ].release();
    }
    m_VecUpSystem.clear();
}


grCParticleEmitterPB* const
grCParticleManagerPB::CreateEmitter( void )
{
    assert( m_CreatedEmitters <= PARTICLE_EMITTERS && "grCParticleManagerPB::Create(): Max emitters already created" );

    uInt id = m_CreatedEmitters;
    ++m_CreatedEmitters;
    m_TotalParticles += PARTICLE_PER_EMITTER;

    m_VecUpEmitter.push_back( std::move( std::make_unique<grCParticleEmitterPB>( id, PARTICLE_PER_EMITTER ) ) );

    return m_VecUpEmitter[ m_CreatedEmitters - 1 ].get();
}


// GetEmitter
//////////////////////////////////////////////////
grCParticleEmitterPB* const
grCParticleManagerPB::GetEmitter( const uInt id )
{
    assert( m_CreatedEmitters <= PARTICLE_EMITTERS && "grCParticleManagerPB::GetEmitter(): Emittor id out of range" );
    return m_VecUpEmitter[ id ].get();
}


// Update
//////////////////////////////////////////////////
void
grCParticleManagerPB::Update( const float deltaT )
{
    // TEST

    // TODO: Test which loop-set is the fastest.
    // Partially possible that it comes down to SIMD vs. Cache cycles, or I know nothing is also a posibility.
    // Cpu on used machine (Ryzen 5 2600x) has 576kb L1. One instance of an emitter (holds the emitter particles and some other minor things) is semi-roughly 8kb (double check).
    // All emitters are allocated linearly and it's contents the same, where all member data declerations are done RowUpDown/SizeMaxMin
    // Additionally there is a light weight system that modifies the data contained in the emitter, which is allocated directly after all the emitters.
    // Don't remeber the size of the system so look it up...

    for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
    {
        m_VecUpSystem[ 0 ]->Activate( *m_VecUpEmitter[ i ].get(), deltaT );
        m_VecUpSystem[ 0 ]->Update( *m_VecUpEmitter[ i ].get(), deltaT );
        m_VecUpSystem[ 0 ]->Deactivate( *m_VecUpEmitter[ i ].get() );
    }

    //for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
    //    m_VecUpSystem[ 0 ]->Activate( *m_VecUpEmitter[ i ].get(), deltaT );

    //for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
    //    m_VecUpSystem[ 0 ]->Update( *m_VecUpEmitter[ i ].get(), deltaT );

    //for ( sizeT i = 0; i < m_CreatedEmitters; ++i )
    //    m_VecUpSystem[ 0 ]->Deactivate( *m_VecUpEmitter[ i ].get() );
}