/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

class LoadingScene final :
	public con::Scene
{
public:
	LoadingScene()
	{
		tag = "Loading";
	}

	void onPush() override
	{
		loadAssets();
		updateWindow();

		con::Global.SceneStack.pop();
		con::Global.SceneStack.push( static_cast<int16_t>( SceneID::Menu ) );
	}

private:
	void loadAssets()
	{
		auto& textures = con::Global.Assets.Texture;

		textures.load( "data/ox.png", "ox" );
		textures.load( "data/board.png", "board" );
		textures.load( "data/buttons.png", "buttons" );

		auto& sound = con::Global.Assets.Sound;
		sound.load( "data/select_menu.wav", "select_menu" );
		sound.load( "data/select_field.wav", "select_field" );

		auto& music = con::Global.Assets.Music;
		music.add( "data/bensound-love.ogg", "menu" );
		music.add( "data/bensound-creativeminds.ogg", "game" );
		music.getPlayer().setLoop( true );

		auto& settings = con::Global.Assets.Settings;
		settings.setValue( "WINDOW", "WIDTH", "480" );
		settings.setValue( "WINDOW", "HEIGHT", "720" );

		if ( !settings.getValue( "GAME", "MUSIC_ON" ).has_value() )
			settings.setValue( "GAME", "MUSIC_ON", "true" );
		if ( !settings.getValue( "GAME", "SOUND_ON" ).has_value() )
			settings.setValue( "GAME", "SOUND_ON", "true" );
	}

	void updateWindow()
	{
		auto& settings = con::Global.Assets.Settings;
		auto prevSize = con::Global.GameWindow.getSize();
		// Resize window, because it's created in wrong size by default.
		Vec2u winSize{ con::ConvertTo<uint32_t>( settings.getValue( "WINDOW", "WIDTH" ).value() ), con::ConvertTo<uint32_t>( settings.getValue( "WINDOW", "HEIGHT" ).value() ) };
		con::Global.GameWindow.setSize( winSize );
		// Fix position after resizing.
		if ( prevSize != winSize ) {
			auto winPos = con::Global.GameWindow.getPosition();
			con::Global.GameWindow.setPosition( Vec2i( winPos.x + prevSize.x * 0.5 - winSize.x * 0.5, winPos.y ) );
		}
	}
};