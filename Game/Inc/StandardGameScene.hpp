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
	CurrentTurn currentTurn;
	Timer timer;
	StandardBoard* board;

	void onPush() override
	{
		tag = "Standard Game";
		board = &spawn<StandardBoard>( Vec2f{ 96, 250 } );

		initCurrentTurnData();
		initTimer();
	}

	void onUpdate() override
	{
		checkInput();
		updateTimerText();
	}

private:
	void initCurrentTurnData()
	{
		// reinit
		auto& field = currentTurn.field;
		auto& text = currentTurn.text;

		field = Field{};
		field.mode = Field::O;
		text.setString( "turn" );
		text.setFont( con::Global.Assets.Font.getDefault() );

		auto& fieldSprite = field.sprite;

		fieldSprite.setScale( Scale, Scale );
		fieldSprite.setTexture( con::Global.Assets.Texture.get( "ox" ) );

		fieldSprite.setPosition( board->position.x + Field::VisualSize.x, board->position.y - Field::VisualSize.y * 1.2 );
		text.setPosition( fieldSprite.getPosition().x + Field::VisualSize.x * 1.2f, fieldSprite.getPosition().y + Field::VisualSize.y / 3 );

		board->defaultColor = field.defaultColor = sf::Color( 133, 181, 222 );
		field.updateSprite();
	}

	void checkInput()
	{
		if ( auto fieldModeRet = board->getFieldModeAtMousePosition(); fieldModeRet.has_value() ) {
			auto& fieldMode = *fieldModeRet.value();
			if ( fieldMode != Field::Empty )
				return;

			if ( con::Global.Input.isDown( con::MouseButton::Left ) ) {
				fieldMode = currentTurn.field.mode;

				if ( currentTurn.field.mode == Field::O )
					currentTurn.field.mode = Field::X;
				else if ( currentTurn.field.mode == Field::X )
					currentTurn.field.mode = Field::O;

				currentTurn.field.updateSprite();
			}
		}
	}

	void initTimer()
	{
		timer.clock.restart();

		timer.text.setFont( con::Global.Assets.Font.getDefault() );
		timer.text.setString( "0.00s" );
		timer.text.setPosition( board->position.x + Field::VisualSize.x * 1.1, currentTurn.field.sprite.getPosition().y - Field::VisualSize.y * 0.5f );
	}

	void updateTimerText()
	{
		auto secondsText = con::ConvertTo<std::string>( timer.clock.getElapsedTime().asSeconds() );
		auto finalText = secondsText.substr( 0, secondsText.find( '.' ) + 3 );

		timer.text.setString( con::ConvertTo<std::string>( finalText, "s" ) );
	}
};