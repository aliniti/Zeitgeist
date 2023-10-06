#pragma once

namespace Utils
{
    bool IsValidTarget( GameObject* pObject, float range = 99999999 );
    GameObject* Get( uint16_t handle );
    
    bool UnderEnemyTurret( );
    bool UnderEnemyTurret( Vector3 pos);
    bool Ready();
    
    int CountEnemiesInRange( GameObject* pObject, float range );
};
