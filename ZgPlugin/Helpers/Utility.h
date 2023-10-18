#pragma once

class Utility
{
public:
    auto Get(uint16_t handle) -> GameObject*;
    auto IsValidTarget( GameObject* pObject, float range = 99999999.0f ) -> bool;
    
    auto IsOnSegment(Vector3 pos, Vector3 start, Vector3 end, float radius ) -> bool;
    auto UnderEnemyTurret( ) -> bool;
    auto UnderEnemyTurret( Vector3 pos ) -> bool;
    auto UnderEnemyTurret( GameObject* unit ) -> bool;
    auto CountEnemiesInRange( float range ) -> int;

    auto operator new( size_t size ) -> void* { return Globals::Malloc( size ); }
    auto operator delete( void* p ) noexcept -> void { Globals::Free( p ); }
};
