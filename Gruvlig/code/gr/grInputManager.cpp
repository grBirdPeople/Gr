#include	"grInputManager.h"

#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grInputManager::grInputManager( void )
	: m_MousePos		( grV2f() )
	, m_MouseScroll	( 0 )
	, m_bMouseMoved	( false )
{
	m_KeyCount = sf::Keyboard::KeyCount;
	m_MouseCount = sf::Mouse::ButtonCount;

	m_Key.reserve( m_KeyCount );
	m_KeyDown.reserve( m_KeyCount );
	m_MouseButton.reserve( m_MouseCount );
	m_MouseButtonDown.reserve( m_MouseCount );

	for ( uInt i = 0; i < m_KeyCount; i++ )
	{
		m_Key.push_back( false );
	}

	for (uInt i = 0; i < m_KeyCount; i++)
	{
		m_KeyDown.push_back(false);
	}

	for ( uInt i = 0; i < m_MouseCount; i++ )
	{
		m_MouseButton.push_back( false );
	}

	for (uInt i = 0; i < m_MouseCount; i++)
	{
		m_MouseButtonDown.push_back(false);
	}
}


// dTor
//////////////////////////////////////////////////
grInputManager::~grInputManager(void)
{
	ResetKeys();
	ResetMouse();
}


// ResetKeys
//////////////////////////////////////////////////
inline void
grInputManager::ResetKeys( void )
{
	for ( uInt i = 0; i < m_KeyCount; i++ )
	{
		m_Key.push_back( false );
	}

	for ( uInt i = 0; i < m_KeyCount; i++ )
	{
		m_KeyDown.push_back( false );
	}
}


// ResetMouse
//////////////////////////////////////////////////
inline void
grInputManager::ResetMouse( void )
{
	for ( uInt i = 0; i < m_MouseCount; i++ )
	{
		m_MouseButton.push_back( false );
	}

	for ( uInt i = 0; i < m_MouseCount; i++ )
	{
		m_MouseButtonDown.push_back( false );
	}

	m_MouseScroll = 0;
	m_bMouseMoved = false;
}
