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
	friend class grCore;

	grInputManager()
		: m_puKey( std::make_unique<bool[]>( sf::Keyboard::KeyCount ) )
		, m_puKeyDown( std::make_unique<bool[]>( sf::Keyboard::KeyCount ) )
		, m_puMouseButton( std::make_unique<bool[]>( sf::Mouse::ButtonCount ) )
		, m_puMouseButtonDown( std::make_unique<bool[]>( sf::Mouse::ButtonCount ) )
		, m_MousePos( { 0.0f, 0.0f } )
		, m_MouseScroll( 0 )
		, m_KeyCount( sf::Keyboard::KeyCount )
		, m_MouseCount( sf::Mouse::ButtonCount )
		, m_bMouseMoved( false )
	{
		ResetKeys();
		ResetMouse();
	}

	inline const bool GetKey( const sf::Keyboard::Key key ) const
	{
		return m_puKey[ key ];
	}

	inline const bool GetKeyDown( const sf::Keyboard::Key key )
	{
		if ( sf::Keyboard::isKeyPressed( key ) == true && m_puKeyDown[ key ] == false  )
		{
			m_puKeyDown[ key ] = true;
			return true;
		}
		return false;
	}

	inline const bool GetMouse( const sf::Mouse::Button button ) const
	{
		return m_puMouseButton[ button ];
	}

	inline const bool GetMouseDown( const sf::Mouse::Button button )
	{
		if ( sf::Mouse::isButtonPressed( button ) == true && m_puMouseButtonDown[ button ] == false )
		{
			m_puMouseButtonDown[ button ] = true;
			return true;
		}
		return false;
	}

	inline intS GetMouseScroll() const
	{
		return m_MouseScroll;
	}

	inline bool GetMouseScrollForwards() const
	{
		return ( m_MouseScroll > 0 ) ? true : false;
	}

	inline bool GetMouseScrollBackwards() const
	{
		return ( m_MouseScroll < 0 ) ? true : false;
	}

	inline bool GetMouseMoved() const
	{
		return m_bMouseMoved;
	}

	inline grV2f& GetMousePos()
	{
		return m_MousePos;
	}

	inline void ResetKeys()
	{
		for ( sizeT i = 0; i < m_KeyCount; ++i )
			m_puKey[ i ] = false;

		for ( sizeT i = 0; i < m_KeyCount; ++i )
			m_puKeyDown[ i ] = false;
	}

	inline void ResetMouse()
	{
		for ( sizeT i = 0; i < m_MouseCount; ++i )
			m_puMouseButton[ i ] = false;

		for ( sizeT i = 0; i < m_MouseCount; ++i )
			m_puMouseButtonDown[ i ] = false;

		m_MouseScroll = 0;
		m_bMouseMoved = false;
	}

private:
	inline void	SetKeyDown( const sf::Keyboard::Key key )
	{
		m_puKey[ key ] = true;
	}

	inline void	SetKeyUp( const sf::Keyboard::Key key )
	{
		m_puKey[ key ] = false;
		m_puKeyDown[ key ] = false;
	}

	inline void SetMouseDown( const sf::Mouse::Button button )
	{
		m_puMouseButton[ button ] = true;
	}

	inline void SetMouseUp( const sf::Mouse::Button button )
	{
		m_puMouseButton[ button ] = false;
		m_puMouseButtonDown[ button ] = false;
	}

	inline void SetMouseScroll( const intS scrollDir )
	{
		m_MouseScroll = scrollDir;
	}

	inline void SetMouseMoved( const grV2f& rPos )
	{
		m_MousePos = rPos;
		m_bMouseMoved = true;
	}

	inline void Update()
	{
		if ( m_MouseScroll != 0 )
			m_MouseScroll = 0;

		if ( m_bMouseMoved != false )
			m_bMouseMoved = false;
	}

	pU<bool[]>	m_puKey;
	pU<bool[]>	m_puKeyDown; // Used for single hit
	pU<bool[]>	m_puMouseButton;
	pU<bool[]>	m_puMouseButtonDown; // Used for single hit

	grV2f m_MousePos;
	intS m_MouseScroll;
	sizeT m_KeyCount, m_MouseCount;
	bool m_bMouseMoved;
};

#endif		//_GRINPUT_H_