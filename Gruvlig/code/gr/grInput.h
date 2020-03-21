#ifndef		_GRINPUT_H_
#define		_GRINPUT_H_

#include	<SFML/Window/Keyboard.hpp>	
#include	<SFML/Window/Mouse.hpp>	

#include	"grCommon.h"
#include	"grSingleton.h"


// grInput
//////////////////////////////////////////////////
class grInput : public grSingleton<grInput>
{
public:

							grInput			( void );
							~grInput		( void );

	//////////////////////////////////////////////////

	inline	const	bool	GetKey			( sf::Keyboard::Key key )		const	{ return m_Key[ key ]; }
	inline	const	bool	GetKeyDown		( sf::Keyboard::Key key )
																					{
																						uInt idx = (uInt)key;
																						if ( sf::Keyboard::isKeyPressed( key ) == true && m_KeyDown[ idx ] == false  )
																						{
																							m_KeyDown[ idx ] = true;
																							return true;
																						}
																						return false;
																					}
	inline			void	SetKeyDown		( sf::Keyboard::Key key )				{ m_Key[ key ] = true; }
	inline			void	SetKeyUp		( sf::Keyboard::Key key )				{ m_Key[ key ] = false; }
	inline	const	bool	GetMouse		( sf::Mouse::Button button )	const	{ return m_MouseButton[ button ]; }
	inline	const	bool	GetMouseDown	( sf::Mouse::Button button )
																					{
																						uInt idx = (uInt)button;
																						if ( sf::Mouse::isButtonPressed( button ) == true && m_MouseButtonDown[ idx ] == false )
																						{
																							m_MouseButtonDown[ idx ] = true;
																							return true;
																						}
																						return false;
																					}
	inline			void	SetMouseDown	( sf::Mouse::Button button )			{ m_MouseButton[ button ] = true; }
	inline			void	SetMouseUp		( sf::Mouse::Button button )
																					{
																						m_MouseButton[ button ] = false;
																						m_MouseButtonDown[ (uInt)button ] = false;
																					}
	inline			void	ResetKeys		( void );

	//////////////////////////////////////////////////

private:

	std::vector<bool>	m_Key,
						m_KeyDown,
						m_MouseButton,
						m_MouseButtonDown;

	uInt				m_KeyCount,
						m_MouseCount;
};

#endif		//_GRINPUT_H_