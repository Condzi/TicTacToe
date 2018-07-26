/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

#include "StandardBoard.hpp"

class StandardGameScene final :
	public con::Scene
{
public:
	void onPush() override
	{
		tag = "Standard Game";

		auto& textures = con::Global.Assets.Texture;

		textures.load( "data/ox.png", "ox" );
		textures.load( "data/board.png", "board" );

		spawn<StandardBoard>( Vec2f{ 250, 25 } );
	}
};