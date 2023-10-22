// ReSharper disable CppClangTidyClangDiagnosticImplicitIntFloatConversion
#include "../stdafx.hpp"

namespace Katarina
{
    // ░█░█░█▀█░▀█▀░█▀█░█▀▄░▀█▀░█▀█░█▀█
    // ░█▀▄░█▀█░░█░░█▀█░█▀▄░░█░░█░█░█▀█
    // ░▀░▀░▀░▀░░▀░░▀░▀░▀░▀░▀▀▀░▀░▀░▀░▀
    // By Kurisu :^)
    
    void OnBoot()
    {
        SetupMenu(  );

        KatQ = new Spell( Q, 625 );
        KatW = new Spell( W, 325 );
        KatE = new Spell( E, 725 );
        KatR = new Spell( R, 550 );

        if (GetPlayer(  )->Spellbook(  )->GetSpell( Summoner1 )->SpellData(  )->Hash(  ) == FNV1A32CI("SummonerDot"))
            Ignite = new Spell ( Summoner1, 600);
        
        if (GetPlayer(  )->Spellbook(  )->GetSpell( Summoner2 )->SpellData(  )->Hash(  ) == FNV1A32CI("SummonerDot"))
            Ignite = new Spell ( Summoner2, 600);

        Cursor = g_pExportedHudManager->CursorPositionUnclipped(  );
    }

    void OnTerminate( )
    {
        Menu::Root->Release(  );
    }

    // ░█▀▄░█▀▀░█▀█░█▀▄░█▀▀░█▀▄
    // ░█▀▄░█▀▀░█░█░█░█░█▀▀░█▀▄
    // ░▀░▀░▀▀▀░▀░▀░▀▀░░▀▀▀░▀░▀
    #pragma region Render
    
    void OnDraw( )
    {
        RenderDaggers(  );
        
        if (!GetPlayer(  )->IsDead(  ))
        {
            const auto q_opacity = GetPlayer( )->Spellbook( )->CanUseSpell( Q ) ? 255 : 25;
            if ( Menu::DrawQ->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( Q )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatQ->Range( ), 6, RGBA( 0, 0, 0, q_opacity ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatQ->Range( ), 3, RGBA( 255, 20, 255, q_opacity ) );
            }

            const auto e_opacity = GetPlayer( )->Spellbook( )->CanUseSpell( E ) ? 255 : 25;
            if ( Menu::DrawE->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( E )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatE->Range( ), 6, RGBA( 0, 0, 0, e_opacity ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatE->Range( ), 3, RGBA( 255, 20, 255, e_opacity ) );
            }

            const auto r_opacity = GetPlayer( )->Spellbook( )->CanUseSpell( R ) ? 255 : 25;
            if ( Menu::DrawR->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( R )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatR->Range( ), 6, RGBA( 0, 0, 0, r_opacity ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatR->Range( ), 3, RGBA( 255, 255, 255, r_opacity ) );
            }
        }
    }

    void OnPresentDraw( )
    {
        for ( auto i : g_pExportedEntityManager->Heroes( ) )
        {
            if ( i->IsEnemy( ) && i->IsVisibleOnScreen( ) && Menu::DrawHPBar->Enabled(  ))
            {
                auto daggers = Menu::DaggerCalc->Value( ) * GroundDaggerDmg( i, true );
                auto dmg = DaggerDmg( i ) + ShunpoDmg( i ) + daggers + UltDmg( i, Menu::Channel->Value( ) ) + IgniteDmg(  );
                i->DrawDamageIndicator( dmg, false, true );
            }
        }

        if ( !GetPlayer(  )->IsDead(  ) )
        {
            if ( Menu::Toggle->Enabled( ) )
            {
                Vector3 wts;
                g_pExportedRenderer->WorldToScreen( GetPlayer( )->Position( ), &wts );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: Q->E", 15, wts.x - 40 + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: Q->E", 15, wts.x - 40, wts.y, RGBA( 255, 20, 255, 255 ) );
            }
            else
            {
                Vector3 wts;
                g_pExportedRenderer->WorldToScreen( GetPlayer( )->Position( ), &wts );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: E->Q", 15, wts.x - 40 + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: E->Q", 15, wts.x - 40, wts.y, RGBA( 255, 20, 255, 255 ) );
            }
        }
    }
    
    #pragma endregion
    
    // ░▀█▀░▀█▀░█▀▀░█░█░█▀▀
    // ░░█░░░█░░█░░░█▀▄░▀▀█
    // ░░▀░░▀▀▀░▀▀▀░▀░▀░▀▀▀
    #pragma region Ticks
    
    void OnPreUpdate( )
    {
        if ( Spinning(  ) && Menu::UseR->Enabled(  ))
        {
            auto target = g_pExportedTargetSelector->GetTarget( KatE->Range(  ), true );
            if ( target != nullptr && CanPostExecute( target ) && GUtils->IsValidTarget( target ))
            {
                if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled(  ))
                {
                    if (Menu::Toggle->Enabled(  ) )
                    {
                        UseQ( target );
                        UseE( target, KatE->Range(  ) );
                    }
                    else
                    {
                        UseE( target, KatE->Range(  ) );
                        UseQ( target );
                    }
                }
            }

            if ( GUtils->CountEnemiesInRange( 500 ) < 1 && Menu::Cancel->Enabled( ) )
            {
                auto pos = g_pExportedHudManager->CursorPositionUnclipped( );
                auto new_pos = GetPlayer(  )->Position(  ).Extend( *pos, GetPlayer(  )->BoundingRadius(  ) + 400);
                GetPlayer( )->IssueMoveOrder( &new_pos, false, false, true, true, &GLimiter );
                
            }
        }
    }

    void OnUpdate( )
    {
        OnPreUpdate( );
        Auto( );
        Combo( );
        Harass(  );
        LastHit( );
        Flee( );
        RemoveDaggers( );
    }
    
    #pragma endregion
    
    // ░█▀▀░█░█░█▀▀░█▀▀░█░█░▀█▀░█▀▀
    // ░█▀▀░▄▀▄░█▀▀░█░░░█░█░░█░░█▀▀
    // ░▀▀▀░▀░▀░▀▀▀░▀▀▀░▀▀▀░░▀░░▀▀▀
    #pragma region Execute Logic
    
    bool CanPostExecute( GameObject* unit )
    {
        auto dmg = DaggerDmg( unit ) + ShunpoDmg( unit );

        for ( auto a : Daggers )
        {
            auto dagger = a.Obj;
            if ( dagger->DistanceXZ( unit ) <= KatW->Range( ) + unit->BoundingRadius( ) )
            {
                if ( KatE->IsReady( ) )
                {
                    if ( dagger->DistanceXZ( GetPlayer( ) ) <= KatE->Range( ) + KatW->Range( ) )
                    {
                        dmg += GroundDaggerDmg( unit );
                        break;
                    }
                }
                else
                {
                    if ( dagger->DistanceXZ( GetPlayer( ) ) <= KatW->Range( ) - 115 )
                    {
                        dmg += GroundDaggerDmg( unit );
                        break;
                    }
                }
            }
        }

        return dmg >= unit->Health( );
    }
    
    bool CanPreExecute( GameObject* unit )
    {
        auto daggers = Menu::DaggerCalc->Value( ) * GroundDaggerDmg( unit, true );
        auto dmg = UltDmg( unit, Menu::Channel->Value( ) ) + DaggerDmg( unit ) + ShunpoDmg( unit ) + IgniteDmg(  );

        return daggers + dmg >= unit->Health( );
    }

    #pragma endregion

    // ░█▄█░█▀█░█▀▄░█▀▀░█▀▀
    // ░█░█░█░█░█░█░█▀▀░▀▀█
    // ░▀░▀░▀▀▀░▀▀░░▀▀▀░▀▀▀
    #pragma region  Modes
    
    void Auto( )
    {
        if ( GUtils->CountEnemiesInRange( 550 ) >= Menu::AutoR->Value(  ) )  
        {
            if ( Menu::UseR->Enabled(  ) &&  KatR->IsReady(  ) && !Spinning(  ))
            {
                (KatW->IsReady( ) ? KatW : KatR)->Cast( );
            }
        }
    }
    
    void Combo( )
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled( ) )
        {
            if ( !Spinning(  ) )
            {
                if (Menu::Toggle->Enabled(  ))
                {
                    auto range = KatQ->IsReady(  ) ? KatQ->Range(  ) : KatE->Range(  );
                    auto target = g_pExportedTargetSelector->GetTarget(range, true  );

                    UseProtoBelt( range );
                    UseQ( target );
                    GapE( );
                    UseE( target, range );
                    UseW( target );
                    UseR( target );
                    UseIgnite( target );
                }
                else
                {
                    auto range = KatE->Range(  );
                    auto target = g_pExportedTargetSelector->GetTarget(range, true  );
                    
                    UseProtoBelt( range );
                    GapE( );
                    UseE( target, range );
                    UseQ( target );
                    UseW( target );
                    UseR( target );
                    UseIgnite( target );
                }
            }
        }
    }

    void Harass( )
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Harass )->Enabled( ) )
        {
            // - dont harass under turret
            if ( !GUtils->UnderEnemyTurret( ) )
            {
                if ( !Spinning(  ) )
                {
                    UseQ( g_pExportedTargetSelector->GetTarget(KatQ->Range(  ), true  ) );
                    UseW( g_pExportedTargetSelector->GetTarget(KatW->Range(  ), true  ) );
                }
            }
        }

        if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::LaneClear )->Enabled( ))
        {
            // - dont harass under turret
            if ( !GUtils->UnderEnemyTurret( ) )
            {
                if ( !Spinning(  ) )
                {
                    UseQ( g_pExportedTargetSelector->GetTarget(KatQ->Range(  ), true  ) );
                    UseW( g_pExportedTargetSelector->GetTarget(KatW->Range(  ), true  ) );
                }
            }
        }
    }

    void LastHit()
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::LastHit )->Enabled( ) )
        {
            if ( Menu::UseQ->Enabled( ) && Menu::LastHitQ->Enabled( ) && KatQ->IsReady( ) )
            {
                for ( auto i : g_pExportedEntityManager->Minions( ) )
                {
                    if ( GUtils->IsValidTarget( i, KatQ->Range(  ) ) && DaggerDmg( i ) >= i->Health( ) )
                    {
                        KatQ->Cast( i );
                    }
                }
            }
        }

        if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::LaneClear )->Enabled( ))
        {
            if ( Menu::UseQ->Enabled( ) && Menu::LastHitQ->Enabled( ) && KatQ->IsReady( ) )
            {
                for ( auto i : g_pExportedEntityManager->Minions( ) )
                {
                    if ( GUtils->IsValidTarget( i, KatQ->Range(  ) ) && DaggerDmg( i ) >= i->Health( ) )
                    {
                        KatQ->Cast( i );
                    }
                }
            }
        }
    }

    void Flee( )
    {
        if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::Flee )->Enabled(  ))
        {
            // - shunpo
            if ( Menu::FleeE->Enabled(  ) && KatE->IsReady(  ) )
            {
                for ( auto t : ShunpoJumpList(  ) )
                {
                    auto pos = ShunpoPosition( t );
                    if ( pos.IsValid( ) && GetPlayer( )->DistanceXZ( pos ) > KatW->Range( ) )
                    {
                        if ( GetPlayer( )->DistanceXZ( pos ) <= KatE->Range( ) && Cursor->DistanceXZ( pos ) <= KatW->Range( ) )
                        {
                            KatE->Cast( pos );
                        }
                    }
                }
            }

            // - preparation
            if ( KatW->IsReady( ) && Menu::FleeW->Enabled(  ) )
            {
                KatW->Cast( );
            }
        }
    }

    #pragma endregion
    
    // ░█▀▀░█▀█░█▄█░█▀▄░█▀█░▀█▀
    // ░█░░░█░█░█░█░█▀▄░█▀█░░█░
    // ░▀▀▀░▀▀▀░▀░▀░▀▀░░▀░▀░░▀░
    #pragma region Combat
    
    void UseQ( GameObject* unit )
    {
        // - bounding blades
        if ( KatQ->IsReady( ) && Menu::UseQ->Enabled( ) )
        {
            if ( unit != nullptr && GUtils->IsValidTarget( unit, KatQ->Range(  ) ) )
            {
                KatQ->Cast( unit ) ;
            }
        }
    }

    void UseW( GameObject* unit )
    {
        // - preparation
        if ( KatW->IsReady( ) && Menu::UseW->Enabled( ) )
        {
            if ( unit != nullptr && GUtils->IsValidTarget( unit, KatW->Range(  ) ) )
            {
                if ( !KatQ->IsReady( ) || !Menu::UseQ->Enabled(  ) )
                {
                    KatW->Cast( );
                }
            }
        }
    }
    
    void UseE( GameObject* unit, float range )
    {
        // - shunpo target
        if ( KatE->IsReady( ) && Menu::UseE->Enabled( ) )
        {
            if ( unit != nullptr && GUtils->IsValidTarget( unit, range ) )
            {
                auto pos = ShunpoPosition( unit );
                if ( pos.IsValid( ) && ( !GUtils->UnderEnemyTurret( pos ) || Menu::DiveE->Enabled( ) || CanPostExecute( unit ) ) )
                {
                    if ( CanPostExecute( unit ) || GetPlayer( )->DistanceXZ( pos ) > GetPlayer( )->CharacterIntermediate( )->AttackRange( ) + 15 )
                    {
                        KatE->Cast( pos );
                    }
                }
            }
        }
    }

    void UseR( GameObject* unit )
    {
        // - death lotus
        if ( KatR->IsReady( ) && Menu::UseR->Enabled( ) )
        {
            // - check validity
            if ( unit == nullptr || !GUtils->IsValidTarget( unit, KatR->Range( ) ) )
                return;

            // - ult if killable or always
            if ( Menu::RKillable->Enabled(  ) && CanPreExecute( unit ) || !Menu::RKillable->Enabled(  ) )
            {
                // - pre cast w (if available)
                ( KatW->IsReady( ) ? KatW : KatR )->Cast( );
            }
        }
    }

    void GapE( )
    {
        // - shunpo gap
        if ( KatE->IsReady( ) && Menu::UseE->Enabled( ) && Menu::GapE->Enabled( ) )
        {
            auto unit = g_pExportedTargetSelector->GetTarget( KatE->Range(  ) *2, true );
            if ( unit != nullptr  )
            {
                const auto pos = ShunpoPosition( unit, false );
                if ( pos.IsValid( ) && ( !GUtils->UnderEnemyTurret( pos ) || Menu::DiveE->Enabled( ) || CanPostExecute( unit ) ) )
                {
                    if ( GetPlayer( )->DistanceXZ( pos ) > KatW->Range(  ) )
                    {
                        if (pos.DistanceXZ( GetPlayer(  )->Position(  ) ) <= KatE->Range(  ))
                        {
                            KatE->Cast( pos );
                        }
                    }
                }
            }
        }  
    }
    
    void UseIgnite( GameObject* unit )
    {
        if ( Ignite != nullptr && Ignite->IsReady(  ) && Menu::UseIgnite->Enabled(  ))
        {
            if ( unit != nullptr && GUtils->IsValidTarget( unit, 600 ) )
            {
                if ( CanPreExecute( unit ) )
                {
                    Ignite->Cast( unit );
                }
            }
        }
    }

    void UseProtoBelt( float range )
    {
        if (GetPlayer(  )->HasItem( ItemID::HextechProtobelt01 ) && Menu::UseItems->Enabled(  ))
        {
            auto pSlot = GetPlayer(  )->GetItemSlot( ItemID::HextechProtobelt01 );
            if ( GetPlayer(  )->Spellbook(  )->CanUseSpell( pSlot ) )
            {
                auto unit = g_pExportedTargetSelector->GetTarget( range + 275 );
                if ( unit != nullptr && GUtils->IsValidTarget( unit, range + 275  ) )
                {
                    auto pos = unit->Position(  );
                    GetPlayer(  )->Spellbook(  )->SendSpellCastPacket( pSlot, &pos, &pos, nullptr);
                }
            }
        }
    }
    
    #pragma endregion
    
    // ░█▀▄░█▀█░█▀▀░█▀▀░█▀▀░█▀▄░█▀▀
    // ░█░█░█▀█░█░█░█░█░█▀▀░█▀▄░▀▀█
    // ░▀▀░░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀
    #pragma region Daggers
    
    void OnCreateParticle( GameObject* pObject, std::uint32_t iHash )
    {
        if ( pObject != nullptr )
            if ( iHash == FNV1A32CI( "Katarina_Dagger_Ground_Indicator" ) ) 
                Daggers.push_back( { .Obj = pObject, .Position = pObject->Position( ), .CreateTime = g_pExportedGlobalClocks->GameTime( ), } );
    }
    
    void RenderDaggers()
    {
        for ( auto i : Daggers )
        {
            if ( Menu::DrawDagger->Enabled( ) )
            {
                Vector3 wts;
                g_pExportedRenderer->WorldToScreen( i.Position, &wts );
                double time = 5.0 - ( g_pExportedGlobalClocks->GameTime( ) - i.CreateTime );

                char buffer[64];
                Globals::Sprintf( buffer, "%2.f", time );

                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatW->Range( ), 4, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatW->Range( ), 2, RGBA( 255, 20, 255, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatW->Range( ) / 2 + 2, 2, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatW->Range( ) / 2, 2, RGBA( 255, 20, 255, 255 ) );
                
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, buffer, 75, wts.x + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, buffer, 75, wts.x, wts.y, RGBA( 255, 20, 255, 255 ) );
            }
        }
    }

    void RemoveDaggers( )
    {
        // - dagger handling
        for ( auto it = Daggers.begin( ); it != Daggers.end( ); it++ )
        {
            const auto blade = it->Obj;

            // - remove invalid? blades
            if ( blade == nullptr || !blade->IsActive( ) || !blade->IsVisible( ) )
            {
                Daggers.erase( it );
                break;
            }

            // - remove blades ive picked up
            if ( blade->Position( ).DistanceXZ( GetPlayer( )->Position( ) ) <= KatW->Range( ) - 115 )
            {
                Daggers.erase( it );
                break;
            }

            // - remove blade after 4 seconds
            if ( g_pExportedGlobalClocks->GameTime( ) - it->CreateTime > 4.0f )
            {
                Daggers.erase( it );
                break;
            }
        }
    }

    #pragma endregion
    
    // ░█▄█░▀█▀░█▀▀░█▀▀
    // ░█░█░░█░░▀▀█░█░░
    // ░▀░▀░▀▀▀░▀▀▀░▀▀▀
    #pragma region Misc
    
    Vector<GameObject*> ShunpoJumpList()
    {
        // create an empty vector to store the GameObject pointers.
        auto jump_list = Vector<GameObject*>( );

        // iterate through all hero entities in the game and add them to the jump_list.
        for ( auto i : g_pExportedEntityManager->Heroes( ) )
            jump_list.push_back( i );

        // iterate through all hero entities in the game and add them to the jump_list.
        for ( auto i : g_pExportedEntityManager->Minions( ) )
            jump_list.push_back( i );
        
        // sort the jump_list based on the distance from each GameObject to the Cursor's position.
        jump_list.sort( [&]( GameObject* a, GameObject* b ) { return a->DistanceXZ( *Cursor ) < b->DistanceXZ( *Cursor ); } );

        // return the sorted jump_list.
        return jump_list;
    }
    
    Vector3 ShunpoPosition( GameObject* pObject, bool hero )
    {
        if ( pObject == nullptr )
        {
            return {};
        }
        
        for ( auto& d : Daggers )
        {
            auto blade = d.Obj;

            if ( pObject->Position(  ).DistanceXZ( blade->Position(  ) ) <= KatW->Range( ) + pObject->BoundingRadius( ) )
            {
                if ( blade->Position(  ).DistanceXZ( GetPlayer( )->Position(  ) ) <= KatE->Range( ) + KatW->Range( ) )
                {
                    if ( CanPostExecute( pObject ) || blade->Position(  ).DistanceXZ( GetPlayer( )->Position(  ) ) > KatW->Range( ) - 115 )
                    {
                        return blade->Position( );
                    }
                }
            }

            if ( pObject->Position(  ).DistanceXZ( blade->Position(  ) ) <= KatE->Range( ) + pObject->BoundingRadius( ) )
            {
                if ( blade->Position(  ).DistanceXZ( GetPlayer( )->Position(  ) ) <= KatE->Range( ) + KatW->Range( ) )
                {
                    if ( CanPostExecute( pObject ) || blade->Position(  ).DistanceXZ( GetPlayer( )->Position(  ) ) > KatW->Range( ) + 115)
                    {
                        return blade->Position( );
                    }
                }
            }
        }
        
        Vector3 pos = { };
        
        if (hero)
        {
            switch ( Menu::ShunpoMode->SelectedItem( ) )
            {
                case 0:
                    pos = pObject->Position( ) + ( GetPlayer( )->Position( ) - pObject->Position( ) ).NormalizeXZ( ) * 135;
                    break;
                case 1:
                    pos = pObject->Position( ) + ( GetPlayer( )->Position( ) - pObject->Position( ) ).NormalizeXZ( ) * -135;
                    break;
                case 2:
                    PredictionInput input = { };
                    input.m_flDelay = g_pExportedNetClient->RoundTripLatency( ) / 1000;

                    auto pred = g_pExportedPrediction->CalculateIntercept( pObject, input );
                    auto walk_to = pObject->Position( ).Extend( pred, 100 );
                    auto reverse = pObject->Position( ) + ( walk_to - pObject->Position( ) ).NormalizeXZ( ) * -135;
                    pos = reverse;
                    break;
            }
        }
        
        return pos;
    }

    float DynamicRange( )
    {
        if ( !KatE->IsReady( ) )
        {
            if ( g_pExportedGlobalClocks->GameTime( ) - KatE->LastCastTime( ) < 0.5 )
            {
                return KatW->Range( );
            }
        }
        
        return KatQ->Range(  );
    }

    bool Spinning( )
    {
        return GetPlayer( )->HasBuff( FNV1A32CI( "katarinarsound" ) );
    }
    
    #pragma endregion
    
    // ░█▀▄░█▀█░█▄█░█▀█░█▀▀░█▀▀
    // ░█░█░█▀█░█░█░█▀█░█░█░█▀▀
    // ░▀▀░░▀░▀░▀░▀░▀░▀░▀▀▀░▀▀▀
    #pragma region Damage
    
    float UltDmg( GameObject* pObject, float time )
    {
        auto p_dmg = 0;
        auto m_dmg = 0;

        if ( !KatR->IsReady( ) )
            return p_dmg + m_dmg;

        // constants for base damage, bonus AD scaling, AP scaling, and on-attack/on-hit effectiveness
        constexpr double base_phys_damage = 0.16;
        constexpr double base_magic_damage[] = { 25.0, 37.5, 50.0 };
        constexpr double on_hit_effectiveness[] = { 0.25, 0.35, 0.4 };
        constexpr double ap_scaling = 0.18;

        // number of daggers thrown during the channeling
        int num_daggers = static_cast<int>(time / 0.166);
        
        auto bonus_ad = GetPlayer( )->CharacterIntermediate( )->FlatPhysicalDamageMod( );
        auto bonus_as = GetPlayer( )->CharacterIntermediate( )->AttackSpeedMod( );
        auto total_ap = GetPlayer( )->TotalAbilityDamage( );

        // calculate the damage for each dagger
        for ( int i = 0; i < num_daggers; i++ )
        {
            double physical_damage = base_phys_damage * ( bonus_ad * ( 1.0 + bonus_as * 0.5 ) );
            double magic_damage = base_magic_damage[GetPlayer( )->Spellbook( )->GetSpell( R )->Level( ) - 1] + total_ap * ap_scaling;
            auto efx = on_hit_effectiveness[GetPlayer( )->Spellbook( )->GetSpell( R )->Level( ) - 1];

            if ( GetPlayer( )->HasItem( ItemID::NashorsTooth ) )
            {
                auto nt = 15.0 + total_ap * 0.20;
                magic_damage += nt * efx;
            }

            if ( GetPlayer( )->HasItem( ItemID::WitsEnd ) )
            {
                auto we = 15 + ( ( 80 - 15 ) / 17.0 ) * ( min( 18, GetPlayer( )->Experience( )->Level( ) ) - 1 );
                magic_damage += we * efx;
            }

            p_dmg += physical_damage;
            m_dmg += magic_damage;
        }

        auto fm_dmg = g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), pObject, m_dmg ).m_flMagical;
        auto fp_dmg = g_pExportedDamage->CalculatePhysicalDamage( GetPlayer( ), pObject, p_dmg ).m_flPhysical;
        auto total = fp_dmg + fm_dmg;
        
        return total;
    }

    float DaggerDmg( GameObject* pObject )
    {
        if ( !KatQ->IsReady( ) )
            return 0;

        // constants for base damage and AP scaling
        constexpr double base_damage[] = { 80.0, 110.0, 140.0, 170.0, 200.0 };
        constexpr double ap_scaling = 0.35;

        const double magic_damage = base_damage[GetPlayer( )->Spellbook( )->GetSpell( Q )->Level( ) - 1] +
            ( ap_scaling * GetPlayer( )->TotalAbilityDamage( ) );

        auto dmg = g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), pObject, magic_damage ).m_flTotal;  
        return dmg; // NOLINT(bugprone-narrowing-conversions)
    }

    float IgniteDmg()
    {
        if ( Ignite != nullptr && Ignite->IsReady(  ) && Menu::UseIgnite->Enabled(  ))
        {
            // constants
            const auto lvl = GetPlayer( )->Experience( )->Level( );
            constexpr double base_value_min = 70.0;
            constexpr double base_value_max = 410.0;

            // calculate the value based on the level
            return base_value_min + ( ( base_value_max - base_value_min ) / 17.0 ) * ( lvl - 1 );
        }
        return 0;
    }

    float GroundDaggerDmg( GameObject* pObject, bool draw )
    {
        if ( Daggers.size( ) > 0 || draw )
        {
            const auto lvl = GetPlayer( )->Experience( )->Level( );
            constexpr double base_damage_min = 68.0;
            constexpr double base_damage_max = 240.0;
            constexpr double bonus_ad_scaling = 0.60;
            constexpr double ap_scaling_min = 0.70;
            constexpr double ap_scaling_max = 1.00;
            
            double base_damage = base_damage_min + ((base_damage_max - base_damage_min) / 17.0) * (lvl - 1);
            double damage = base_damage + (GetPlayer( )->CharacterIntermediate( )->FlatPhysicalDamageMod( ) * bonus_ad_scaling) +
                ((ap_scaling_min + ((ap_scaling_max - ap_scaling_min) / 17.0) * (lvl - 1)) * GetPlayer( )->TotalAbilityDamage( ));

            auto dmg = g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), pObject, damage ).m_flTotal;
            return dmg;
        }
        
        return 0;
    }

    float ShunpoDmg( GameObject* pObject )
    {
        if ( !KatE->IsReady( ) )
            return 0;

        // constants for base damage, AD scaling, and AP scaling
        constexpr double base_damage[] = { 20.0, 35.0, 50.0, 65.0, 80.0 };
        constexpr double ad_scaling = 0.4;
        constexpr double ap_scaling = 0.25;

        // calculate the magic damage
        const double magic_damage = base_damage[GetPlayer( )->Spellbook( )->GetSpell( E )->Level( ) - 1]
            + ( ad_scaling * GetPlayer( )->TotalAttackDamage( ) ) + ( ap_scaling * GetPlayer( )->TotalAbilityDamage( ) );

        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), pObject, magic_damage ).m_flTotal;
    }
    
#pragma endregion
    
    // ░█▄█░█▀▀░█▀█░█░█
    // ░█░█░█▀▀░█░█░█░█
    // ░▀░▀░▀▀▀░▀░▀░▀▀▀
    #pragma region Menu
    
    void SetupMenu( )
    {
        Menu::Root = g_pExportedMenu->AddMenu("EzKatarina", MenuConfig("EzSeries"));
        Menu::Root->AddSeparator( MenuString( "EzSeries - Katarina" ) );
        Menu::UseIgnite = Menu::Root->AddCheckbox( MenuString( "[Summoner] Ignite" ), MenuConfig( "katarina.use.ignite" ), true );
        
        const auto q_menu = Menu::Root->AddMenu( MenuString( "[Q] Bouncing Blade" ), MenuConfig( "BouncingBlade" ) );
        Menu::UseQ = q_menu->AddCheckbox( MenuString( "Use [Q]" ), MenuConfig( "katarina.use.q" ), true );
        Menu::LastHitQ = q_menu->AddCheckbox( MenuString( "- Last Hit" ), MenuConfig( "katarina.last.hit.q" ), true );
        Menu::DaggerCalc = q_menu->AddSlider( MenuString( "- Daggers [Dmg Calc]" ), MenuConfig( "katarina.q.daggers" ), 0, 3, 2, 0, 1 );
        Menu::DaggerCalc->SetTooltipName( TooltipString( "Calculates as if # daggers near target." ) );
        Menu::DrawDagger = q_menu->AddCheckbox( MenuString( "- Draw Dagger Lifetime" ), MenuConfig( "katarina.dagger.life" ), true );
        Menu::DrawQ = q_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "katarina.q.draw" ), true );

        const auto w_menu = Menu::Root->AddMenu( MenuString( "[W] Preparation" ), MenuConfig( "Preparation" ) );
        Menu::UseW = w_menu->AddCheckbox( MenuString( "Use [W]" ), MenuConfig( "katarina.use.w" ), true );
        Menu::FleeW = w_menu->AddCheckbox( MenuString( "- Flee" ), MenuConfig( "katarina.use.w.flee" ), true );

        const auto e_menu = Menu::Root->AddMenu( MenuString( "[E] Shunpo" ), MenuConfig( "Shunpo" ) );
        Menu::UseE = e_menu->AddCheckbox( MenuString( "Use [E]" ), MenuConfig( "katarina.use.e" ), true );
        Menu::GapE = e_menu->AddCheckbox( MenuString( "- Gapclose [Dagger]" ), MenuConfig( "katarina.use.e.gap" ), false );
        Menu::GapE->SetTooltipName( TooltipString( "Shunpo to a dagger for distance not damage." ) );
        Menu::FleeE = e_menu->AddCheckbox( MenuString( "- Flee" ), MenuConfig( "katarina.use.e.flee" ), true );
        
        Vector<CompileTimeString<char, 64>> shunpo_mode_items;
        shunpo_mode_items.push_back( MenuString( "Front" ) );
        shunpo_mode_items.push_back( MenuString( "Behind" ) );
        shunpo_mode_items.push_back( MenuString( "Auto" ) );
        Menu::ShunpoMode = e_menu->AddDropdown( MenuString( "- Position:" ), MenuConfig( "katarina.e.where" ), shunpo_mode_items, 0 );

        Menu::DiveE = e_menu->AddKeybind( MenuString( "- Turret Dive" ), MenuConfig( "katarina.dive.e" ), 'T', true, true );
        Menu::DrawE = e_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "katarina.e.draw" ), true );

        const auto r_menu = Menu::Root->AddMenu( MenuString( "[R] Death Lotus" ), MenuConfig( "DeathLotus" ) );
        Menu::UseR = r_menu->AddCheckbox( MenuString( "Use [R]" ), MenuConfig( "katarina.use.r" ), true );
        Menu::RKillable = r_menu->AddCheckbox( MenuString( "- Only if Can Kill" ), MenuConfig( "katarina.r.killable" ), false );
        
        Menu::AutoR = r_menu->AddSlider( MenuString( "- Auto use if # in Range" ), MenuConfig( "katarina.auto.r" ), 1, 6, 3, 0, 1 );
        Menu::Channel = r_menu->AddSlider( MenuString( "- Channel Time [Dmg Calc]" ), MenuConfig( "katarina.r.channel" ), 0.0, 2.5, 2.0, 1, .1 );
        Menu::Cancel = r_menu->AddCheckbox( MenuString( "- Cancel if None in Range" ), MenuConfig( "katarina.use.r.cancel" ), true );
        Menu::DrawR = r_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "katarina.r.draw" ), true );
        
        auto misc = Menu::Root->AddMenu( MenuString("[Other] Misc"), MenuConfig( "Misc.Other" ) );
  
        Menu::UseItems = misc->AddCheckbox( MenuString( "Rocketbelt" ), MenuConfig( "katarina.use.items" ), true );
        
        Vector<CompileTimeString<char, 64>> spell_priority;
        spell_priority.push_back( MenuString( "E -> Q" ) );
        spell_priority.push_back( MenuString( "Q -> E" ) );
        Menu::Toggle = misc->AddKeybind( MenuString( "Combo Toggle" ), MenuConfig( "katarina.toggle" ), 'X', true );
        Menu::DrawHPBar = misc->AddCheckbox( MenuString( "HPBarFill Draw" ), MenuConfig( "katarina.r.draw.hp" ), true );
        
        Menu::Root->AddSeparator( MenuString( "EzSeries v0.61" ) );
    }

#pragma endregion
}
