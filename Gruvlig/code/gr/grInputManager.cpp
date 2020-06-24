#include	"grInputManager.h"

#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grInputManager::grInputManager( void )
	: m_MousePos	( grV2f() )
	, m_MouseScroll	( 0 )
	, m_bMouseMoved	( false )
{
	m_KeyCount = sf::Keyboard::KeyCount;
	m_MouseCount = sf::Mouse::ButtonCount;

	m_puKey.reset( new bool[ m_KeyCount ] );
	m_puKeyDown.reset( new bool[ m_KeyCount ] );
	m_puMouseButton.reset( new bool[ m_MouseCount ] );
	m_puMouseButtonDown.reset( new bool[ m_MouseCount ] );

	ResetKeys();
	ResetMouse();
}


// dTor
//////////////////////////////////////////////////
grInputManager::~grInputManager( void )
{
	ResetKeys();
	ResetMouse();
}


// ResetKeys
//////////////////////////////////////////////////
inline void
grInputManager::ResetKeys( void )
{
	for ( sizeT i = 0; i < m_KeyCount; ++i )
		m_puKey[ i ] = false;

	for ( sizeT i = 0; i < m_KeyCount; ++i )
		m_puKeyDown[ i ] = false;
}


// ResetMouse
//////////////////////////////////////////////////
inline void
grInputManager::ResetMouse( void )
{
	for ( sizeT i = 0; i < m_MouseCount; ++i )
		m_puMouseButton[ i ] = false;

	for ( sizeT i = 0; i < m_MouseCount; ++i )
		m_puMouseButtonDown[ i ] = false;

	m_MouseScroll = 0;
	m_bMouseMoved = false;
}
