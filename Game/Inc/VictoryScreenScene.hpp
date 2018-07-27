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

	const sf::Color BackgroundColor = sf::Color( 30, 50, 60 );
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

	void onPush()
	{
		winText.setFont( con::Global.Assets.Font.getDefault() );
		timeText.setFont( con::Global.Assets.Font.getDefault() );

		if ( con::Global.PlayerStats.at( "winner" ) == "draw" )
			winText.setString( "draw!" );
		else
			winText.setString( "won!" );

		timeText.setString( con::ConvertTo<std::string>( "Time: ", con::Global.PlayerStats.at( "time" ) ) );

		auto texSize = static_cast<Vec2i>( Field::TextureSize );

		float offsetInX = Field::VisualSize.x * 1.2f;

		if ( con::Global.PlayerStats.at( "winner" ) == "O" )
			winnerSprite.setTextureRect( { 0,0,texSize.x, texSize.y } );
		else if ( con::Global.PlayerStats.at( "winner" ) == "X" )
			winnerSprite.setTextureRect( { texSize.x,0,texSize.x, texSize.y } );
		// we want to have two fields displayed
		else {
			offsetInX += Field::VisualSize.x;
			winnerSprite.setTextureRect( { 0, 0, texSize.x * 2, texSize.y } );
		}

		winnerSprite.setPosition( 96 + Field::VisualSize.x * 0.5f, 250 - Field::VisualSize.y * 1.28f ); // 1.28 instead 1.3 to make small animation effect
		winText.setPosition( winnerSprite.getPosition().x + offsetInX, winnerSprite.getPosition().y + Field::VisualSize.y / 3 );
		timeText.setPosition( winnerSprite.getPosition().x, winnerSprite.getPosition().y + Field::VisualSize.y * 1.2f );

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
	void initButtons()
	{
		againButton = &spawn<Button>();
		menuButton = &spawn<Button>();

		againButton->position = timeText.getPosition();
		againButton->position.y += Field::VisualSize.y * 1.2f;
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

		// @ToDo: Go to menu.
		menuButton->callback = [&]() {
			con::Global.ExitGame = true;
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

		fill = sf::Color::White;
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
