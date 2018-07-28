/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "GamePCH.hpp"

#include "StandardGameScene.hpp"
#include "VictoryScreenScene.hpp"
#include "MenuScene.hpp"
#include "LoadingScene.hpp"

int main()
{
	con::Global.SceneStack.registerScene<LoadingScene>( static_cast<int16_t>( SceneID::Loading ) );
	con::Global.SceneStack.registerScene<MenuScene>( static_cast<int16_t>( SceneID::Menu ) );
	con::Global.SceneStack.registerScene<StandardGameScene>( static_cast<int16_t>( SceneID::StandardGame ) );
	con::Global.SceneStack.registerScene<VictoryScreenScene>( static_cast<int16_t>( SceneID::VictoryScreen ) );

	con::Global.SceneStack.push( static_cast<int16_t>( SceneID::Loading ) );

	con::Global.Game.run();
}