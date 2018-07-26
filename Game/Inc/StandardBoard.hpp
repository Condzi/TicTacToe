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

	}

private:
	con::Sprite board;

	void updatePositions()
	{
		board.setPosition( position );
		for ( uint8_t x = 0; x < 3; x++ )
			for ( uint8_t y = 0; y < 3; y++ )
				fields.at( { x,y } ).sprite.setPosition( x * Field::Size.x + position.x, y * Field::Size.y + position.y );
	}

	void setTextures()
	{
		auto& textures = con::Global.Assets.Texture;

		board.setTexture( textures.get( "board" ) );
		auto& oxTexture = textures.get( "ox" );

		for ( auto& field : fields ) {
			field.sprite.setTexture( oxTexture );
			// Don't draw anything.
			field.sprite.setTextureRect( { 0,0,0,0 } );
		}
	}
};