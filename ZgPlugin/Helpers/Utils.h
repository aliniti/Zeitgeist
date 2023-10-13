#pragma once

class Utils
{
public:
    auto Get(uint16_t handle) -> GameObject*;
    auto IsValidTarget( GameObject* pObject, float range = 99999999.0f ) -> bool;
    
    auto IsOnSegment(Vector3 pos, Vector3 start, Vector3 end, float radius ) -> bool;
    auto UnderEnemyTurret( ) -> bool;
    auto UnderEnemyTurret( Vector3 pos ) -> bool;
    auto UnderEnemyTurret( GameObject* unit ) -> bool;
    auto CountEnemiesInRange( GameObject* pObject, float range ) -> int;
};

