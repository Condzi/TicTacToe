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

enum class SceneID : uint8_t
{
	Menu,
	StandardGame,
	GomoroGame
	// FinishScreen ?
};

struct Field final
{
	inline static const Vec2u Size{ 64,64 };

	con::Sprite sprite;
	enum
	{
		Empty,
		O,
		X
	} mode;
};