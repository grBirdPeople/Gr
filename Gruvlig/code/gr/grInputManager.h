#ifndef		_GRINPUT_H_
#define		_GRINPUT_H_

#include	<SFML/Window/Keyboard.hpp>	
#include	<SFML/Window/Mouse.hpp>	

#include	"grCommon.h"
#include	"grSingleton.h"
#include	"grV2.h"


// grInput
//////////////////////////////////////////////////
class grInputManager : public grSingleton<grInputManager>
{
public:
	grInputManager( void );
	~grInputManager( void );

	// TODO: To avoid missuse, make some functions only accessible from friend classes (grCore), or similar

	inline const bool GetKey( sf::Keyboard::Key key ) const { return m_puKey[ key ]; }
	inline const bool GetKeyDown( sf::Keyboard::Key key )
	{
		sizeT idx = ( sizeT )key;
		if ( sf::Keyboard::isKeyPressed( key ) == true && m_puKeyDown[ idx ] == false  )
		{
			m_puKeyDown[ idx ] = true;
			return true;
		}
		return false;
	}
	inline void	SetKeyDown( sf::Keyboard::Key key ) { m_puKey[ key ] = true; }
	inline void	SetKeyUp( sf::Keyboard::Key key ) { m_puKey[ key ] = false; }
	inline const bool GetMouse( sf::Mouse::Button button ) const { return m_puMouseButton[ button ]; }
	inline const bool GetMouseDown( sf::Mouse::Button button )
	{
		sizeT idx = ( sizeT )button;
		if ( sf::Mouse::isButtonPressed( button ) == true && m_puMouseButtonDown[ idx ] == false )
		{
			m_puMouseButtonDown[ idx ] = true;
			return true;
		}
		return false;
	}
	inline void SetMouseDown( sf::Mouse::Button button )			{ m_puMouseButton[ button ] = true; }
	inline void SetMouseUp( sf::Mouse::Button button )
	{
		m_puMouseButton[ button ] = false;
		m_puMouseButtonDown[ ( sizeT )button ] = false;
	}
	inline intS GetMouseScroll( void ) const { return m_MouseScroll; }
	inline bool GetMouseScrollForwards( void ) const { return ( m_MouseScroll > 0 ) ? true : false; }
	inline bool GetMouseScrollBackwards( void ) const { return ( m_MouseScroll < 0 ) ? true : false; }
	inline void SetMouseScroll( const intS scrollDir ) { m_MouseScroll = scrollDir; }
	inline bool GetMouseMoved( void ) const { return m_bMouseMoved; }
	inline grV2f& GetMousePos( void ) { return m_MousePos; }
	inline void SetMouseMoved( const grV2f& rPos )
	{
		m_MousePos = rPos;
		m_bMouseMoved = true;
	}

	//////////////////////////////////////////////////

	inline void Update( void )
	{
		if( m_MouseScroll != 0 )
		{
			m_MouseScroll = 0;
		}

		if( m_bMouseMoved != false )
		{
			m_bMouseMoved = false;
		}
	}
	inline void ResetKeys( void );
	inline void ResetMouse( void );

private:
	pU<bool[]>	m_puKey;
	pU<bool[]>	m_puKeyDown;
	pU<bool[]>	m_puMouseButton;
	pU<bool[]>	m_puMouseButtonDown;

	grV2f	m_MousePos;

	intS	m_MouseScroll;

	sizeT	m_KeyCount,
			m_MouseCount;

	bool	m_bMouseMoved;

};

#endif		//_GRINPUT_H_