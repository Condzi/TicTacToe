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