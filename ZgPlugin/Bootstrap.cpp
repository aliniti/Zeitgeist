#include "stdafx.hpp"

// set to SDBM32CI("ChampionName") to load on a specific champion only
extern "C" inline __declspec(dllexport) std::uint32_t g_iUniqueAssemblyName = 0;

MenuElement* Config;

void OnCreateParticle( GameObject* pObject, std::uint32_t iHash )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnCreateParticle( pObject, iHash );
        default: ;
    }
}

void OnDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnDraw( );
        default: ;
    }
}

void OnPresentDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnPresentDraw( );
            default: ;
    }
}

void OnUpdate( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnUpdate( );
        default: ;
    }
}

void OnBootPlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnBoot( );
        default: ;
    }
}

void OnTerminatePlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnTerminate( );
        default: ;
    }
}

#pragma section(".tempc", execute, read)
extern "C" bool __declspec(dllexport, noinline, code_seg(".tempc")) __stdcall ZgLoad( SdkContext* pSdkContext )
{
    pSdkContext->Initialize( );
    Globals::Allocate( );

    OnBootPlugin( );
    g_pExportedEventHandler->Add( EventType::OnCreateParticle, OnCreateParticle );
    g_pExportedEventHandler->Add( EventType::OnWorldDraw, OnDraw );
    g_pExportedEventHandler->Add( EventType::OnPresentDraw, OnPresentDraw );
    g_pExportedEventHandler->Add( EventType::OnGameUpdate, OnUpdate );
    return true;
}

extern "C" void __declspec(dllexport, noinline, code_seg(".text")) __stdcall ZgUnload( )
{
    OnTerminatePlugin(  );
    g_pExportedEventHandler->Remove( EventType::OnCreateParticle, OnCreateParticle );
    g_pExportedEventHandler->Remove( EventType::OnWorldDraw, OnDraw );
    g_pExportedEventHandler->Remove( EventType::OnPresentDraw, OnPresentDraw );
    g_pExportedEventHandler->Remove( EventType::OnGameUpdate, OnUpdate );
}
