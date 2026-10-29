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

	    UpdateGodMode();
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCursor    = new KeyMouseBinding( GetModuleType() , "ToggleCursor"  , "Toggles the cursor."   , true     );
		KeyMouseBinding toggleCOMEditor = new KeyMouseBinding( GetModuleType() , "ShowCOMEditor" ,  "Opens the COM Editor."           );
		KeyMouseBinding teleport	    = new KeyMouseBinding( GetModuleType() , "TeleportCursor",  "Teleport to cursor position."    );
		KeyMouseBinding reload          = new KeyMouseBinding( GetModuleType() , "Reload"        ,  "Instantly reloads mag."		  );
        KeyMouseBinding spawnZ          = new KeyMouseBinding( GetModuleType() , "SpawnZ"        ,  "Spawns infected."                );
        KeyMouseBinding hideHud         = new KeyMouseBinding( GetModuleType() , "HideHud"       ,  "Hides ui completely."            );
        KeyMouseBinding printPlayer     = new KeyMouseBinding( GetModuleType() , "PrintPlayer"   ,  "Print current player position."  );
        KeyMouseBinding autoRun         = new KeyMouseBinding( GetModuleType() , "AutoRun"       ,  "Toggle autorun."                 );
//        KeyMouseBinding keyFrame        = new KeyMouseBinding( GetModuleType() , "OpenKeyframe"  ,  "Toggle dayz dev cinematic tool." );

		toggleCursor   .AddBinding( "kU" );
		toggleCOMEditor.AddBinding( "kY" );
		teleport       .AddBinding( "kEnd" );
		reload         .AddBinding( "kR" );
        spawnZ         .AddBinding( "kO" );
        hideHud        .AddBinding( "kHome" );
        printPlayer    .AddBinding( "kP" );
        autoRun        .AddBinding( "kX" );
//        keyFrame       .AddBinding( "kPrior" );

		RegisterKeyMouseBinding( toggleCursor );
		RegisterKeyMouseBinding( toggleCOMEditor );
		RegisterKeyMouseBinding( teleport );
		RegisterKeyMouseBinding( reload );
        RegisterKeyMouseBinding( spawnZ );
        RegisterKeyMouseBinding( hideHud );
        RegisterKeyMouseBinding( printPlayer );
        RegisterKeyMouseBinding( autoRun );
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

    void TeleportCursor()
    {
        Print( "COMKeyBinds::TeleportCursor()" );

//		if ( CameraTool.Cast( m_Mission.GetModule( CameraTool ) ).IsUsingCamera() ) //Todo renable after module manager is done
//		{
//			COM_GetPB().MessageStatus( "You can not teleport while you are inside the freecam!" );
//			return;
//		}
		
        if (!COM_GetPB()) return;
        vector hitPos;
        if (!COM_TryGetCursorPos(hitPos)) { COM_Message("No surface under the cursor."); return; }

        float distance = vector.Distance( COM_GetPB().GetPosition(), hitPos );

        if ( distance < 5000 )
        {
            EntityAI oVehicle = COM_GetPB().GetDrivingVehicle();

            if( oVehicle )
            {
                COM_GetPB().MessageStatus( "Get out of the vehicle first!" );
            }
            else
            {
                COM_GetPB().SetPosition( hitPos );
                COM_GetPB().MessageStatus( "Teleported!" );
            }
        }
        else
        {
            COM_GetPB().MessageStatus( "Distance for teleportation is too far!" );
        }
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
        if (!COM_GetPB()) return;
        if( m_nAutoWalkMode && !COM_SHIFT() && !COM_CTRL() )
        {
            m_nAutoWalkMode = 0;
            COM_GetPB().GetInputController().OverrideMovementSpeed( false, 0 );
            COM_GetPB().GetInputController().OverrideMovementAngle( false, 0 );
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

    void UpdateAutoWalk()
    {
        PlayerBase player = COM_GetPB();
        if (!player) return;
        CameraTool camera = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool));
        if (camera && camera.IsUsingCamera()) return;
        if (m_nAutoWalkMode > 0 && EditorMenu.Cast(GetGame().GetUIManager().GetMenu()))
        {
            player.GetInputController().OverrideMovementSpeed(true, 0);
            player.GetInputController().OverrideMovementAngle(false, 0);
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

    void UpdateGodMode()
    {
        // just putting this here for now
        if ( m_COM_GodMode ) // located in staticfunctions
        {
            COM_GetPB().SetHealth( COM_GetPB().GetMaxHealth( "", "" ) );
            COM_GetPB().SetHealth( "","Blood", COM_GetPB().GetMaxHealth( "", "Blood" ) );
            COM_GetPB().SetHealth( "","Shock", COM_GetPB().GetMaxHealth( "", "Shock" ) );

            //GetPlayer().GetStaminaHandler().SyncStamina(1000, 1000);
            COM_GetPB().GetStatStamina().Set(COM_GetPB().GetStaminaHandler().GetStaminaCap());
            COM_GetPB().GetStatEnergy().Set(1000);
            COM_GetPB().GetStatWater().Set(1000);
//            COM_GetPB().GetStatStomachVolume().Set(300);
//            COM_GetPB().GetStatStomachWater().Set(300);
//            COM_GetPB().GetStatStomachEnergy().Set(300);
            COM_GetPB().GetStatHeatComfort().Set(0);

            RefillWeapon(Weapon_Base.Cast(COM_GetPB().GetHumanInventory().GetEntityInHands()), false);
        }
    }
}