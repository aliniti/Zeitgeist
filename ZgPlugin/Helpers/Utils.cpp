#include "../stdafx.hpp"


namespace Utils
{
    auto IsValidTarget( GameObject* pObject, float range ) -> bool
    {
        if ( pObject == nullptr || !pObject->IsAttackable( true, false ) )
        {
            return false;
        }

        return GetPlayer( )->DistanceXZ( pObject ) <= range;
    }

    auto UnderEnemyTurret( ) -> bool
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

    auto UnderEnemyTurret( Vector3 pos ) -> bool
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

    auto UnderEnemyTurret( GameObject* unit ) -> bool
    {
        int turrets = 0;

        for ( auto i : g_pExportedEntityManager->Turrets( ) )
        {
            if ( i->IsEnemy( ) && !i->IsDead( ) )
            {
                if ( i->DistanceXZ( unit->Position( ) ) <= 1100 )
                {
                    turrets++;
                }
            }
        }

        return turrets > 0;
    }

    auto CountEnemiesInRange( GameObject* pObject, float range ) -> int
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
