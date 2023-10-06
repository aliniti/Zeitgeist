#pragma once

namespace Utils
{
    auto IsValidTarget( GameObject* pObject, float range = 99999999 ) -> bool;
    auto UnderEnemyTurret( ) -> bool;
    auto UnderEnemyTurret( Vector3 pos ) -> bool;
    auto UnderEnemyTurret( GameObject* unit ) -> bool;
    auto CountEnemiesInRange( GameObject* pObject, float range ) -> int;
};
