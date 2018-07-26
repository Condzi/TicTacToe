/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "GamePCH.hpp"

#include "StandardGameScene.hpp"

int main()
{
	con::Global.SceneStack.registerScene<StandardGameScene>( 0 );
	con::Global.SceneStack.push( 0 );

	con::Global.Game.run();
}