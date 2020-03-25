#include	"grInput.h"

#include	"grDefine.h"


// cTor
//////////////////////////////////////////////////
grInput::grInput( void )
	: m_MouseScroll	( 0 )
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
grInput::~grInput(void)
{
	ResetKeys();
	ResetMouse();
}


// ResetKeys
//////////////////////////////////////////////////
inline void
grInput::ResetKeys( void )
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
grInput::ResetMouse( void )
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
