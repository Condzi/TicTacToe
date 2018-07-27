/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

#include "StandardBoard.hpp"
#include "VictoryScreenScene.hpp"
#include "Timer.hpp"
#include "Button.hpp"

class StandardGameScene final :
	public con::Scene
{
public:
	const sf::Color WinnerColor = sf::Color( 133, 181, 222 );
	con::RectangleShape background;
	CurrentTurn currentTurn;
	Timer timer;
	StandardBoard* board;
	Button* smallExitButton;

	void onPush() override
	{
		tag = "Standard Game";
		board = &spawn<StandardBoard>( Vec2f{ 96, 250 } );

		initCurrentTurnData();
		initTimer();
		initButton();

		background.setSize( static_cast<Vec2f>( con::Global.GameWindow.getSize() ) );
		background.setFillColor( sf::Color( 30, 40, 50 ) );
	}

	void onEnable() override
	{
		// Because we are reseting the timer in cleanUpDuringVictoryScreen
		timer.clock.restart();
	}

	void onUpdate() override
	{
		cleanUpDuringVictoryScreen();
		// don't update below
		if ( auto val = con::Global.SceneStack.getSceneOnTop().value(); val->tag == "Victory Screen" )
			return;

		checkInput();
		tryCheckWin();
		timer.updateTimerText();
	}

private:
	bool makeWinCheck = false;

	void initCurrentTurnData()
	{
		// reinit
		auto& field = currentTurn.field;
		auto& text = currentTurn.text;

		field = Field{};
		field.mode = Field::O;
		text.setString( "turn" );
		text.setCharacterSize( 50 );
		text.setFont( con::Global.Assets.Font.getDefault() );

		auto& fieldSprite = field.sprite;

		fieldSprite.setScale( Scale, Scale );
		fieldSprite.setTexture( con::Global.Assets.Texture.get( "ox" ) );

		fieldSprite.setPosition( board->position.x + Field::VisualSize.x * 0.5, board->position.y - Field::VisualSize.y * 1.3 );
		text.setPosition( fieldSprite.getPosition().x + Field::VisualSize.x * 1.2f, fieldSprite.getPosition().y + Field::VisualSize.y / 3 );

		board->defaultColor = field.defaultColor = WinnerColor;
		field.updateSprite();
	}

	void checkInput()
	{
		// Don't check input if game didn't start
		if ( timer.mode == Timer::Mode::Countdown )
			return;

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
				makeWinCheck = true;
			}
		}
	}

	void initTimer()
	{
		timer.reset();

		timer.text.setFont( con::Global.Assets.Font.getDefault() );
		timer.text.setCharacterSize( 50 );
		timer.text.setPosition( board->position.x + Field::VisualSize.x * 0.9f, currentTurn.field.sprite.getPosition().y - Field::VisualSize.y * 0.9f );
	}

	std::optional<Field::Mode> checkWin()
	{
		makeWinCheck = false;
		auto& fields = board->fields;

		// max 3
		uint8_t countOfFieldModes = 0;
		Field::Mode currentCheckingMode;
		// For highliting winner
		sf::Color* fieldsColors[3];
		auto highlightFields = [&] {
			for ( auto field : fieldsColors )
				*field = WinnerColor;
		};
		auto isWinnersLine = [&] {
			if ( countOfFieldModes == 3 && currentCheckingMode != Field::Mode::Empty ) {
				highlightFields();
				return true;
			}
			return false;
		};
		// check every row
		for ( size_t y = 0; y < 3; y++ ) {
			const Vec2u firstFieldPos{ 0,y };
			fieldsColors[0] = &fields.at( firstFieldPos ).defaultColor;
			currentCheckingMode = fields.at( firstFieldPos ).mode;
			countOfFieldModes = 1;
			for ( size_t x = 1; x < 3; x++ ) {
				if ( auto& field = fields.at( { x,y } ); field.mode == currentCheckingMode ) {
					fieldsColors[countOfFieldModes] = &field.defaultColor;
					countOfFieldModes++;
				} else
					break;
			}

			if ( isWinnersLine() )
				return currentCheckingMode;
		}

		// check every column
		for ( size_t x = 0; x < 3; x++ ) {
			const Vec2u firstFieldPos{ x,0 };
			fieldsColors[0] = &fields.at( firstFieldPos ).defaultColor;
			currentCheckingMode = fields.at( firstFieldPos ).mode;
			countOfFieldModes = 1;
			for ( size_t y = 1; y < 3; y++ ) {
				if ( auto& field = fields.at( { x,y } ); field.mode == currentCheckingMode ) {
					fieldsColors[countOfFieldModes] = &field.defaultColor;
					countOfFieldModes++;
				} else
					break;
			}

			if ( isWinnersLine() )
				return currentCheckingMode;
		}

		// check crossing A
		if ( allOf( fields.at( { 0,0 } ).mode, fields.at( { 1,1 } ).mode, fields.at( { 2,2 } ).mode ) )
			if ( auto m = fields.at( { 0,0 } ).mode; m != Field::Mode::Empty ) {
				fieldsColors[0] = &fields.at( { 0,0 } ).defaultColor;
				fieldsColors[1] = &fields.at( { 1,1 } ).defaultColor;
				fieldsColors[2] = &fields.at( { 2,2 } ).defaultColor;
				highlightFields();
				return m;
			}

		// check crossing B
		if ( allOf( fields.at( { 0,2 } ).mode, fields.at( { 1,1 } ).mode, fields.at( { 2,0 } ).mode ) )
			if ( auto m = fields.at( { 0,2 } ).mode; m != Field::Mode::Empty ) {
				fieldsColors[0] = &fields.at( { 0,2 } ).defaultColor;
				fieldsColors[1] = &fields.at( { 1,1 } ).defaultColor;
				fieldsColors[2] = &fields.at( { 2,0 } ).defaultColor;
				highlightFields();
				return m;
			}

		// If all fields are occupied then return Empty - this means that it's a draw.
		if ( std::none_of( fields.begin(), fields.end(), []( const Field& f ) {
			return f.mode == Field::Empty;
		} ) )
			return Field::Empty;

		return {};
	}

	void initButton()
	{
		smallExitButton = &spawn<Button>();
		// @ToDo: Go to menu.
		smallExitButton->callback = []() {
			con::Global.ExitGame = true;
		};
		smallExitButton->sprite.setTexture( con::Global.Assets.Texture.get( "buttons" ) );
		smallExitButton->sprite.setTextureRect( sf::IntRect( 4*Button::TextureSize.x,0, Button::TextureSize.x * 0.5f,Button::TextureSize.y ) );
		smallExitButton->position = Vec2f( 8, con::Global.GameWindow.getSize().y - Button::TextureSize.y - 8 );
	}

	void cleanUpDuringVictoryScreen()
	{
		// Disable in next frame - give time for empty fields to render as empty
		static bool disableNow = false;
		if ( disableNow ) {
			disableNow = false;
			// reset, so 'O' will start first (we set it as last placed Field below, so need to reset)
			currentTurn.field.mode = Field::O;
			currentTurn.field.updateSprite();
			timer.reset();
			Disable();
			return;
		}

		// Clear fields only if player can't see them
		if ( auto val = con::Global.SceneStack.getSceneOnTop().value(); val->tag == "Victory Screen" ) {
			if ( dynamic_cast<VictoryScreenScene&>( *val ).state == VictoryScreenScene::State::Stable ) {

				for ( auto& field : board->fields ) {
					field.mode = Field::Empty;
					field.defaultColor = sf::Color::White;
					field.updateSprite();
				}

				disableNow = true;
			}
		}
	}

	void tryCheckWin()
	{
		if ( makeWinCheck )
			if ( auto w = checkWin(); w ) {
				// get the string from timer text, so update it first
				timer.updateTimerText();
				con::Global.PlayerStats["time"] = timer.text.getString().toAnsiString();

				if ( w.value() == Field::Empty )
					con::Global.PlayerStats["winner"] = "draw";
				else
					con::Global.PlayerStats["winner"] = w.value()==Field::O ? "O" : "X";

				con::Global.SceneStack.push( static_cast<int16_t>( SceneID::VictoryScreen ) );

				// in input we set the next field's turn, and it's not esthetical
				currentTurn.field.mode = currentTurn.field.mode==Field::O ? Field::X : Field::O;
				currentTurn.field.updateSprite();
			}
	}
};