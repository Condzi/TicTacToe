/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

#include "Button.hpp"

class MenuScene final :
	public con::Scene
{
public:
	const sf::Color BackgroundColor = sf::Color( 30, 40, 50 );
	con::RectangleShape backgroundRectangle;
	con::Text title;
	Button* playButton;
	Button* exitButton;

	MenuScene()
	{
		tag = "Menu";
	}

	void onPush() override
	{
		initText();
		initButtons();

		backgroundRectangle.setSize( static_cast<Vec2f>( con::Global.GameWindow.getSize() ) );
		backgroundRectangle.setFillColor( BackgroundColor );

		con::Global.Assets.Music.play( "menu" );
	}

	void onPop() override
	{
		// @ToDo: add Music.stop. Maybe not needed?
		con::Global.Assets.Music.getPlayer().stop();
	}

private:
	void initButtons()
	{
		playButton = &spawn<Button>();
		exitButton = &spawn<Button>();

		playButton->sprite.setTexture( con::Global.Assets.Texture.get( "buttons" ) );
		exitButton->sprite.setTexture( con::Global.Assets.Texture.get( "buttons" ) );

		playButton->sprite.setTextureRect( sf::IntRect( 0, 0, Button::TextureSize.x, Button::TextureSize.y ) );
		exitButton->sprite.setTextureRect( sf::IntRect( Button::TextureSize.x, 0, Button::TextureSize.x, Button::TextureSize.y ) );

		playButton->position = static_cast<Vec2f>( con::Global.GameWindow.getSize() ) / 2.f - static_cast<Vec2f>( Button::TextureSize / 2u );
		exitButton->position = playButton->position;
		exitButton->position.y += Button::TextureSize.y * 1.2f;

		playButton->callback = [] {
			con::Global.SceneStack.pop();
			con::Global.SceneStack.push( static_cast<int16_t>( SceneID::StandardGame ) );
		};

		exitButton->callback = [] {
			con::Global.ExitGame = true;
		};

		playButton->soundName = exitButton->soundName = "select_menu";
	}

	void initText()
	{
		title.setFont( con::Global.Assets.Font.getDefault() );
		title.setCharacterSize( 72 );
		title.setString( "Tic Tac Toe" );

		auto winSizeX = static_cast<float32_t>( con::Global.GameWindow.getSize().x );
		Vec2f titleSize{ title.getGlobalBounds().width, title.getGlobalBounds().height };

		title.setPosition( winSizeX / 2 - titleSize.x / 2, titleSize.y * 2.f );
		title.move( 0, static_cast<float32_t>( Button::TextureSize.y ) * -1.2f );
	}
};