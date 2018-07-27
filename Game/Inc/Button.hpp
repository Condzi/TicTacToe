/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

class Button final :
	public con::Entity
{
public:
	inline static const Vec2u TextureSize{ 128, 64 };
	inline static const sf::Color ColorClick = sf::Color( 150, 150, 150 );
	inline static const sf::Color ColorNormal = sf::Color::White;

	con::Sprite sprite;
	std::function<void()> callback;

	void onUpdate() override
	{
		sprite.setPosition( position );
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
				sprite.setColor( ColorClick );
			else if ( con::Global.Input.isUp( con::MouseButton::Left ) ) {
				sprite.setColor( ColorClick );
				if ( callback )
					callback();
			}

		} else
			sprite.setColor( ColorNormal );
	}
};