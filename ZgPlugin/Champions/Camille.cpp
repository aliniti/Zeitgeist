#include "../stdafx.hpp"

namespace Camille
{

    
    void OnBoot( )
    {
        Menu::Root = g_pExportedMenu->AddMenu( "EzSeries", MenuConfig( "EzSeries" ) );
        Menu::Root->AddSeparator( MenuString( "Camille" ) );

        const auto q_menu = Menu::Root->AddMenu( MenuString( "Precision Protocol (Q)" ), MenuConfig( "PrecisionProtocol" ) );
        Menu::UseQ = q_menu->AddCheckbox( MenuString( "Use Precision Protocol (Q)" ), MenuConfig( "camille.use.q" ), true );
        Menu::HarassQPct = q_menu->AddSlider( MenuString( "- Harass Mana (%) " ), MenuConfig( "camille.q.harass.pct" ), 0.0, 100.0, 65.0, 1, .1 );
        Menu::LastHitQ = q_menu->AddCheckbox( MenuString( "- Last Hit" ), MenuConfig( "camille.lasthit.q" ), true );
        Menu::DrawQ = q_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "camille.q.draw" ), true );

        const auto w_menu = Menu::Root->AddMenu( MenuString( "Tactical Sweep (W)" ), MenuConfig( "TacticalSweep" ) );
        Menu::UseW = w_menu->AddCheckbox( MenuString( "Use Tactical Sweep (W)" ), MenuConfig( "camille.use.w" ), true );
        Menu::HarassWPct = w_menu->AddSlider( MenuString( "- Harass Mana (%) " ), MenuConfig( "camille.w.harass.pct" ), 0.0, 100.0, 65.0, 1, .1 );

        const auto e_menu = Menu::Root->AddMenu( MenuString( "Hookshot (E)" ), MenuConfig( "Hookshot" ) );
        Menu::UseE = e_menu->AddCheckbox( MenuString( "Use Hookshot (E)" ), MenuConfig( "camille.use.e" ), true );
        
        Vector<CompileTimeString<char, 64>> prefered_spot_items;
        prefered_spot_items.push_back( MenuString( "Closest To Hero" ) );
        prefered_spot_items.push_back( MenuString( "Closest To Me" ) );
        Menu::HookSpot = e_menu->AddDropdown( MenuString( "- Preffered Spot" ), MenuConfig( "camille.e.spot" ), prefered_spot_items, 0 );
        Menu::DiveE = e_menu->AddKeybind( MenuString( "- Turret Dive" ), MenuConfig( "camille.dive.e" ), 'T', true, true );
        Menu::DrawE = e_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "camille.e.draw" ), true );

        const auto r_menu = Menu::Root->AddMenu( MenuString( "Hextech Ultimatum (R)" ), MenuConfig( "Ultimatum" ) );
        Menu::UseR = r_menu->AddCheckbox( MenuString( "Use Hextech Ultimatum (R)" ), MenuConfig( "camille.use.r" ), true );
        Menu::Difficulty = r_menu->AddSlider( MenuString( "- Weight Damage (%) " ), MenuConfig( "camille.r.difficulty" ), 100.0, 200.0, 100.0, 1, .1 );
        Menu::Difficulty->SetTooltipName( TooltipString( "Higher = Increased Predicted Damage = Ult Sooner." ) );
        Menu::Selected = r_menu->AddCheckbox( MenuString( "- Only Selected" ), MenuConfig( "camille.use.r.selected" ), false );
        Menu::DrawR = r_menu->AddCheckbox( MenuString( "- Draw Range" ), MenuConfig( "camille.r.draw" ), true );

        Menu::Root->AddSeparator( MenuString( "Mechanics" ) );
        Menu::UseIgnite = Menu::Root->AddCheckbox( MenuString( "Use Ignite" ), MenuConfig( "camille.use.ignite" ), true );
        Menu::DrawDamageBar = Menu::Root->AddCheckbox( MenuString( "HPBarFill Draw" ), MenuConfig( "camille.r.draw.hp" ), true );
        Menu::Root->AddSeparator( MenuString( "EzSeries v0.49" ) );

        if ( GetPlayer( )->Spellbook( )->GetSpell( Summoner1 )->SpellData( )->Hash( ) == FNV1A32CI( "SummonerDot" ) )
            Ignite = new Spell( Summoner1, 600 );

        if ( GetPlayer( )->Spellbook( )->GetSpell( Summoner2 )->SpellData( )->Hash( ) == FNV1A32CI( "SummonerDot" ) )
            Ignite = new Spell( Summoner2, 600 );

        CamQ = new Spell( Q, 225 );
        CamW = new Spell( W, 625 );
        CamE = new Spell( E, 975 );
        CamR = new Spell( R, 465 );

        MyRange = GetPlayer(  )->CharacterIntermediate(  )->AttackRange(  );
        Cursor = g_pExportedHudManager->CursorPositionUnclipped( );
        //Globals::Write( "Camille Loaded!\n" );
    }

    void OnTerminate( )
    {
        Menu::Root->Release( );
    }

    void OnUpdate( )
    {
        // - remove invalid danger points
        for ( auto it = AvoidList.begin( ); it != AvoidList.end( ); it++ )
        {
            auto timestamp = it->CreateTime;
            const auto emitter = it->Obj;

            if (emitter == nullptr || emitter->IsDead(  ) || !emitter->IsVisible( ) ) {
                AvoidList.erase( it );
                break; }
            

            // - remove  after 4 seconds
            if ( g_pExportedGlobalClocks->GameTime( ) - timestamp > 4.0f )
                AvoidList.erase( it );
                break;
        }

        // - combo
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled( ) )
        {
            auto w_tar = g_pExportedTargetSelector->GetTarget( CamW->Range( ), true );
            if ( w_tar != nullptr )
            {
                UseW( w_tar );
            }

            auto e_tar = g_pExportedTargetSelector->GetTarget( CamE->Range(  ) * 2, true );
            if ( e_tar != nullptr)
            {
                UseE( e_tar->Position(  ) );
            }
            
            auto r_tar = g_pExportedTargetSelector->GetTarget( CamR->Range( ), true );
            if ( r_tar != nullptr )
            {
                UseR( r_tar );
            }
        }

        // - harass
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Harass )->Enabled(  ) )
        {
            auto w_tar = g_pExportedTargetSelector->GetTarget( CamW->Range( ), true );
            if ( w_tar != nullptr )
            {
                if ( GetPlayer( )->PAR( ) / GetPlayer( )->PAR( ) * 100 > Menu::HarassWPct->Value( ) )
                {
                    UseW( w_tar );
                }
            }
        }
    }

    void OnPresentDraw( )
    {
        if ( Menu::DrawDamageBar->Enabled( ) )
        {
            for ( auto i : g_pExportedEntityManager->Heroes( ) )
            {
                if ( i->IsEnemy( ) && i->IsVisibleOnScreen( ) && Menu::DrawDamageBar->Enabled( ) )
                {
                    auto dmg = ComboDamage( i );
                    i->DrawDamageIndicator( dmg, true, false );
                }
            }
        }
    }

    void OnDraw( )
    {
        if ( !GetPlayer( )->IsDead( ) )
        {
            const auto q_opacity = CamQ->IsReady( ) ? 255 : 25;
            if ( Menu::DrawQ->Enabled( ) && CamQ->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), CamQ->Range( ), 6, RGBA( 0, 0, 0, q_opacity ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), CamQ->Range( ), 3, RGBA( 255, 51, 204, q_opacity ) );
            }

            const auto e_opacity = CamE->IsReady( ) ? 255 : 25;
            if ( Menu::DrawE->Enabled( ) && CamE->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), CamE->Range( ), 6, RGBA( 0, 0, 0, e_opacity ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), CamE->Range( ), 3, RGBA( 255, 51, 204, e_opacity ) );
            }

            const auto r_opacity = CamR->IsReady( ) ? 255 : 25;
            if ( Menu::DrawR->Enabled( ) && CamR->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), CamR->Range( ), 6, RGBA( 0, 0, 0, r_opacity ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), CamR->Range( ), 3, RGBA( 255, 51, 204, r_opacity ) );
            }
        }
    }

    void OnCreateParticle( GameObject* pObject, std::uint32_t iHash )
    {
        if ( pObject != nullptr )
        {
            if ( iHash == FNV1A32CI( "Camille_R_Indicator_Edge" ) )
                AvoidList.push_back( { .Obj = pObject, .CreateTime = g_pExportedGlobalClocks->GameTime( ), .Radius = 475  } );

            if ( iHash == FNV1A32CI( "Veigar_E_Cage_Red" ) )
                AvoidList.push_back( { .Obj = pObject, .CreateTime = g_pExportedGlobalClocks->GameTime( ), .Radius = 425 } );
        }
    }

    void OnExecuteCastFrame( GameObject* pCaster, SpellCasterClient* pCasterClient )
    {
        auto caster = Utils::Get( pCasterClient->SpellCastInfo( )->CasterHandle( ) );
        if ( caster != GetPlayer( ) || !Menu::UseQ->Enabled( ) )
        {
            return;
        }

        auto target = Utils::Get( pCasterClient->SpellCastInfo( )->TargetHandle( ) );
        if ( target != nullptr && target->HasUnitTag( Champion ) )
        {
            if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled( ) )
            {
                UseQ( target );
            }

            if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Harass )->Enabled( ) )
            {
                if ( GetPlayer( )->PAR( ) / GetPlayer( )->PAR( ) * 100 > Menu::HarassQPct->Value( ) )
                {
                    UseQ( target );
                }
            }
        }

        if ( target != nullptr && target->IsAttackable( true, false ) )
        {
            if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::LastHit )->Enabled( ) )
            {
                if ( Menu::LastHitQ->Enabled(  ) )
                {
                    for (auto i : g_pExportedEntityManager->Minions(  ))
                    {
                        if (i-> IsEnemy(  ) && Utils::IsValidTarget( i, MyRange + 100 ))
                        {
                            if (QDmg( i, HasQ2(  ) ) >= i->Health(  ))
                            {
                                UseQ( i );
                            }
                        }
                    }
                }
            }
            
            if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::LaneClear )->Enabled( ) )
            {
                if ( target->HasUnitTag( Structure ) && Utils::CountEnemiesInRange( GetPlayer(  ), 1200) < 1 )
                {
                    UseQ( target );
                }
                
                if ( target->HasUnitTag( Minion ) && Utils::CountEnemiesInRange( GetPlayer(  ), 1200) < 1 )
                {
                    UseQ( target );
                }

                if ( target->HasUnitTag( Monster ) )
                {
                    UseQ( target );
                }
            }
        }
    }
    
#pragma region OnIssueOrder
    
    void OnIssueOrder( Vector3* pPosition, GameObject* pTarget, bool bWasPlayerIssued, bool* pCancel )
    {
        if ( OnWall(  ) && CamE->IsReady(  ) )
        {
            if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled(  ))
            {
                auto pos = *pPosition;
                auto dir = (pos - GetPlayer(  )->Position(  )).NormalizeXZ(  );

                auto e_tar = g_pExportedTargetSelector->GetTarget( CamE->Range(  ) + 100, true );
                if ( e_tar != nullptr && e_tar->IsAttackable( true, false ) )
                {
                    auto hero_dir = (e_tar->Position(  ) - GetPlayer(  )->Position(  )).NormalizeXZ(  );
                    bool any_dangerous_pos = false;
                    auto dash_end = GetPlayer(  )->Position(  ) + hero_dir * GetPlayer(  )->DistanceXZ( e_tar );

                    if ( Utils::UnderEnemyTurret(dash_end) && !Menu::DiveE->Enabled(  ) )
                        any_dangerous_pos = true;

                    for (auto o : AvoidList)
                    {
                        auto obj = o.Obj;
                        auto in_to_out = GetPlayer(  )->DistanceXZ( obj ) <= o.Radius;
                        
                        if ( in_to_out )
                        {
                            if (dash_end.DistanceXZ( obj->Position( ) ) > o.Radius)
                            {
                                any_dangerous_pos = true;
                                break;
                            }
                        }
                        else
                        {
                            if (Utils::IsOnSegment( obj->Position(  ), GetPlayer(  )->Position(  ), dash_end, o.Radius ))
                            {
                                any_dangerous_pos = true;
                                break;
                            }
                        }
                    }

                    if (any_dangerous_pos)
                    {
                        *pCancel = true;
                        return;
                    }

                    //Globals::Write( "Debug: 22\n" );
                    *pCancel = true;
                    auto pos = e_tar->Position(  );
                    GetPlayer( )->IssueMoveOrder( &pos, false, false, true, true, &Limiter );
                    return;
                }
            }
        }

        *pCancel = false;
    }
    
#pragma endregion
    
#pragma region Aura Mechancis

    bool HasQ( )
    {
        return GetPlayer( )->HasBuff( FNV1A32CI( "camilleqprimingstart" ) );
    }

    bool HasQ2( )
    {
        return GetPlayer( )->HasBuff( FNV1A32CI( "camilleqprimingcomplete" ) );
    }

    bool ChargingW( )
    {
        return GetPlayer( )->HasBuff( FNV1A32CI( "camillewconeslashcharge" ) );
    }

    bool OnWall( )
    {
        return CamE->Hash(  ) != FNV1A32CI( "CamilleE" ) || GetPlayer( )->HasBuff( FNV1A32CI( "camilleeonwall" ) );
    }

    bool IsDashing( )
    {
        return GetPlayer( )->HasBuff( FNV1A32CI( "camilleedash1" ) ) || GetPlayer( )->HasBuff( FNV1A32CI( "camilleedash2" ) );
    }

    bool KnockedBack( GameObject* unit )
    {
        return unit != nullptr && unit->HasBuff( FNV1A32CI( "camilleeknockback2" ) );
    }

    bool CanW( GameObject* unit )
    {
        if ( CamQ->IsReady( ) )
        {
            if ( !HasQ( ) || HasQ2( ) )
            {
                if ( GetPlayer( )->DistanceXZ( unit ) <= MyRange + 100 )
                {
                    return false;
                }
            }

            if ( unit != nullptr && QDmg( unit, false ) + AutoAttackDmg( unit ) * 1 >= unit->Health( ) )
            {
                return false;
            }
        }

        if ( GetPlayer( )->DistanceXZ( unit ) <= MyRange + 100 )
        {
            if ( AutoAttackDmg( unit ) * 3 >= unit->Health( ) )
            {
                return false;
            }
        }

        auto start = GetPlayer( )->GetBuff( FNV1A32CI( "camilleqprimingstart" ) );
        if ( start != nullptr )
        {
            if ( start->ExpireTime( ) - g_pExportedGlobalClocks->GameTime( ) <= 2.0f )
            {
                return false;
            }
        }

        auto complete = GetPlayer( )->GetBuff( FNV1A32CI( "camilleqprimingcomplete" ) );
        if ( complete != nullptr )
        {
            if ( complete->ExpireTime( ) - g_pExportedGlobalClocks->GameTime( ) <= 2.0f )
            {
                return false;
            }
        }

        return true;
    }

#pragma endregion

#pragma region Combat
    void UseQ( GameObject* unit )
    {
        if ( CamQ->IsReady( ) && Menu::UseQ -> Enabled(  ) )
        {
            if ( !HasQ( ) || HasQ2( ) )
            {
                CamQ->Cast( );
            }
            else
            {
                if ( unit != nullptr && QDmg( unit, false ) + AutoAttackDmg( unit ) * 1 >= unit->Health( ) )
                {
                    CamQ->Cast( );
                }
            }
        }
    }

    void UseW( GameObject* unit )
    {
        if ( !ChargingW( ) && !OnWall( ) && CanW( unit ) && !KnockedBack( unit ) )
        {
            if ( CamW->IsReady( ) && Menu::UseW->Enabled( ) )
            {
                if ( unit->DistanceXZ( GetPlayer( ) ) > MyRange + 65 )
                {
                    CamW->Cast( unit->Position( ) );
                }
            }
        }
    }

    void UseR( GameObject* unit , bool force)
    {
        if ( Menu::UseR->Enabled(  ) )
        {
            if ( CamR->IsReady( ) && force )
            {
                CamR->Cast( unit );
            }

            if ( CamR->IsReady( ) && Utils::IsValidTarget( unit, CamR->Range( ) ) )
            {
                if ( Menu::Selected->Enabled( ) )
                {
                    auto focus = g_pExportedTargetSelector->GetForcedTarget( );
                    if ( focus == nullptr || focus != nullptr && focus != unit )
                    {
                        return;
                    }
                }

                if ( QDmg( unit ) + AutoAttackDmg( unit ) * 2 >= unit->Health( ) )
                {
                    if ( GetPlayer( )->DistanceXZ( unit ) <= MyRange + 100 )
                    {
                        return;
                    }
                }

                if ( ComboDamage( unit ) >= unit->Health( ) )
                {
                    CamR->Cast( unit );
                }
            }
        }
    }

    void UseE(Vector3 pos, bool combo)
    {
        if ( !IsDashing(  ) && !OnWall(  ) && CamE->IsReady(  ) )
        {
            if ( Utils::UnderEnemyTurret(pos) && !Menu::DiveE->Enabled(  ) )
                return;
            
            auto pos_checked = 0;
            constexpr auto max_pos_checked = 95;
            auto radius_index = 0;
            auto pos_list = Vector<Vector3>();

            while (pos_checked < max_pos_checked)
            {
                radius_index++;
                constexpr auto pos_radius = 65;
                const auto cur_radius = radius_index * (2 * pos_radius);
                const auto cur_circle_checks = Math::CeilF((2 * Pi * cur_radius) / (2 * pos_radius));
                
                for (auto i = 1; i < static_cast<int>(cur_circle_checks); i++)
                {
                    pos_checked++;
                    const auto c_radians = (2 * Pi / (cur_circle_checks - 1)) * i;
                    const auto x_pos = Math::FloorF(pos.x + cur_radius * g_pExportedMath->CosF(c_radians));
                    const auto z_pos = Math::FloorF(pos.z + cur_radius * g_pExportedMath->SinF(c_radians));
                    auto pos_for_3d = Vector3(x_pos, z_pos);
                    bool any_dangerous_pos = false;
                    
                    if (pos_for_3d.DistanceXZ( pos ) > CamE->Range(  ))
                        continue;

                    if (pos_for_3d.DistanceXZ( GetPlayer(  )->Position(  ) ) > CamE->Range(  ))
                        continue;
                    
                    if ( Utils::UnderEnemyTurret(pos_for_3d) && !Menu::DiveE->Enabled(  ) )
                        any_dangerous_pos = true;

                    for (auto o : AvoidList)
                    {
                        auto obj = o.Obj;
                        auto in_to_out = GetPlayer(  )->DistanceXZ( obj ) <= o.Radius;
                        
                        if ( in_to_out )
                        {
                            if (pos_for_3d.DistanceXZ( obj->Position( ) ) > o.Radius)
                            {
                                any_dangerous_pos = true;
                                break;
                            }
                        }
                        else
                        {
                            if (Utils::IsOnSegment( obj->Position(  ), GetPlayer(  )->Position(  ), pos_for_3d, o.Radius ))
                            {
                                any_dangerous_pos = true;
                                break;
                            }
                        }
                    }

                    if (any_dangerous_pos)
                        continue;

                    if (g_pExportedNavigationMesh->GetCellState(pos_for_3d) == CellCollisionFlags::Wall)
                        pos_list.push_back(pos_for_3d);

                    if (g_pExportedNavigationMesh->GetCellState(pos_for_3d) == CellCollisionFlags::Building)
                        pos_list.push_back(pos_for_3d);

                    pos_list.sort([&](Vector3 a, Vector3 b)
                    {
                        switch (Menu::HookSpot->SelectedItem(  ))
                        {
                            case 0:
                                return a.DistanceXZ( pos ) < b.DistanceXZ( pos );
                    
                            case 1:
                                return a.DistanceXZ( GetPlayer(  )->Position(  ) ) < b.DistanceXZ( GetPlayer(  )->Position(  ) );
                        }
                    });
                }
            }

            for (auto vec : pos_list)
            {
                if ( vec.IsValid(  ) )
                {
                    CamE->Cast(vec);
                    break;
                }
            }
        }
    }


#pragma endregion

#pragma region Damage
    float ComboDamage( GameObject* unit )
    {
        auto aa = AutoAttackDmg( unit ) * 4;
        auto qq = QDmg( unit, true ) * 2;
        auto ww = WDmg( unit, false );
        auto ee = EDmg( unit, true );
        auto rr = RDmg( aa, unit );

        return IgniteDmg( ) + ( qq + ww + ee + rr ) * Menu::Difficulty->Value( ) / 100;
    }

    float IgniteDmg( )
    {
        if ( !Ignite->IsReady( ) || !Menu::UseIgnite->Enabled( ) )
            return 0;

        // constants
        const auto lvl = GetPlayer( )->Experience( )->Level( );
        constexpr double base_value_min = 70.0;
        constexpr double base_value_max = 410.0;

        // calculate the value based on the level
        return base_value_min + ( ( base_value_max - base_value_min ) / 17.0 ) * ( lvl - 1 );
    }

    float QDmg( GameObject* unit, bool includeq2 )
    {
        auto dmg = 0;

        if ( CamQ->IsReady( ) && unit != nullptr )
        {
            auto camille_level = GetPlayer( )->Experience( )->Level( );
            auto bonus_physical_damage = ( 20 + ( CamQ->Level( ) - 1 ) * 5 ) / 100.0 * GetPlayer( )->TotalAttackDamage( );
            auto mixed_damage = ( 40 + ( CamQ->Level( ) - 1 ) * 10 ) / 100.0 * GetPlayer( )->TotalAttackDamage( );
            auto min_tru_dmg_pct = 0.4;
            auto max_tru_dmg_pct = 1.0;
            auto true_dmg_pct = min_tru_dmg_pct + ( max_tru_dmg_pct - min_tru_dmg_pct ) * ( min( camille_level - 1, 16 ) / 16.0 );


            auto both = bonus_physical_damage;
            double converted = 0;
            double after = both;

            if ( includeq2 )
            {
                both = mixed_damage;
                converted = both * true_dmg_pct;
                after = both - converted;
            }

            auto result = g_pExportedDamage->CalculatePhysicalDamage( GetPlayer( ), unit, after ).m_flTotal;

            result += converted;
            return result;
        }

        return dmg;
    }

    float WDmg( GameObject* unit, bool bonus )
    {
        auto dmg = 0;
        if ( CamW->IsReady( ) && unit != nullptr )
        {
            // get bonus attack damage
            auto bonus_ad = GetPlayer( )->CharacterIntermediate( )->FlatPhysicalDamageMod( );

            // base physical damage
            double base_damage = ( 70 + ( CamW->Level( ) - 1 ) * 30 ) + ( 0.6 * bonus_ad );

            // Calculate additional damage for hitting the outer half
            double bonus_damage = ( 5 + ( CamW->Level( ) - 1 ) * 0.5 ) / 100.0 * unit->MaxHealth( ) + ( 0.025 * bonus_ad );

            // cap the bonus damage against monsters at 300
            if ( unit->HasUnitTag( Minion ) )
                bonus_damage = min( bonus_damage, 300.0 );

            auto result = base_damage;
            if ( bonus && GetPlayer( )->DistanceXZ( unit ) > 400 )
                result += bonus_damage;

            return g_pExportedDamage->CalculatePhysicalDamage( GetPlayer( ), unit, result ).m_flTotal;
        }

        return dmg;
    }

    float AutoAttackDmg( GameObject* unit )
    {
        return g_pExportedDamage->GetAutoAttackDamage( GetPlayer( ), unit, true ).m_flTotal;
    }

    float EDmg( GameObject* unit, bool aa )
    {
        auto dmg = 0;
        if ( CamE->IsReady( ) && unit != nullptr )
        {
            // get bonus attack damage
            auto bonus_ad = GetPlayer( )->CharacterIntermediate( )->FlatPhysicalDamageMod( );

            // calculate base physical damage
            double base_damage = ( 90 + ( CamE->Level( ) - 1 ) * 30 ) + ( 0.9 * bonus_ad );

            auto result = g_pExportedDamage->CalculatePhysicalDamage( GetPlayer( ), unit, base_damage ).m_flTotal;

            if ( aa )
                result += AutoAttackDmg( unit );

            return result;
        }

        return dmg;
    }

    float RDmg( double dmg, GameObject* unit )
    {
        auto extra = 0;
        if ( ( CamR->IsReady( ) || GetPlayer( )->HasBuff( FNV1A32CI( "camillertether" ) ) ) && unit != nullptr )
        {
            // get player level
            auto level = min( 18, GetPlayer( )->Experience( )->Level( ) );

            // calculate bonus magic damage
            extra = ( 20 + ( level - 1 ) * 10 ) + ( ( 0.04 * unit->Health( ) ) + ( 0.02 * level - 1 ) * unit->Health( ) ) * 0.01;
        }

        return dmg + extra;
    }

#pragma endregion
}
