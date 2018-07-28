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
	con::Text createdByText;
	Button* playButton;
	Button* exitButton;
	Button* musicButton;
	Button* soundButton;
	bool musicOn = true, soundOn = true;

	MenuScene()
	{
		tag = "Menu";
	}

	void onPush() override
	{
		initOptionsFromSettings();
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
	void initOptionsFromSettings()
	{
		auto& settings = con::Global.Assets.Settings;
		musicOn = con::ConvertTo<bool>( settings.getValue( "GAME", "MUSIC_ON" ).value() );
		soundOn = con::ConvertTo<bool>( settings.getValue( "GAME", "SOUND_ON" ).value() );
	}

	void initButtons()
	{
		playButton = &spawn<Button>();
		exitButton = &spawn<Button>();

		auto& buttonsTexture = con::Global.Assets.Texture.get( "buttons" );
		playButton->sprite.setTexture( buttonsTexture );
		exitButton->sprite.setTexture( buttonsTexture );

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

		musicButton = &spawn<Button>();
		soundButton = &spawn<Button>();

		musicButton->sprite.setTexture( buttonsTexture );
		soundButton->sprite.setTexture( buttonsTexture );
		updateButtonsIcons();

		musicButton->position = Vec2f( 8, con::Global.GameWindow.getSize().y - Button::TextureSize.y - 8 );
		soundButton->position = musicButton->position;
		soundButton->position.x += Button::TextureSize.x * 0.5f;

		musicButton->callback = [&] {
			musicOn = !musicOn;
			con::Global.Assets.Settings.setValue( "GAME", "MUSIC_ON", con::ConvertTo<std::string>( musicOn ) );

			updateVolume();
			updateButtonsIcons();
		};

		soundButton->callback = [&] {
			soundOn = !soundOn;
			con::Global.Assets.Settings.setValue( "GAME", "SOUND_ON", con::ConvertTo<std::string>( soundOn ) );

			updateVolume();
			updateButtonsIcons();
		};

		updateVolume();
		updateButtonsIcons();
	}

	void updateButtonsIcons()
	{
		if ( musicOn )
			musicButton->sprite.setTextureRect( sf::IntRect( 4.5f * Button::TextureSize.x, 0, Button::TextureSize.x * 0.5f, Button::TextureSize.y ) );
		else
			musicButton->sprite.setTextureRect( sf::IntRect( 5 * Button::TextureSize.x, 0, Button::TextureSize.x * 0.5f, Button::TextureSize.y ) );

		if ( soundOn )
			soundButton->sprite.setTextureRect( sf::IntRect( 5.5f * Button::TextureSize.x, 0, Button::TextureSize.x * 0.5f, Button::TextureSize.y ) );
		else
			soundButton->sprite.setTextureRect( sf::IntRect( 6 * Button::TextureSize.x, 0, Button::TextureSize.x * 0.5f, Button::TextureSize.y ) );
	}

	void initText()
	{
		title.setFont( con::Global.Assets.Font.getDefault() );
		title.setCharacterSize( 72 );
		title.setString( "Tic Tac Toe" );

		createdByText.setFont( con::Global.Assets.Font.getDefault() );
		createdByText.setCharacterSize( 32 );
		createdByText.setString( "created by\nCondzi" );
		createdByText.setFillColor( sf::Color( 255, 255, 255, 20 ) );

		auto winSize = static_cast<Vec2f>( con::Global.GameWindow.getSize() );
		Vec2f titleSize{ title.getGlobalBounds().width, title.getGlobalBounds().height };

		title.setPosition( winSize.x / 2 - titleSize.x / 2, titleSize.y * 2.f );
		title.move( 0, static_cast<float32_t>( Button::TextureSize.y ) * -1.2f );

		Vec2f createdBySize{ createdByText.getGlobalBounds().width, createdByText.getGlobalBounds().height };
		createdByText.setPosition( winSize - createdBySize );
		createdByText.move( -8, -16 );
	}

	void updateVolume()
	{
		if ( musicOn )
			con::Global.Assets.Music.setVolume( 100 );
		else
			con::Global.Assets.Music.setVolume( 0 );

		if ( soundOn )
			con::Global.Assets.Sound.setVolume( 100 );
		else
			con::Global.Assets.Sound.setVolume( 0 );
	}
};