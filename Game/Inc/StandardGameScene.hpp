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
	StandardBoard* board;
	enum class Turn
	{
		O, X
	} turn = Turn::O;

	void onPush() override
	{
		tag = "Standard Game";

		auto& textures = con::Global.Assets.Texture;

		textures.load( "data/ox.png", "ox" );
		textures.load( "data/board.png", "board" );

		board = &spawn<StandardBoard>( Vec2f{ 25, 25 } );
	}

	void onUpdate() override
	{
		
		if ( auto fieldModeRet = board->getFieldModeAtMousePosition(); fieldModeRet.has_value() ) {
			auto& fieldMode = *fieldModeRet.value();
			if ( fieldMode != Field::Empty )
				return;

			if ( con::Global.Input.isDown( con::MouseButton::Left ) ) {
				if ( turn == Turn::O ) {
					fieldMode = Field::O;
					turn = Turn::X;
				}
				else if ( turn == Turn::X ) {
					fieldMode = Field::X;
					turn = Turn::O;
				}
			}
		}
		
	}
};