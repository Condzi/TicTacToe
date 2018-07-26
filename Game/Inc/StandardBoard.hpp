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
		setTexture();
	}

	void onUpdate() override
	{
		for ( auto& field : fields )
			field.updateSprite();
	}

	std::optional<Field::Mode* const> getFieldModeAtMousePosition()
	{
		auto mousePos = static_cast<Vec2f>( con::Global.Input.getMousePosition() );

		for ( auto& field : fields ) {
			sf::FloatRect fixedRect = field.sprite.getGlobalBounds();
			fixedRect.width -= Field::VisualSize.x;
			if ( fixedRect.contains( mousePos ) )
				return &field.mode;
		}

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

	void setTexture()
	{
		board.setTexture( con::Global.Assets.Texture.get( "board" ) );
		board.setScale( Scale, Scale );
	}
};