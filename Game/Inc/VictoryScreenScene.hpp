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
	float transparency = 0;
	// @ToDo: Fade out

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
		if ( transparency < 255 ) {
			transparency += FadeInDelta;
			if ( transparency > 255 )
				transparency = 255;
			auto fill = BackgroundColor;
			fill.a = transparency;
			backgroundRectangle.setFillColor( fill );
			fill = TextColor;
			fill.a = transparency;
			winText.setFillColor( fill );
			timeText.setFillColor( fill );
			return;
		}
	}
};
