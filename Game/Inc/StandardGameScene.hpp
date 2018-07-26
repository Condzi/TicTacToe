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
	Field currentTurnField;
	con::Text currentTurnText;

	StandardBoard* board;

	void onPush() override
	{
		tag = "Standard Game";

		auto& textures = con::Global.Assets.Texture;

		textures.load( "data/ox.png", "ox" );
		textures.load( "data/board.png", "board" );

		board = &spawn<StandardBoard>( Vec2f{ 96, 250 } );
		initCurrentTurnData();
	}

	void onUpdate() override
	{
		if ( auto fieldModeRet = board->getFieldModeAtMousePosition(); fieldModeRet.has_value() ) {
			auto& fieldMode = *fieldModeRet.value();
			if ( fieldMode != Field::Empty )
				return;

			if ( con::Global.Input.isDown( con::MouseButton::Left ) ) {
				fieldMode = currentTurnField.mode;

				if ( currentTurnField.mode == Field::O )
					currentTurnField.mode = Field::X;
				else if ( currentTurnField.mode == Field::X )
					currentTurnField.mode = Field::O;

				currentTurnField.updateSprite();
			}
		}
	}

private:
	void initCurrentTurnData()
	{
		// reinit
		currentTurnField = Field{};
		currentTurnField.mode = Field::O;
		currentTurnText.setString( "turn" );
		currentTurnText.setFont( con::Global.Assets.Font.getDefault() );

		auto& fieldSprite = currentTurnField.sprite;

		fieldSprite.setScale( Scale, Scale );
		fieldSprite.setTexture( con::Global.Assets.Texture.get( "ox" ) );

		fieldSprite.setPosition( board->position.x + Field::VisualSize.x, board->position.y - Field::VisualSize.y * 1.2 );
		currentTurnText.setPosition( fieldSprite.getPosition().x + Field::VisualSize.x * 1.2f, fieldSprite.getPosition().y + Field::VisualSize.y / 3 );

		board->defaultColor = currentTurnField.defaultColor = sf::Color( 133, 181, 222 );
		currentTurnField.updateSprite();
	}
};