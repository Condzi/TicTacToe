/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "GamePCH.hpp"

#include "StandardGameScene.hpp"
#include "VictoryScreenScene.hpp"
#include "MenuScene.hpp"

// @ToDo: Add LoadingScene instead:
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
	
	auto& settings = con::Global.Assets.Settings;
	// Force these dimensions. @ToDo: Probably should be set *before* game loads.
	// @Bug: Doesn't set?
	settings.setValue( "WINDOW", "WIDTH", "500" );
	settings.setValue( "WINDOW", "HEIGHT", "500" );


	if ( !settings.getValue( "GAME", "MUSIC_ON" ).has_value() )
		settings.setValue( "GAME", "MUSIC_ON", "true" );
	if ( !settings.getValue( "GAME", "SOUND_ON" ).has_value() )
		settings.setValue( "GAME", "SOUND_ON", "true" );
}

int main()
{
	loadAssets();

	con::Global.SceneStack.registerScene<MenuScene>( static_cast<int16_t>( SceneID::Menu ) );
	con::Global.SceneStack.registerScene<StandardGameScene>( static_cast<int16_t>( SceneID::StandardGame ) );
	con::Global.SceneStack.registerScene<VictoryScreenScene>( static_cast<int16_t>( SceneID::VictoryScreen ) );

	con::Global.SceneStack.push( static_cast<int16_t>( SceneID::Menu ) );

	con::Global.Game.run();
}