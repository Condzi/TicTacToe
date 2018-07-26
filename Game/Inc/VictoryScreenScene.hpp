/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

class VictoryScreenScene final :
	public con::Scene
{
public:
	con::RectangleShape backgroundRectangle;
	con::Sprite winnerSprite;
	con::Text winText;
	con::Text timeText;

	const sf::Color BackgroundColor = sf::Color( 40, 40, 40 );
	const sf::Color TextColor = sf::Color( 130, 130, 130 );

	static constexpr float32_t FadeInDelta = 15.0f;
	static constexpr float32_t FadeOutDelta = 25.0f;
	float transparency = 0;

	VictoryScreenScene()
	{
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

		winnerSprite.setPosition( 96 + Field::VisualSize.x / 2, 250 - Field::VisualSize.y * 1.2 );
		winText.setPosition( winnerSprite.getPosition().x + offsetInX, winnerSprite.getPosition().y + Field::VisualSize.y / 3 );
		timeText.setPosition( winnerSprite.getPosition().x, winnerSprite.getPosition().y + Field::VisualSize.y * 1.2 );
	}

	void onUpdate() override
	{
		updateTransparency();
		checkInput();

		if ( state == State::Exit ) {
			con::Global.SceneStack.pop();
			con::Global.SceneStack.enableCurrentScene();
		}
	}

private:
	enum class State : uint8_t
	{
		FadeIn,
		FadeOut,
		Stable,
		Exit
	} state;

	void updateTransparency()
	{
		if ( state == State::FadeIn ) {

			if ( transparency <= 255 ) {
				transparency += FadeInDelta;
				if ( transparency >= 255 ) {
					state = State::Stable;
					transparency = 255;
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
	}

	void checkInput()
	{
		// @ToDo: Placeholder until buttons show up.
		if ( state == State::Stable )
			if ( con::Global.Input.isDown( con::KeyboardKey::Escape ) )
				state = State::FadeOut;
	}
};
