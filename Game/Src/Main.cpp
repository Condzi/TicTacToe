/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "GamePCH.hpp"

#include "StandardGameScene.hpp"

// @ToDo: Add LoadingScene instead:
void loadAssets()
{
	auto& textures = con::Global.Assets.Texture;

	textures.load( "data/ox.png", "ox" );
	textures.load( "data/board.png", "board" );
}

int main()
{
	loadAssets();

	con::Global.SceneStack.registerScene<StandardGameScene>( static_cast<int16_t>( SceneID::StandardGame ) );
	con::Global.SceneStack.push( static_cast<int16_t>( SceneID::StandardGame ) );

	con::Global.Game.run();
}