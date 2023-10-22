#include "stdafx.hpp"
extern "C" inline __declspec(dllexport) std::uint32_t g_iUniqueAssemblyName = 0;


/**
 * @brief This function is called when a particle is created for a game object.
 * @param unit The game object the particle is being created for.
 * @param iHash The hash of the particle being created.
 * @return void
 */
void OnCreateParticle( GameObject* unit, std::uint32_t iHash )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnCreateParticle( unit, iHash );
        default: ;
    }
}


/**
 * @brief This function is called every frame to draw the plugin's content on the screen.
 * @return void
 */
void OnDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnDraw( );
        case SDBM32CI( "Vex" ):
            return Vex::OnDraw( );
        default: ;
    }
}

/**
 * @brief This function is called every frame to draw the plugin's content on the screen.
 * @return void
 */
void OnPresentDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnPresentDraw( );
        case SDBM32CI( "Vex" ):
            return Vex::OnPresentDraw( );
        default: ;
    }
}

/**
 * @brief Called every frame to update the plugin's logic.
 * @return void
 */
void OnUpdate( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnUpdate( );
        case SDBM32CI( "Vex" ):
            return Vex::OnUpdate( );
        default: ;
    }
}

/**
 * @brief Called when the plugin is loaded.
 * @return void
 */
void OnBootPlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnBoot( );
        case SDBM32CI( "Vex" ):
            return Vex::OnBoot( );
        default: ;
    }
}

/**
 * @brief Called when the plugin is unloaded.
 * @return void
 */
void OnTerminatePlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnTerminate( );
        case SDBM32CI( "Vex" ):
            return Vex::OnTerminate( );         
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
