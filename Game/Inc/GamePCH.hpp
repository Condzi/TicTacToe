/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

// Engine includes.

#include <Engine/EnginePCH.hpp>
#include <Engine/Global.hpp>
#include <Engine/Game.hpp>
#include <Engine/GUI.hpp>
#include <Engine/Assets.hpp>
#include <Engine/SceneStack.hpp>
#include <Engine/Entity.hpp>
#include <Engine/Input.hpp>
#include <Engine/Utility.hpp>
#include <Engine/FixedArray2D.hpp>
#include <Engine/Drawable.hpp>

// Common structures / enums etc

constexpr float32_t Scale = 1.5f;

enum class SceneID : uint8_t
{
	Menu,
	StandardGame,
	GomoroGame,
	VictoryScreen
};

struct Field final
{
	inline static const Vec2u TextureSize{ 64,64 };
	inline static const Vec2u VisualSize{ static_cast<uint32_t>( TextureSize.x * Scale ), static_cast<uint32_t>( TextureSize.y * Scale ) };

	con::Sprite sprite;
	sf::Color defaultColor = sf::Color::White;
	enum Mode
	{
		Empty,
		O,
		X
	} mode = Empty;

	Field()
	{
		sprite.setScale( Scale, Scale );
		sprite.setTexture( con::Global.Assets.Texture.get( "ox" ) );
	}

	void updateSprite()
	{
		if ( mode == Empty )
			sprite.setColor( sf::Color::Transparent );
		else
			sprite.setColor( defaultColor );

		auto texSize = static_cast<Vec2i>( TextureSize );

		if ( mode == O )
			sprite.setTextureRect( { 0,0, texSize.x, texSize.y } );
		else if ( mode == X )
			sprite.setTextureRect( { texSize.x,0, texSize.x, texSize.y } );
	}
};

struct CurrentTurn final
{
	Field field;
	con::Text text;
};

struct Timer final
{
	Timer()
	{
		auto desiredColor = TextColorCounting;
		auto startingColor = TextColorCountdown;
		auto t = CountdownTime.asSeconds();

		// just V = s/t
		colorDelta.r = ( desiredColor.r - startingColor.r ) / t;
		colorDelta.g = ( desiredColor.g - startingColor.g ) / t;
		colorDelta.b = ( desiredColor.b - startingColor.b ) / t;
		colorDelta.a = ( desiredColor.a - startingColor.a ) / t;
	}

	enum class Mode
	{
		Countdown,
		Counting
	} mode = Mode::Countdown;

	inline static const sf::Time CountdownTime = sf::seconds( 1 );
	sf::Clock clock;
	con::Text text;
	inline static const sf::Color TextColorCountdown = sf::Color::Red;
	inline static const sf::Color TextColorCounting = sf::Color::White;
	struct
	{
		float32_t r, g, b, a;
	} colorDelta;
	// @ToDo: Different text color for Modes.

	void reset()
	{
		auto countdownTimeStr = con::ConvertTo<std::string>( CountdownTime.asSeconds() + 0.001 );

		text.setString( con::ConvertTo<std::string>( "-", countdownTimeStr.substr( 0, countdownTimeStr.find( '.' ) + 3 ), "s" ) );
		clock.restart();
		mode = Mode::Countdown;
		text.setFillColor( TextColorCountdown );
	}

	void updateTimerText()
	{
		std::string secondsString;
		if ( mode == Mode::Countdown ) {
			float32_t sec = clock.getElapsedTime().asSeconds();
			float32_t countdownSec = CountdownTime.asSeconds();
			float32_t countdownDelta = countdownSec - sec;

			// @Bug: Can't set sf::Clock time point, so there is small amout of milliseconds that are discarded.
			if ( countdownDelta <= 0.0f ) {
				mode = Mode::Counting;
				clock.restart();
				text.setFillColor( TextColorCounting );
			} else {
				// Fading to TextColorCounting 
				auto currentColor = text.getFillColor();
				auto dt = con::Global.FrameTime.asSeconds();

				currentColor.r += colorDelta.r * dt;
				currentColor.g += colorDelta.g * dt;
				currentColor.b += colorDelta.b * dt;
				currentColor.a += colorDelta.a * dt;

				std::cout << (int)currentColor.r << ", " << (int)currentColor.g << ", " << (int)currentColor.b << ", d: "<< countdownDelta << '\n';

				text.setFillColor( currentColor );

				secondsString = con::ConvertTo<std::string>( "-", countdownSec - sec );
			}
		}
		if ( mode == Mode::Counting )
			secondsString = con::ConvertTo<std::string>( clock.getElapsedTime().asSeconds() );

		auto finalString = secondsString.substr( 0, secondsString.find( '.' ) + 3 );
		text.setString( con::ConvertTo<std::string>( finalString, "s" ) );
	}
};

// @ToDo: Add to the engine codebase.
template <typename T, typename ...TArgs>
bool allOf( T t, TArgs&& ...args )
{
	return ( ( t == args ) && ... );
}