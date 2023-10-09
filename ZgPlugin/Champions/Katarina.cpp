#include "../stdafx.hpp"

namespace Katarina
{
    void OnBoot()
    {
        Menu::Root = g_pExportedMenu->AddMenu("EzSeries", MenuConfig("EzSeries"));
        Menu::Root->AddSeparator( MenuString( "Katarina" ) );

        const auto q_menu = Menu::Root->AddMenu( MenuString( "Bouncing Blade (Q)" ), MenuConfig( "BouncingBlade" ) );
        Menu::UseQ = q_menu->AddCheckbox( MenuString( "Use Bouncing Blade (Q)" ), MenuConfig( "katarina.use.q" ), true );
        Menu::LastHitQ = q_menu->AddCheckbox( MenuString( "- Last Hit" ), MenuConfig( "katarina.last.hit.q" ), true );
        Menu::DaggerCalc = q_menu->AddSlider( MenuString( "- Daggers (Dmg Calc)" ), MenuConfig( "katarina.q.daggers" ), 0, 3, 2, 0, 1 );
        Menu::DaggerCalc->SetTooltipName( TooltipString( "Calculates as if # daggers near target." ) );
        Menu::DrawDagger = q_menu->AddCheckbox( MenuString( "- Draw Dagger Lifetime" ), MenuConfig( "katarina.dagger.life" ), true );
        Menu::DrawQ = q_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "katarina.q.draw" ), true );

        const auto w_menu = Menu::Root->AddMenu( MenuString( "Preparation (W)" ), MenuConfig( "Preparation" ) );
        Menu::UseW = w_menu->AddCheckbox( MenuString( "Use Preparation (W)" ), MenuConfig( "katarina.use.w" ), true );
        Menu::FleeW = w_menu->AddCheckbox( MenuString( "- Flee" ), MenuConfig( "katarina.use.w.flee" ), true );

        const auto e_menu = Menu::Root->AddMenu( MenuString( "Shunpo (E)" ), MenuConfig( "Shunpo" ) );
        Menu::UseE = e_menu->AddCheckbox( MenuString( "Use Shunpo (E)" ), MenuConfig( "katarina.use.e" ), true );
        Menu::FleeE = e_menu->AddCheckbox( MenuString( "- Flee" ), MenuConfig( "katarina.use.e.flee" ), true );
        Menu::FleeE->SetTooltipName( TooltipString( "Soon" ) );
        
        Vector<CompileTimeString<char, 64>> shunpo_mode_items;
        shunpo_mode_items.push_back( MenuString( "Front" ) );
        shunpo_mode_items.push_back( MenuString( "Behind" ) );
        shunpo_mode_items.push_back( MenuString( "Auto (Beta)" ) );
        Menu::ShunpoMode = e_menu->AddDropdown( MenuString( "- Position:" ), MenuConfig( "katarina.e.where" ), shunpo_mode_items, 0 );

        Menu::DiveE = e_menu->AddKeybind( MenuString( "- Turret Dive" ), MenuConfig( "katarina.dive.e" ), 'T', true, true );
        Menu::DrawE = e_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "katarina.e.draw" ), true );
        //Menu::FleeE = menu->AddCheckbox( MenuString( "- Use In Flee" ), MenuConfig( "katarina.use.flee.e" ), true );

        const auto r_menu = Menu::Root->AddMenu( MenuString( "Death Lotus (R)" ), MenuConfig( "DeathLotus" ) );
        Menu::UseR = r_menu->AddCheckbox( MenuString( "Use Death Lotus (R)" ), MenuConfig( "katarina.use.r" ), true );

        Vector<CompileTimeString<char, 64>> ult_mode_items;
        ult_mode_items.push_back( MenuString( "Always" ) );
        ult_mode_items.push_back( MenuString( "Killable" ) );
        Menu::UltMode = r_menu->AddDropdown( MenuString( "- Use When:" ), MenuConfig( "katarina.r.when" ), ult_mode_items, 0 );

        Menu::AutoR = r_menu->AddSlider( MenuString( "- Auto use if # in Range" ), MenuConfig( "katarina.auto.r" ), 1, 6, 3, 0, 1 );
        Menu::Channel = r_menu->AddSlider( MenuString( "- Channel Time (Dmg Calc)" ), MenuConfig( "katarina.r.channel" ), 0.0, 2.5, 2.0, 1, .1 );
        Menu::Cancel = r_menu->AddCheckbox( MenuString( "- Cancel if None in Range" ), MenuConfig( "katarina.use.r.cancel" ), true );
        Menu::DrawR = r_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "katarina.r.draw" ), true );

        Menu::Root->AddSeparator( MenuString( "Mechanics" ) );
        Menu::UseIgnite = Menu::Root->AddCheckbox( MenuString( "Use Ignite" ), MenuConfig( "katarina.use.ignite" ), true );
        Menu::UseItems = Menu::Root->AddCheckbox( MenuString( "Use Rocketbelt" ), MenuConfig( "katarina.use.items" ), true );
        //Menu::Killsteal = Menu::Root->AddCheckbox( MenuString( "Killsteal" ), MenuConfig( "katarina.ks" ), false );
        //Menu::Killsteal->SetTooltipName( TooltipString( "Soon" ) );
        
        Vector<CompileTimeString<char, 64>> spell_priority;
        spell_priority.push_back( MenuString( "E -> Q" ) );
        spell_priority.push_back( MenuString( "Q -> E" ) );
        Menu::Toggle = Menu::Root->AddKeybind( MenuString( "Combo Priority" ), MenuConfig( "katarina.toggle" ), 'X', true );
        Menu::DrawHPBar = Menu::Root->AddCheckbox( MenuString( "HPBarFill Draw" ), MenuConfig( "katarina.r.draw.hp" ), true );
        Menu::Root->AddSeparator( MenuString( "EzSeries v0.49" ) );

        if (GetPlayer(  )->Spellbook(  )->GetSpell( Summoner1 )->SpellData(  )->Hash(  ) == FNV1A32CI("SummonerDot"))
            Ignite = new Spell ( Summoner1, 600);
        
        if (GetPlayer(  )->Spellbook(  )->GetSpell( Summoner2 )->SpellData(  )->Hash(  ) == FNV1A32CI("SummonerDot"))
            Ignite = new Spell ( Summoner2, 600);

        KatarinaQ = new Spell( Q, 625 );
        KatarinaW = new Spell( W, 325 );
        KatarinaE = new Spell( E, 725 );
        KatarinaR = new Spell( R, 550 );

        Cursor = g_pExportedHudManager->CursorPositionUnclipped( );
    }

    void OnTerminate( )
    {
        Menu::Root->Release(  );
    }
    
    void OnDraw( )
    {
        if (GetPlayer(  )->IsDead(  ))
            return;
        
        const auto q_opacity = GetPlayer(  )->Spellbook(  )->CanUseSpell( Q ) ? 255 : 25;
        if ( Menu::DrawQ->Enabled( ) && GetPlayer(  )->Spellbook(  )->GetSpell( Q )->Level(  ) > 0 )
        {
            g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatarinaQ->Range( ), 6, RGBA( 0, 0, 0, q_opacity ) );
            g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatarinaQ->Range( ), 3, RGBA( 255, 51, 204, q_opacity ) );
        }

        const auto e_opacity = GetPlayer(  )->Spellbook(  )->CanUseSpell( E ) ? 255 : 25;
        if ( Menu::DrawE->Enabled( ) && GetPlayer(  )->Spellbook(  )->GetSpell( E )->Level(  ) > 0 )
        {
            g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatarinaE->Range( ), 6, RGBA( 0, 0, 0, e_opacity ) );
            g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatarinaE->Range( ), 3, RGBA( 255, 51, 204, e_opacity ) );
        }

        const auto r_opacity = GetPlayer(  )->Spellbook(  )->CanUseSpell( R ) ? 255 : 25;
        if ( Menu::DrawR->Enabled( ) && GetPlayer(  )->Spellbook(  )->GetSpell( R )->Level(  ) > 0 )
        {
            g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatarinaR->Range( ), 6, RGBA( 0, 0, 0, r_opacity ) );
            g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), KatarinaR->Range( ), 3, RGBA( 255, 255, 255, r_opacity ) );
        }
        
        for ( auto i : Daggers )
        {
            if ( Menu::DrawDagger->Enabled( ) )
            {
                Vector3 wts;
                g_pExportedRenderer->WorldToScreen( i.Position, &wts );
                double time_left = 5.0 - ( g_pExportedGlobalClocks->GameTime( ) - i.CreateTime );

                char Buffer[64];
                Globals::Sprintf( Buffer, "%2.f", time_left );

                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatarinaW->Range( ), 4, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatarinaW->Range( ), 2, RGBA( 255, 51, 204, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatarinaW->Range( ) / 2 + 2, 2, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( i.Position, KatarinaW->Range( ) / 2, 2, RGBA( 255, 51, 204, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, Buffer, 75, wts.x + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, Buffer, 75, wts.x, wts.y, RGBA( 255, 51, 204, 255 ) );
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

        if (GetPlayer(  )->IsDead(  ))
            return;

        if (Menu::Toggle->Enabled(  ))
        {
            Vector3 wts;
            g_pExportedRenderer->WorldToScreen( GetPlayer(  )->Position(  ), &wts );
            g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: Q->E", 15, wts.x - 40 + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
            g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: Q->E", 15, wts.x - 40, wts.y, RGBA( 255, 51, 204, 255 ) );
        }
        else
        {
            Vector3 wts;
            g_pExportedRenderer->WorldToScreen( GetPlayer(  )->Position(  ), &wts );
            g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: E->Q", 15, wts.x - 40 + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
            g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, "Combo: E->Q", 15, wts.x - 40, wts.y, RGBA( 255, 51, 204, 255 ) );
        }
    }

    void OnCreateParticle( GameObject* pObject, std::uint32_t iHash )
    {
        if ( pObject != nullptr )
        {
            if ( iHash == FNV1A32CI( "Katarina_Dagger_Ground_Indicator" ) )
            {
                Daggers.push_back( {.Obj = pObject, .Position = pObject->Position( ), .CreateTime = g_pExportedGlobalClocks->GameTime( ), } );
            }
        }
    }

    Vector3 ShunpoPosition( GameObject* pObject )
    {
        if ( pObject == nullptr )
        {
            return { };
        }

        for ( auto& d : Daggers )
        {
            auto blade = d.Obj;

            if ( pObject->Position( ).DistanceXZ( blade->Position( ) ) <= KatarinaW->Range( ) + pObject->BoundingRadius( ) )
            {
                if ( blade->Position( ).DistanceXZ( GetPlayer( )->Position( ) ) <= KatarinaE->Range( ) + KatarinaW->Range( ) )
                {
                    if ( blade->DistanceXZ( GetPlayer( )->Position( ) ) > KatarinaW->Range( ) - 115 || CanPostExecute( pObject ) )
                    {
                        return blade->Position( );
                    }
                }
            }
        }

        Vector3 pos;

        switch ( Menu::ShunpoMode->SelectedItem( ) )
        {
            case 0:
                pos = pObject->Position( ) + ( GetPlayer( )->Position( ) - pObject->Position( ) ).NormalizeXZ( ) * 135;
                break;
            case 1:
                pos = pObject->Position( ) + ( GetPlayer( )->Position( ) - pObject->Position( ) ).NormalizeXZ( ) * -135;
                break;
            case 2:
                auto pred = g_pExportedPrediction->CalculateIntercept( pObject, { .m_flDelay = g_pExportedNetClient->RoundTripLatency( ) / 1000 } );
                auto walk_to = pObject->Position( ).Extend( pred, 100 );
                auto reverse = pObject->Position( ) + ( walk_to - pObject->Position( ) ).NormalizeXZ( ) * -135;
                pos = reverse;
                break;
        }
        
        return pos;
    }

    void OnPreUpdate( )
    {
        if ( Spinning(  ) && Menu::UseR->Enabled(  ))
        {
            auto target = g_pExportedTargetSelector->GetTarget( KatarinaE->Range(  ), true );
            if ( target != nullptr && CanPostExecute( target ) && Utils::IsValidTarget( target ))
            {
                if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled(  ))
                {
                    if (Menu::Toggle->Enabled(  ))
                    {
                        UseQ( );
                        UseE( KatarinaQ->Range(  ) - 115);
                    }
                    else
                    {
                        UseE( KatarinaE->Range(  ) );
                        UseQ( );
                    }
                }
                
                //Globals::Write( "Cancel: Execute!\n" );
                //Globals::Write( target->Name(  )->c_str(  ) );
            }

            if ( Utils::CountEnemiesInRange( GetPlayer( ), 500 ) < 1 && Menu::Cancel->Enabled( ) )
            {
                auto pos = g_pExportedHudManager->CursorPositionUnclipped( );
                auto new_pos = GetPlayer(  )->Position(  ).Extend( *pos, GetPlayer(  )->BoundingRadius(  ) + 400);
                GetPlayer( )->IssueMoveOrder( &new_pos, false, false, true, true, &Limiter );
                //Globals::Write( "Cancel: none in range!\n" );
            }
        }
    }

    void OnUpdate( )
    {
        OnPreUpdate( );
        
        if ( Utils::CountEnemiesInRange( GetPlayer(  ), 550) >= Menu::AutoR->Value(  ) )
        {
            if ( Menu::UseR->Enabled(  ) &&  KatarinaR->IsReady(  ) )
            {
                if ( KatarinaW->IsReady( ) && !Spinning(  ) )
                {
                    KatarinaW->Cast( );
                }

                if ( KatarinaR->IsReady(  ) )
                {
                    KatarinaR->Cast( );
                }
            }
        }
        
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled( ) )
        {
            if ( !Spinning(  ) )
            {
                if (Menu::Toggle->Enabled(  ))
                {
                    UseQ( );
                    UseE( KatarinaQ->Range(  ) - 115);
                    UseW( );
                    UseR( );
                    UseIgnite(  );
                }
                else
                {
                    UseE( KatarinaE->Range(  ) );
                    UseQ( );
                    UseW( );
                    UseR( );
                    UseIgnite(  );
                }
            }
        }

        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Harass )->Enabled( ) ||
            g_pExportedOrbwalker->GetMode( OrbwalkerMode::LaneClear )->Enabled( ) )
        {
            // - dont harass under turret
            if ( !Utils::UnderEnemyTurret( ) )
            {
                if ( !Spinning(  ) )
                {
                    UseQ( );
                    UseW( );
                }
            }
        }

        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::LastHit )->Enabled( ) ||
            g_pExportedOrbwalker->GetMode( OrbwalkerMode::LaneClear )->Enabled( ) )
        {
            if ( Menu::UseQ->Enabled( ) && Menu::LastHitQ->Enabled( ) && KatarinaQ->IsReady( ) )
            {
                for ( auto i : g_pExportedEntityManager->Minions( ) )
                {
                    if ( Utils::IsValidTarget( i, KatarinaQ->Range(  ) ) && DaggerDmg( i ) >= i->Health( ) )
                    {
                        KatarinaQ->Cast( i );
                    }
                }
            }
        }

        if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::Flee )->Enabled(  ))
        {
            // - preparation
            if ( KatarinaW->IsReady( ) && Menu::UseW->Enabled( ) )
            {
                KatarinaW->Cast( );
            }

            Flee(  );
        }
        
        OnPostUpdate( );
    }

    void OnPostUpdate( )
    {
        // - dagger handling
        for ( auto it = Daggers.begin( ); it != Daggers.end( ); it++ )
        {
            const auto blade = it->Obj;

            // - remove invalid? blades
            if ( blade == nullptr || !blade->IsActive( ) || !blade->IsVisible( ) )
            {
                //Globals::Write( "dagger deleted: invalid\n" );
                Daggers.erase( it );
                break;
            }

            // - remove blades ive picked up
            if ( blade->Position( ).DistanceXZ( GetPlayer( )->Position( ) ) <= KatarinaW->Range( ) - 115 )
            {
                //Globals::Write( "dagger deleted: proximity\n" );
                Daggers.erase( it );
                break;
            }

            // - remove blade after 4 seconds
            if ( g_pExportedGlobalClocks->GameTime( ) - it->CreateTime > 4.0f )
            {
                //Globals::Write( "dagger deleted: expiry\n" );
                Daggers.erase( it );
                break;
            }
        }
    }

    float DynamicRange( )
    {
        if ( !KatarinaE->IsReady( ) )
        {
            if ( g_pExportedGlobalClocks->GameTime( ) - KatarinaE->LastCastTime( ) < 0.5 )
            {
                //Globals::Write( "TRUEE\n" );
                return KatarinaW->Range( );
            }
        }
        
        return KatarinaQ->Range(  );
    }

    // - post execute while channeling ult
    bool CanPostExecute( GameObject* pObject )
    {
        auto dmg = DaggerDmg( pObject ) + ShunpoDmg( pObject );

        for ( auto a : Daggers )
        {
            auto dagger = a.Obj;
            if ( dagger->DistanceXZ( pObject ) <= KatarinaW->Range( ) + pObject->BoundingRadius( ) )
            {
                if ( KatarinaE->IsReady( ) )
                {
                    if ( dagger->DistanceXZ( GetPlayer( ) ) <= KatarinaE->Range( ) + KatarinaW->Range( ) )
                    {
                        dmg += GroundDaggerDmg( pObject );
                        break;
                    }
                }
                else
                {
                    if ( dagger->DistanceXZ( GetPlayer( ) ) <= KatarinaW->Range( ) - 115 )
                    {
                        dmg += GroundDaggerDmg( pObject );
                        break;
                    }
                }
            }
        }

        return dmg >= pObject->Health( );
    }

    // - pre execute including everything
    bool CanPreExecute( GameObject* pObject )
    {
        auto daggers = Menu::DaggerCalc->Value( ) * GroundDaggerDmg( pObject, true );
        auto dmg = UltDmg( pObject, Menu::Channel->Value( ) ) + DaggerDmg( pObject ) + ShunpoDmg( pObject ) + IgniteDmg(  );

        return daggers + dmg >= pObject->Health( );
    }

    bool Spinning( )
    {
        return GetPlayer( )->HasBuff( FNV1A32CI( "katarinarsound" ) );
    }

    void UseQ( )
    {
        // - bounding blades
        if ( KatarinaQ->IsReady( ) && Menu::UseQ->Enabled( ) )
        {
            UseProtoBelt( DynamicRange(  ) );
            auto target = g_pExportedTargetSelector->GetTarget( DynamicRange(  ), true );
            if ( target != nullptr && Utils::IsValidTarget( target ) )
            {
                KatarinaQ->Cast( target ) ;
            }
        }
    }

    void UseW( )
    {
        // - preparation
        if ( KatarinaW->IsReady( ) && Menu::UseW->Enabled( ) )
        {
            auto target = g_pExportedTargetSelector->GetTarget( KatarinaW->Range( ), true );
            if ( target != nullptr && Utils::IsValidTarget( target ) )
            {
                if ( !KatarinaQ->IsReady( ) || !Menu::UseQ->Enabled(  ) )
                {
                    KatarinaW->Cast( );
                }
            }
        }
    }

    void UseE( float range )
    {
        if ( ! KatarinaQ->IsReady(  ) )
            range = KatarinaE->Range(  );
        
        // - shunpo target
        if ( KatarinaE->IsReady( ) && Menu::UseE->Enabled( ) )
        {
            UseProtoBelt( range );
            auto target = g_pExportedTargetSelector->GetTarget( range, true );
            if ( target != nullptr && Utils::IsValidTarget( target ) )
            {
                const auto pos = ShunpoPosition( target );
                if ( pos.IsValid( ) && ( !Utils::UnderEnemyTurret( pos ) || Menu::DiveE->Enabled( ) || CanPostExecute( target ) ) )
                {
                    if ( CanPostExecute( target ) || GetPlayer( )->DistanceXZ( pos ) > GetPlayer( )->CharacterIntermediate( )->AttackRange( ) + 125 )
                    {
                        KatarinaE->Cast( pos );
                    }
                }
            }
        }
    }

    void UseR( )
    {
        // - death lotus
        if ( KatarinaR->IsReady( ) && Menu::UseR->Enabled( ) )
        {
            auto target = g_pExportedTargetSelector->GetTarget( KatarinaR->Range( ), true );
            if ( target != nullptr && Utils::IsValidTarget( target ) )
            {
                if ( Menu::UltMode->SelectedItem( ) == 0 || Menu::UltMode->SelectedItem( ) == 1 && CanPreExecute( target ) )
                {
                    if (KatarinaW->IsReady(  ))
                    {
                        KatarinaW->Cast();
                    }
                    else
                    {
                        KatarinaR->Cast( );
                    }
                }
            }
        }
    }

    void UseIgnite( )
    {
        if ( Ignite->IsReady(  ) && Menu::UseIgnite->Enabled(  ))
        {
            auto target = g_pExportedTargetSelector->GetTarget( KatarinaR->Range( ), true );
            if ( target != nullptr && Utils::IsValidTarget( target ) )
            {
                if ( CanPreExecute( target ) )
                {
                    Ignite->Cast(target);
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
                auto target = g_pExportedTargetSelector->GetTarget( range + 275, true );
                if ( target != nullptr && Utils::IsValidTarget( target ) )
                {
                    auto pos = target->Position(  );
                    GetPlayer(  )->Spellbook(  )->SendSpellCastPacket( pSlot, &pos, &pos, nullptr);
                }
            }
        }
    }

    void Flee( )
    {
        if ( !Menu::FleeE->Enabled(  ) || !KatarinaE->IsReady(  ) )
            return;
        
        auto jump_list = Vector<GameObject*>();
        jump_list.clear(  );

        for ( auto i : g_pExportedEntityManager->Heroes(  ) )
            jump_list.push_back( i );

        for ( auto i : g_pExportedEntityManager->Minions(  ) )
            jump_list.push_back( i );

        jump_list.sort( [&]( GameObject* a, GameObject* b )
            { return a->DistanceXZ( *Cursor ) < b->DistanceXZ( *Cursor );  } );

        for ( auto t : jump_list )
        {
            auto pos = ShunpoPosition( t );
            if ( pos.IsValid(  ) && GetPlayer(  )->DistanceXZ( pos ) > KatarinaW->Range(  ))
            {
                if ( GetPlayer( )->DistanceXZ( pos ) <= KatarinaE->Range( ) )
                {
                    if ( Cursor->DistanceXZ( pos ) <= KatarinaW->Range( ) )
                    {
                        KatarinaE->Cast( pos );
                    }
                }
            }
        }
    }

    float UltDmg( GameObject* pObject, float time )
    {
        auto p_dmg = 0;
        auto m_dmg = 0;

        if ( !KatarinaR->IsReady( ) )
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
            //double physical_damage = bonus_ad * (1.0 + 0.5 * bonus_as);
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
        if ( !KatarinaQ->IsReady( ) )
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
        if ( !Ignite->IsReady(  ) || !Menu::UseIgnite->Enabled(  ))
            return 0;
        
        // constants
        const auto lvl = GetPlayer( )->Experience( )->Level( );
        constexpr double base_value_min = 70.0;
        constexpr double base_value_max = 410.0;

        // calculate the value based on the level
        return base_value_min + ((base_value_max - base_value_min) / 17.0) * (lvl - 1);
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
        if ( !KatarinaE->IsReady( ) )
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
}
