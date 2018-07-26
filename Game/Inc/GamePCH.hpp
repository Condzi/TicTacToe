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
	enum Mode
	{
		Empty,
		O,
		X
	} mode;
};