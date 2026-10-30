/*
	Manages all keybinds for COM
*/
class COMKeyBinds extends Module 
{
	bool m_IsHudVisible = true;
	protected int m_nAutoWalkMode = 0;

	void COMKeyBinds()
	{
	}

	void ~COMKeyBinds()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( float timeslice )
	{
        if (!COM_GetPB()) return;
	    UpdateAutoWalk();
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCursor    = new KeyMouseBinding( GetModuleType() , "ToggleCursor"  , "Toggles the cursor."   , true     );
		KeyMouseBinding toggleCOMEditor = new KeyMouseBinding( GetModuleType() , "ShowCOMEditor" ,  "Opens the COM Editor."           );
		KeyMouseBinding teleport	    = new KeyMouseBinding( GetModuleType() , "TeleportCursor",  "Teleport to cursor position."    );
		KeyMouseBinding reload          = new KeyMouseBinding( GetModuleType() , "Reload"        ,  "Instantly reloads mag."		  );
        KeyMouseBinding teleportH = new KeyMouseBinding(GetModuleType(), "TeleportCursorH", "Teleport to mouse cursor (H).");
        KeyMouseBinding spawnZ          = new KeyMouseBinding( GetModuleType() , "SpawnZ"        ,  "Spawns infected."                );
        KeyMouseBinding hideHud         = new KeyMouseBinding( GetModuleType() , "HideHud"       ,  "Hides ui completely."            );
        KeyMouseBinding printPlayer     = new KeyMouseBinding( GetModuleType() , "PrintPlayer"   ,  "Print current player position."  );
        KeyMouseBinding autoRun         = new KeyMouseBinding( GetModuleType() , "AutoRun"       ,  "Toggle autorun."                 );
//        KeyMouseBinding keyFrame        = new KeyMouseBinding( GetModuleType() , "OpenKeyframe"  ,  "Toggle dayz dev cinematic tool." );

		toggleCursor   .AddBinding( "kU" );
		toggleCOMEditor.AddBinding( "kY" );
		teleport       .AddBinding( "kEnd" );
        teleportH.AddBinding("kH");
		reload         .AddBinding( "kR" );
        spawnZ         .AddBinding( "kO" );
        hideHud        .AddBinding( "kHome" );
        printPlayer    .AddBinding( "kP" );
        autoRun        .AddBinding( "kZ" );
//        keyFrame       .AddBinding( "kPrior" );

		RegisterKeyMouseBinding( toggleCursor );
		RegisterKeyMouseBinding( toggleCOMEditor );
		RegisterKeyMouseBinding( teleport );
        RegisterKeyMouseBinding(teleportH);
		RegisterKeyMouseBinding( reload );
        RegisterKeyMouseBinding( spawnZ );
        RegisterKeyMouseBinding( hideHud );
        RegisterKeyMouseBinding( printPlayer );
        RegisterKeyMouseBinding( autoRun );
        // Clear earlier COM autorun alternatives, including the old X binding.
        UAInput autoRunInput = GetUApi().GetInputByName(autoRun.GetUAInputName());
        if (autoRunInput) { autoRunInput.ClearBinding(); autoRunInput.BindCombo("kZ"); }
//        RegisterKeyMouseBinding( keyFrame );
	}

//    void OpenKeyframe()
//    {
//        GetGame().GetUIManager().ShowScriptedMenu( COM_GetMission().CreateScriptedMenu(MENU_CAMERA_TOOLS) , NULL );
//    }

    void COM_ToggleCursor()
    {
        if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
        {
            GetGame().GetInput().ChangeGameFocus( 1 );
            GetGame().GetUIManager().ShowUICursor( true );
            FreeDebugCamera.GetInstance().SetFreezed(true);
        }
        else
        {
            GetGame().GetUIManager().ShowUICursor( false );
            GetGame().GetInput().ResetGameFocus();
            FreeDebugCamera.GetInstance().SetFreezed(false);
        }
         COM_Message(GetDayZGame().GetMissionFolderPath());
    }

    void ShowCOMEditor()
    {
        GetGame().GetUIManager().EnterScriptedMenu(EditorMenu.MENU_ID, NULL);
    }

    void TeleportCursorH()
    {
        TeleportCursor(false);
    }

    void TeleportCursor(bool announce = true)
    {
        if (!COM_GetPB() || GetGame().GetUIManager().GetMenu()) return;
        vector hitPos;
        if (!TryGetTeleportCursor(hitPos)) { COM_Message("No surface under the cursor."); return; }
        TeleportToPosition(hitPos, false, announce);
    }

    bool TryGetTeleportCursor(out vector hitPos)
    {
        if (!GetGame().GetUIManager().IsCursorVisible()) return COM_TryGetCursorPos(hitPos);
        hitPos = vector.Zero;
        if (!COM_GetPB()) return false;
        vector from = GetGame().GetCurrentCameraPosition();
        vector to = from + GetGame().GetPointerDirection() * 10000;
        vector normal;
        int component;
        return DayZPhysics.RaycastRV(from, to, hitPos, normal, component, NULL, NULL, COM_GetPB());
    }

    bool TeleportToPosition(vector destination, bool snapToSurface = false, bool announce = true)
    {
        PlayerBase player = COM_GetPB();
        if (!player || !player.IsAlive()) return false;
        if (player.GetCommand_Vehicle()) { COM_Message("Get out of the vehicle first!"); return false; }
        float worldSize = GetGame().GetWorld().GetWorldSize();
        if (destination[0] < 0 || destination[2] < 0 || destination[0] >= worldSize || destination[2] >= worldSize)
        {
            COM_Message("Teleport destination is outside the map.");
            return false;
        }
        if (snapToSurface) destination[1] = GetGame().SurfaceY(destination[0], destination[2]);
        player.SetPosition(destination);
        if (announce) COM_Message("Teleported!");
        return true;
    }
    void RefillWeapon(Weapon_Base weapon, bool chamber = true)
    {
        if (!weapon) return;
        Magazine magazine = weapon.GetMagazine(weapon.GetCurrentMuzzle());
        if (magazine) magazine.ServerSetAmmoCount(magazine.GetAmmoMax());
        if (weapon.HasInternalMagazine(-1)) weapon.FillInnerMagazine("", WeaponWithAmmoFlags.NONE);
        if (chamber)
        {
            string ammoType;
            if (magazine) ammoType = magazine.GetType();
            weapon.FillChamber(ammoType, WeaponWithAmmoFlags.CHAMBER);
        }
        EntityAI suppressor = weapon.GetAttachmentByConfigTypeName("ItemSuppressor");
        if (suppressor) suppressor.SetHealth01("", "", 1);
    }

    void Reload()
    {
        PlayerBase player = COM_GetPB();
        if (!player) return;
        RefillWeapon(Weapon_Base.Cast(player.GetHumanInventory().GetEntityInHands()));
    }
    void SpawnZ() 
    {
        if( COM_CTRL() )
        {
            GetGame().CreateObject( "Animal_CanisLupus_Grey", COM_GetCursorPos(), false, true );
        }
        else if( COM_SHIFT() )
        {
            GetGame().CreateObject( COM_GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase", 2 ), COM_GetCursorPos(), false, true );
        }
        else
        {
            GetGame().CreateObject( COM_GetRandomChildFromBaseClass( "cfgVehicles", "ZombieBase", 2 ), COM_GetCursorPos(), false, true );
        }
    }

    void HideHud() 
    {
        Widget hudWidget = IngameHud.Cast(COM_GetClientMission().GetHud()).GetHudPanelWidget();
        hudWidget.Show(!hudWidget.IsVisible());
    }

    void PrintPlayer()
    {
        Print( "Position:" + COM_GetPB().GetPosition().ToString() );
        Print( "Orientation" + COM_GetPB().GetOrientation().ToString() );
        COM_Message( "POS X:" + COM_GetPB().GetPosition()[0] + " Y:" + COM_GetPB().GetPosition()[2] + " Z:" + COM_GetPB().GetPosition()[1] );
        COM_Message( "Player position and orientation vector were written to the game logs too." );
    }

    void AutoRun()
    {
        if (!COM_GetPB() || GetGame().GetUIManager().GetMenu()) return;
        CameraTool camera = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool));
        if (camera && camera.IsUsingCamera()) return;
        if( m_nAutoWalkMode && !COM_SHIFT() && !COM_CTRL() )
        {
            CancelAutoRun();
        }
        else
        {
            if( COM_SHIFT() )
            {
                m_nAutoWalkMode = 3;
            }
            else if( COM_CTRL() )
            {
                m_nAutoWalkMode = 1; //CTRL == slow mode
            }
            else
            {
                m_nAutoWalkMode = 2;
            }
        }
    }

    bool IsAutoRunning() { return m_nAutoWalkMode > 0; }
    void CancelAutoRun()
    {
        if (!m_nAutoWalkMode) return;
        m_nAutoWalkMode = 0;
        PlayerBase player = COM_GetPB();
        CameraTool camera = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool));
        if (!player || (camera && camera.IsUsingCamera())) return;
        player.GetInputController().OverrideMovementSpeed(false, 0);
        player.GetInputController().OverrideMovementAngle(false, 0);
    }
    bool HasManualMovementInput()
    {
        // Read physical movement actions, not the autorun movement override.
        UAInput forward = GetUApi().GetInputByName("UAMoveForward");
        UAInput backward = GetUApi().GetInputByName("UAMoveBack");
        return (forward && forward.LocalValue() > 0) || (backward && backward.LocalValue() > 0);
    }
    void UpdateAutoWalk()
    {
        PlayerBase player = COM_GetPB();
        if (!player) return;
        CameraTool camera = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool));
        if (camera && camera.IsUsingCamera()) return;
        if (m_nAutoWalkMode > 0 && (!player.IsAlive() || GetGame().GetUIManager().GetMenu() || HasManualMovementInput()))
        {
            CancelAutoRun();
            return;
        }
        if( m_nAutoWalkMode > 0 )
        {
            if( m_nAutoWalkMode == 1 )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 1 );
            }
            else if( ( COM_GetPB().GetInputController().LimitsIsSprintDisabled() ) || ( m_nAutoWalkMode == 2 ) )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 2 );
            }
            else if( m_nAutoWalkMode == 3 )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 3 );
            }

            COM_GetPB().GetInputController().OverrideMovementAngle( true, 1 );
        }
    }

}
