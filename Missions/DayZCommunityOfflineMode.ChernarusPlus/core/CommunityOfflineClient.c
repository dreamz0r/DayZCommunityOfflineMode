class CommunityOfflineClient extends MissionGameplay
{
	protected bool HIVE_ENABLED = true; //Local Hive / Economy / Infected spawn

    protected bool m_loaded;
    protected bool m_COMOwnsHive;
    protected bool m_COMRespawnPending;

	void CommunityOfflineClient()
	{
	    m_loaded = false;

		NewModuleManager();
	}

	override void OnInit()
	{
		super.OnInit();

        InitHive();

        SetupWeather();

		SpawnPlayer();

		GetDayZGame().SetMissionPath( "$saves:CommunityOfflineMode\\" ); // CameraToolsMenu
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

        COM_GetModuleManager().OnInit();
		COM_GetModuleManager().OnMissionStart();
	}

	override void OnMissionFinish()
	{
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.RespawnOfflinePlayer);
        COM_GetModuleManager().OnMissionFinish();

		CloseAllMenus();

		DestroyAllMenus();

		if (m_COMOwnsHive && GetHive())
		{
			DestroyHive();
		}

		super.OnMissionFinish();
	}

    void OnMissionLoaded()
    {
		COM_GetModuleManager().OnMissionLoaded();
    }

	override void OnUpdate( float timeslice )
	{
	    super.OnUpdate( timeslice );

        COM_GetModuleManager().OnUpdate( timeslice );

        if( !m_loaded && !GetDayZGame().IsLoading() )
        {
            m_loaded = true;
            OnMissionLoaded();
        }
	}

    void SpawnPlayer()
    {
//		#ifndef MODULE_PERSISTENCY
//		GetGame().SelectPlayer( NULL, COM_CreateCustomDefaultCharacter() );
//		#endif

//		#ifdef DISABLE_PERSISTENCY
		GetGame().SelectPlayer( NULL, COM_CreateCustomDefaultCharacter() );
//		#endif
    }

	void InitHive()
	{
		if ( GetGame().IsClient() && GetGame().IsMultiplayer() ) return;

		// RD /s /q "storage_-1" > nul 2>&1
		if ( !HIVE_ENABLED ) return;
	
        // An existing native hive must not be initialized a second time.
        if (GetHive()) return;
        Hive oHive = CreateHive();

		if( oHive )
		{
			m_COMOwnsHive = true;
            oHive.InitOffline();
			oHive.SetShardID("100");
			oHive.SetEnviroment("stable");
		}

	}

    void RequestOfflineRespawn()
    {
        if (GetGame().IsMultiplayer() || m_COMRespawnPending) return;
        m_COMRespawnPending = true;
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RespawnOfflinePlayer, 200, false);
    }
    void RespawnOfflinePlayer()
    {
        m_COMRespawnPending = false;
        PlayerBase previous = COM_GetPB();
        CameraTool camera = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool));
        if (camera && camera.IsUsingCamera()) camera.DisableCamera();
        PlayerBase replacement = COM_CreateCustomDefaultCharacter();
        if (!replacement) { COM_Message("Unable to create a new character."); return; }
        GetGame().SelectPlayer(NULL, replacement);
        // Refresh both roles before the old character becomes an interaction target.
        PrepareOfflinePlayerActions(replacement);
        PrepareOfflinePlayerActions(previous);
        if (previous && previous != replacement && previous.IsAlive()) previous.SetHealth("", "", 0);
        OnPlayerRespawned(replacement);
        SetPlayerRespawning(false);
        Continue();
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
    }
    void PrepareOfflinePlayerActions(PlayerBase player)
    {
        if (!player) return;
        player.InitializeActions();
        // DayZ 1.30 also registers PickUpBricks into the legacy map for remote players.
        // GetActions merges that map without checking whether the role has an input list.
        // Empty lists keep that native merge valid without adding or removing any actions.
        for (int i = 0; i < player.m_InputActionMap.Count(); i++)
        {
            typename inputType = player.m_InputActionMap.GetKey(i);
            if (!player.m_InputActionMapControled.Contains(inputType))
                player.m_InputActionMapControled.Insert(inputType, new array<ActionBase_Basic>);
            if (!player.m_InputActionMapAsTarget.Contains(inputType))
                player.m_InputActionMapAsTarget.Insert(inputType, new array<ActionBase_Basic>);
        }
        player.m_ActionsInitialize = true;
    }
    static void SetupWeather()
    {
        Weather weather = g_Game.GetWeather();
        weather.MissionWeather( true );

        weather.GetOvercast().SetLimits( 0.0 , 1.0 );
        weather.GetRain().SetLimits( 0.0 , 1.0 );
        weather.GetFog().SetLimits( 0.0 , 1.0 );

        weather.GetOvercast().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetRain().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetFog().SetForecastChangeLimits( 0.0, 0.0 );

        weather.GetOvercast().SetForecastTimeLimits( 1800 , 1800 );
        weather.GetRain().SetForecastTimeLimits( 600 , 600 );
        weather.GetFog().SetForecastTimeLimits( 600 , 600 );

        weather.GetOvercast().Set( 0.0, 0, 0 );
        weather.GetRain().Set( 0.0, 0, 0 );
        weather.GetFog().Set( 0.0, 0, 0 );

        weather.SetWindMaximumSpeed( 50 );
        weather.SetWindFunctionParams( 0, 0, 1 );
    }
    
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if (id == MENU_INGAME) return new COMInGameMenu();
        if(id == EditorMenu.MENU_ID)
        {
            return new EditorMenu();
        }
        
        return super.CreateScriptedMenu(id);
    }
}
