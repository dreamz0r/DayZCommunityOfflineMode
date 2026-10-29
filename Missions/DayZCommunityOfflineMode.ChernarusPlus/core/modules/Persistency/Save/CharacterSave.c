class CharacterSave
{
    static bool IsValidName(string name)
    {
        return name != "" && name != "." && name.IndexOf("..") == -1 && name.IndexOf("/") == -1 && name.IndexOf("\\") == -1 && name.IndexOf(":") == -1;
    }
    static void SavePlayer(string character, string save, PlayerBase player = NULL)
    {
        if (!player || !IsValidName(character)) return;
        if (save == "") save = "latest";
        if (!IsValidName(save)) return;
        CharacterData data = new CharacterData;
        data.Version = 2;
        data.SModel = player.GetType();
        data.VecPosition = player.GetPosition();
        data.VecDirection = player.GetDirection();
        data.VecOrientation = player.GetOrientation();
        data.FHealth = player.GetHealth("", "");
        data.FBlood = player.GetHealth("GlobalHealth", "Blood");
        data.FShock = player.GetHealth("GlobalHealth", "Shock");
        data.IBloodStatType = player.GetStatBloodType().Get();
        data.FEnergy = player.GetStatEnergy().Get();
        data.FWater = player.GetStatWater().Get();
        data.FHeatComfort = player.GetStatHeatComfort().Get();
        data.FWet = player.GetStatWet().Get();
        data.FTremor = player.GetStatTremor().Get();
        data.FStamina = player.GetStatStamina().Get();
        data.FLastShaved = player.GetLastShavedSeconds();
        data.FBloodyHands = player.HasBloodyHands();
        data.FPlayTime = player.StatGet("playtime");
        if (player.GetStomach())
        {
            foreach (StomachItem content : player.GetStomach().m_StomachContents)
            {
                COMStomachData entry = new COMStomachData;
                entry.ClassName = content.GetClassName();
                entry.Amount = content.GetAmount();
                entry.SavedFoodStage = content.GetFoodStage();
                entry.Agents = content.m_Agents;
                entry.Temperature = content.GetTemperature();
                data.Stomach.Insert(entry);
            }
        }
        data.HasInventory = true;
        InventorySave.Save(player, player, data.OInventory);
        EntityAI hands = player.GetHumanInventory().GetEntityInHands();
        data.HasItemInHands = hands != NULL;
        if (hands) HandSave.Save(hands, player, data.OHands);
        MakeDirectory(BASE_COM_DIR);
        MakeDirectory(BASE_PLAYER_SAVE_DIR);
        MakeDirectory(BASE_PLAYER_SAVE_DIR + "\\" + character);
        string error;
        if (!JsonFileLoader<CharacterData>.SaveFile(BASE_PLAYER_SAVE_DIR + "\\" + character + "\\" + save + ".json", data, error))
            Print("[COM] Character save failed: " + error);
    }
    static void CreatePlayer(string character, PlayerBase player)
    {
        SavePlayer(character, "latest", player);
    }
}