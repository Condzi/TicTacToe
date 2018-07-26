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
	GomoroGame
	// FinishScreen ?
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
	sf::Clock clock;
	con::Text text;
};