/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

class Button final :
	public con::Entity
{
public:
	std::string soundName;
	inline static const Vec2u TextureSize{ 128, 64 };
	sf::Color colorClick = sf::Color( 150, 150, 150 );
	sf::Color colorNormal = sf::Color::White;
	bool recieveInput = true;

	con::Sprite sprite;
	std::function<void()> callback;

	void onUpdate() override
	{
		sprite.setPosition( position );
		if ( recieveInput )
			checkIsClicked();
	}

private:
	bool isCursorOverButton()
	{
		return sprite.getGlobalBounds().contains( static_cast<Vec2f>( con::Global.Input.getMousePosition() ) );
	}

	void checkIsClicked()
	{
		if ( isCursorOverButton() ) {
			if ( con::Global.Input.isDown( con::MouseButton::Left ) )
				sprite.setColor( colorClick );
			else if ( con::Global.Input.isUp( con::MouseButton::Left ) ) {
				sprite.setColor( colorClick );
				if ( !soundName.empty() )
					con::Global.Assets.Sound.play( soundName );
				if ( callback )
					callback();
			}
		} else
			sprite.setColor( colorNormal );
	}
};