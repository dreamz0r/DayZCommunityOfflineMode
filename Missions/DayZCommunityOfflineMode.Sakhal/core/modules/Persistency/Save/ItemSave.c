class ItemSave
{
    static ItemData Save(EntityAI oEntity, PlayerBase oPlayer, ItemData oThisData) {
        if (!oEntity || !oPlayer || !oThisData) return NULL;
        auto oCargo = oEntity.GetInventory().GetCargo();
        ItemData oItemData = NULL;

        ItemBase oItem = NULL;

        if (Class.CastTo(oItem, oEntity) && oEntity.IsItemBase())
        {
            oThisData.Name = oItem.GetType();
            oThisData.Version = 2;
            InventoryLocation location = new InventoryLocation;
            if (oItem.GetInventory().GetCurrentInventoryLocation(location))
            {
                oThisData.CargoIndex = location.GetIdx();
                oThisData.Flipped = location.GetFlip();
                oThisData.AttachmentSlot = location.GetSlot();
                oThisData.X = location.GetRow();
                oThisData.Y = location.GetCol();
            }
            if (oItem.HasEnergyManager()) oThisData.Energy = oItem.GetCompEM().GetEnergy();
            oThisData.LiquidType = oItem.GetLiquidType();
            oThisData.Temperature = oItem.GetTemperature();
            if (oItem.GetFoodStage()) oThisData.SavedFoodStage = oItem.GetFoodStage().GetFoodStageType();

            oThisData.Quantity = oThisData.GetItemQuantity(oItem);
            oThisData.ItemWet = oItem.GetWet();
            oThisData.ItemHealth = oItem.GetHealth("", "");
            
            Weapon_Base weapon = Weapon_Base.Cast(oItem);
            if (weapon)
            {
                oThisData.CurrentMuzzle = weapon.GetCurrentMuzzle();
                for (int muzzle = 0; muzzle < weapon.GetMuzzleCount(); muzzle++)
                {
                    float damage;
                    string ammoType;
                    if (!weapon.IsChamberEmpty(muzzle) && weapon.GetCartridgeInfo(muzzle, damage, ammoType))
                    {
                        COMCartridgeData chamber = new COMCartridgeData;
                        chamber.SavedMuzzle = muzzle;
                        chamber.InChamber = true;
                        chamber.Damage = damage;
                        chamber.SavedBulletType = ammoType;
                        oThisData.Cartridges.Insert(chamber);
                    }
                    for (int cartridge = 0; cartridge < weapon.GetInternalMagazineCartridgeCount(muzzle); cartridge++)
                    {
                        if (!weapon.GetInternalMagazineCartridgeInfo(muzzle, cartridge, damage, ammoType)) continue;
                        COMCartridgeData round = new COMCartridgeData;
                        round.SavedMuzzle = muzzle;
                        round.Damage = damage;
                        round.SavedBulletType = ammoType;
                        oThisData.Cartridges.Insert(round);
                    }
                }
            }
            oThisData.IQuickBarIndex = oPlayer.FindQuickBarEntityIndex(oItem);
        }

		if (oCargo)
		{
            for (int iCargo = 0; iCargo < oCargo.GetItemCount(); iCargo++)
            {
                EntityAI oCEntity = oCargo.GetItem(iCargo);

                if (oCEntity)
                {
                    oItemData = new ItemData;
                    if (ItemSave.Save(oCEntity, oPlayer, oItemData)) {
                        oThisData.NumItems++;

                        oItemData.LocationType = COM_CARGO_TYPE; // Cargo

                        oThisData.Items.Insert(oItemData);
                    }
                }
            }
        }

        for (int iAttachment = 0; iAttachment < oEntity.GetInventory().AttachmentCount(); iAttachment++)
		{
			EntityAI oAttachment = oEntity.GetInventory().GetAttachmentFromIndex(iAttachment);

            if (oAttachment)
            {
                oItemData = new ItemData;
                if (ItemSave.Save(oAttachment, oPlayer, oItemData)) {
                    oThisData.NumAttachments++;

                    oItemData.LocationType = COM_ATTACHMENT_TYPE; // Attachment

                    oThisData.ItemAttachments.Insert(oItemData);
                }
            }
        }

        return oThisData;
    }
}