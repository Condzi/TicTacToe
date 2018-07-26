/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

class StandardBoard final :
	public con::Entity
{
public:
	con::FixedArray2D<Field> fields{ {3,3} };

	StandardBoard( const Vec2f& position_ )
	{
		position = position_;

		updatePositions();
		setTextures();
	}

	void onUpdate() override
	{
		for ( auto& field : fields ) {
			// because conversion... 
			auto textureSize = static_cast<Vec2i>( Field::TextureSize );

			// Don't draw anything.
			if ( field.mode == Field::Empty ) {
				field.sprite.setTextureRect( { 0,0,64,64 } );
				field.sprite.setColor( sf::Color::Transparent );
			} else
				field.sprite.setColor( sf::Color::White );

			if ( field.mode == Field::O )
				field.sprite.setTextureRect( { 0,0,textureSize.x, textureSize.y } );
			else if ( field.mode == Field::X )
				field.sprite.setTextureRect( { textureSize.x,0,textureSize.x, textureSize.y } );
		}
	}

	std::optional<Field::Mode* const> getFieldModeAtMousePosition()
	{
		auto mousePos = static_cast<Vec2f>( con::Global.Input.getMousePosition() );

		for ( auto& field : fields )
			if ( field.sprite.getGlobalBounds().contains( mousePos ) )
				return &field.mode;

		return {};
	}

private:
	con::Sprite board;

	void updatePositions()
	{
		board.setPosition( position );
		for ( uint8_t x = 0; x < 3; x++ )
			for ( uint8_t y = 0; y < 3; y++ )
				fields.at( { x,y } ).sprite.setPosition( x * Field::VisualSize.x + position.x, y * Field::VisualSize.y + position.y );
	}

	void setTextures()
	{
		auto& textures = con::Global.Assets.Texture;

		board.setTexture( textures.get( "board" ) );
		board.setScale( Scale, Scale );

		for ( auto& field : fields ) {
			field.sprite.setScale( Scale, Scale );
			field.sprite.setTexture( textures.get( "ox" ) );
		}
	}
};