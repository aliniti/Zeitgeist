#include "../stdafx.hpp"

Spell::Spell( SpellSlot slot, float range )
{
    this->slot = slot;
    this->range = range;
    this->time = 0;
}

float Spell::Range( ) const
{
    return this->range;
}

SpellSlot Spell::Slot( ) const
{
    return this->slot;
}

float Spell::LastCastTime( ) const
{
    return this->time;
}

bool Spell::IsReady( const float time ) const
{
    const auto spellbook = GetPlayer( )->Spellbook( );
    if ( !spellbook ) return false;

    const auto spell = spellbook->GetSpell( this->slot );
    if ( !spell || spell->Level( ) <= 0 ) return false;

    if ( spell->GetRemainingCooldown( ) > 0 && spell->GetRemainingCooldown( ) <= time ) return true;
    return spellbook->CanUseSpell( this->slot );
}

void Spell::Cast( )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time > .25f )
    {
        const auto player = GetPlayer( );
        if ( const auto spellbook = player->Spellbook( ) )
        {
            auto position = player->Position( );
            if ( spellbook->SendSpellCastPacket( this->slot, &position, &position, nullptr ) )
            {
                this->time = g_pExportedGlobalClocks->GameTime( );
            }
        }
    }
}

void Spell::Cast( GameObject* target )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time > .25f )
    {
        const auto player = GetPlayer( );
        if ( const auto spellbook = player->Spellbook( ) )
        {
            auto position = target->Position( );
            if ( spellbook->SendSpellCastPacket( this->slot, &position, &position, target ) )
            {
                this->time = g_pExportedGlobalClocks->GameTime( );
            }
        }
    }
}

void Spell::Cast( Vector3 position )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time > .25f )
    {
        const auto player = GetPlayer( );
        if ( const auto spellbook = player->Spellbook( ) )
        {
            if ( spellbook->SendSpellCastPacket( this->slot, &position, &position, nullptr ) )
            {
                this->time = g_pExportedGlobalClocks->GameTime( );
            }
        }
    }
}
