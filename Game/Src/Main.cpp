/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "GamePCH.hpp"

#include "StandardGameScene.hpp"

int main()
{
	con::Global.SceneStack.registerScene<StandardGameScene>( static_cast<int16_t>( SceneID::StandardGame ) );
	con::Global.SceneStack.push( static_cast<int16_t>( SceneID::StandardGame ) );

	con::Global.Game.run();
}