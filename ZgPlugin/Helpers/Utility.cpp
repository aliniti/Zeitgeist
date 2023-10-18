#include "../stdafx.hpp"

auto Utility::Get( uint16_t handle ) -> GameObject*
{
    return g_pExportedObjectManager->GetObjectByHandle( handle );
}

auto Utility::IsValidTarget( GameObject* pObject, float range ) -> bool
{
    if ( pObject == nullptr || !pObject->IsAttackable( true, false ) )
    {
        return false;
    }

    return GetPlayer( )->DistanceXZ( pObject ) <= range;
}

auto Utility::IsOnSegment(Vector3 pos, Vector3 start, Vector3 end, float radius ) -> bool
{
    Vector3 segment;
    if (g_pExportedMath->IsOnSegment( pos, start, end, segment ))
    {
        if (pos.DistanceXZ( segment ) <= radius)
        {
            return true;
        }
    }

    return false;
}

auto Utility::UnderEnemyTurret( ) -> bool
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

auto Utility::UnderEnemyTurret( Vector3 pos ) -> bool
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

auto Utility::UnderEnemyTurret( GameObject* unit ) -> bool
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

auto Utility::CountEnemiesInRange( float range ) -> int
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
