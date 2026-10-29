class CharacterLoad
{
    static PlayerBase LoadPlayer(string sCharacter, string sSave, bool bObject = false, PlayerIdentity oIdentity = NULL ) {
        if (!CharacterSave.IsValidName(sCharacter) || !CharacterSave.IsValidName(sSave)) return NULL;
        CharacterData oData;

		string file = BASE_PLAYER_SAVE_DIR + "\\" + sCharacter + "\\" + sSave + ".json";

		Print("Loading file: " + file);

        string error;
        if (!FileExist(file) || !JsonFileLoader<CharacterData>.LoadFile(file, oData, error) || !oData || oData.SModel == "" || !GetGame().IsKindOf(oData.SModel, "SurvivorBase"))
        {
            Print("[COM] Cannot load character: " + file + " " + error);
            return NULL;
        }

		PlayerBase oPlayer;
		
		if ( bObject )
		{
			oPlayer = PlayerBase.Cast( GetGame().CreateObject( oData.SModel, "0 0 0", true, true, true ) );
		} else 
		{
			oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( oIdentity, oData.SModel, "0 0 0", 0, "NONE") );
		}

		if (!oPlayer) return NULL;
        // Keep near-death legacy saves playable.
        if (oData.FHealth < 20) oData.FHealth = 20;
        if (oData.FBlood < 2000) oData.FBlood = 2000;

		oPlayer.SetDirection( oData.VecDirection );
		oPlayer.SetOrientation( oData.VecOrientation );
        
		oPlayer.SetHealth("","", oData.FHealth );
		oPlayer.SetHealth("GlobalHealth", "Blood", oData.FBlood );
		oPlayer.GetStatBloodType().Set( oData.IBloodStatType );
		//oPlayer.GetStatTemperature().Set( oData.FTemperature );
		//oPlayer.GetStatShock().Set( oData.FShock );
		oPlayer.GetStatEnergy().Set( oData.FEnergy );
		oPlayer.GetStatWater().Set( oData.FWater );
		oPlayer.GetStatHeatComfort().Set( oData.FHeatComfort );
		oPlayer.GetStatWet().Set( oData.FWet );
		oPlayer.GetStatTremor().Set( oData.FTremor );
		if (oPlayer.GetStaminaHandler()) oPlayer.GetStaminaHandler().SetStamina(oData.FStamina);
		oPlayer.StatRegister("playtime");
		oPlayer.StatUpdate("playtime", oData.FPlayTime);
		oPlayer.SetLastShavedSeconds(oData.FLastShaved);
		oPlayer.SetBloodyHands(oData.FBloodyHands);

        if (oData.Version >= 2)
        {
            oPlayer.SetHealth("GlobalHealth", "Shock", oData.FShock);
            if (oPlayer.GetStomach())
            {
                oPlayer.GetStomach().ClearContents();
                foreach (COMStomachData entry : oData.Stomach)
                    if (entry) oPlayer.GetStomach().AddToStomach(entry.ClassName, entry.Amount, entry.SavedFoodStage, entry.Agents, entry.Temperature);
            }
        }
        if (oData.HasInventory && oData.OInventory) {
            InventoryLoad.Load(oPlayer, oPlayer, oData.OInventory);
        }

        if (oData.HasItemInHands && oData.OHands) {
            HandLoad.Load(oPlayer, oPlayer, oData.OHands);
        }

		oPlayer.SetPosition( oData.VecPosition );

        return oPlayer;

    }
}