/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

#include "Button.hpp"

class VictoryScreenScene final :
	public con::Scene
{
public:
	enum class State : uint8_t
	{
		FadeIn,
		FadeOut,
		Stable,
		Exit
	} state;

	con::RectangleShape backgroundRectangle;
	con::Sprite winnerSprite;
	con::Text winText;
	con::Text timeText;

	Button* againButton;
	Button* menuButton;

	const sf::Color BackgroundColor = sf::Color( 30, 40, 50 );
	const sf::Color TextColor = sf::Color( 133, 181, 222 );

	static constexpr float32_t FadeInDelta = 10.0f;
	static constexpr float32_t FadeOutDelta = 25.0f;
	float transparency = 0;

	VictoryScreenScene()
	{
		tag = "Victory Screen";

		backgroundRectangle.setSize( static_cast<Vec2f>( con::Global.GameWindow.getSize() ) );
		winnerSprite.setTexture( con::Global.Assets.Texture.get( "ox" ) );
		winnerSprite.setScale( Scale, Scale );
	}

	void onPush() override
	{
		initTextAndSprite();
		initButtons();
	}

	void onUpdate() override
	{
		updateTransparency();

		if ( state == State::Exit ) {
			con::Global.SceneStack.pop();
			con::Global.SceneStack.enableCurrentScene();
		}
	}

private:
	void initTextAndSprite()
	{
		winText.setFont( con::Global.Assets.Font.getDefault() );
		timeText.setFont( con::Global.Assets.Font.getDefault() );
		winText.setCharacterSize( 72 );
		timeText.setCharacterSize( 50 );

		if ( con::Global.PlayerStats.at( "winner" ) == "draw" )
			winText.setString( "draw!" );
		else
			winText.setString( "won!" );

		timeText.setString( con::ConvertTo<std::string>( "Time: ", con::Global.PlayerStats.at( "time" ) ) );

		auto texSize = static_cast<Vec2i>( Field::TextureSize );

		winnerSprite.setPosition( 96 + Field::VisualSize.x * 0.5f, 250 - Field::VisualSize.y * 1.3f );

		if ( con::Global.PlayerStats.at( "winner" ) == "O" )
			winnerSprite.setTextureRect( { 0,0,texSize.x, texSize.y } );
		else if ( con::Global.PlayerStats.at( "winner" ) == "X" )
			winnerSprite.setTextureRect( { texSize.x,0,texSize.x, texSize.y } );
		// we want to have two fields displayed
		else {
			winnerSprite.move( -static_cast<float>( Field::VisualSize.x ), 0 );
			winnerSprite.setTextureRect( { 0, 0, texSize.x * 2, texSize.y } );
		}

		
		winText.setPosition( winnerSprite.getPosition() );
		winText.move( winnerSprite.getGlobalBounds().width * 1.1f, 0 );
		auto windowSize = static_cast<Vec2f>( con::Global.GameWindow.getSize() );
		Vec2f timeTextSize{ timeText.getGlobalBounds().width, timeText.getGlobalBounds().height };
		timeText.setPosition( windowSize / 2.f - timeTextSize / 2.f );
		timeText.move( 0, static_cast<float32_t>( Button::TextureSize.y ) * -1.3f );
	}

	void initButtons()
	{
		againButton = &spawn<Button>();
		menuButton = &spawn<Button>();

		againButton->position = static_cast<Vec2f>( con::Global.GameWindow.getSize() ) / 2.f - static_cast<Vec2f>( Button::TextureSize / 2u );
		menuButton->position = againButton->position;
		menuButton->position.y += Button::TextureSize.y * 1.2f;

		againButton->sprite.setTexture( con::Global.Assets.Texture.get( "buttons" ) );
		againButton->sprite.setTextureRect( sf::IntRect( 2*Button::TextureSize.x, 0, Button::TextureSize.x, Button::TextureSize.y ) );

		menuButton->sprite.setTexture( con::Global.Assets.Texture.get( "buttons" ) );
		menuButton->sprite.setTextureRect( sf::IntRect( 3*Button::TextureSize.x, 0, Button::TextureSize.x, Button::TextureSize.y ) );

		againButton->callback = [&]() {
			if ( state == State::Stable ) {
				state = State::FadeOut;
				menuButton->recieveInput = false;
				againButton->recieveInput = false;
			}
		};

		menuButton->callback = [&]() {
			// Pop this and game scene.
			con::Global.SceneStack.pop();
			con::Global.SceneStack.pop();
			con::Global.SceneStack.push( static_cast<int16_t>( SceneID::Menu ) );
		};

		menuButton->recieveInput = false;
		againButton->recieveInput = false;
	}

	void updateTransparency()
	{
		if ( state == State::FadeIn ) {
			if ( transparency <= 255 ) {
				transparency += FadeInDelta;
				if ( transparency >= 255 ) {
					state = State::Stable;
					transparency = 255;
					menuButton->recieveInput = true;
					againButton->recieveInput = true;
				}
			}
		} else if ( state == State::FadeOut ) {
			if ( transparency >= 0 ) {
				transparency -= FadeOutDelta;
				if ( transparency <= 0 ) {
					state = State::Exit;
					transparency = 0;
				}
			}
		} else return;

		// if fade in / fade out then set these
		auto fill = BackgroundColor;
		fill.a = transparency;
		backgroundRectangle.setFillColor( fill );
		fill = TextColor;
		fill.a = transparency;
		winText.setFillColor( fill );
		timeText.setFillColor( fill );

		fill = Field::DefaultColor;
		fill.a = transparency;
		winnerSprite.setColor( fill );

		fill = menuButton->colorNormal;
		fill.a = transparency;
		menuButton->sprite.setColor( fill );
		fill = againButton->colorNormal;
		fill.a = transparency;
		againButton->sprite.setColor( fill );
	}
};
