#include "../stdafx.hpp"


namespace Utils
{
    bool IsValidTarget( GameObject* pObject, float range )
    {
        if ( pObject == nullptr || !pObject->IsAttackable( true, false ) )
        {
            return false;
        }
        
        return GetPlayer(  )->DistanceXZ( pObject ) <= range;
    }

    GameObject* Get( uint16_t handle )
    {
        return g_pExportedObjectManager->GetObjectByHandle( handle );
    }

    bool UnderEnemyTurret( )
    {
        int turrets = 0;

        for ( auto i : g_pExportedEntityManager->Turrets( ) )
        {
            if ( i->IsEnemy( ) && !i->IsDead( ) )
            {
                if ( i->DistanceXZ( GetPlayer( ) ) <= 1100 )
                {
                    turrets++;
                }
            }
        }

        return turrets > 0;
    }

    bool UnderEnemyTurret( Vector3 pos )
    {
        int turrets = 0;

        for ( auto i : g_pExportedEntityManager->Turrets( ) )
        {
            if ( i->IsEnemy( ) && !i->IsDead( ) )
            {
                if ( i->DistanceXZ( pos ) <= 1100 )
                {
                    turrets++;
                }
            }
        }

        return turrets > 0;
    }

    int CountEnemiesInRange( GameObject* pObject, float range )
    {
        int units = 0;

        for ( auto i : g_pExportedEntityManager->Heroes( ) )
        {
            if ( i->IsEnemy( ) && IsValidTarget( i, range ) )
            {
                units++;
            }
        }
        
        return units;
    }
}
