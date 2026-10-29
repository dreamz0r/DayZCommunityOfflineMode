// Vanilla military test kit, shared identically by all four offline missions.
static void COM_LoadoutPrepare(EntityAI item)
{
    if (!item) return;
    item.SetHealth01("", "", 1);
    int batterySlot = InventorySlots.GetSlotIdFromString("BatteryD");
    if (item.GetInventory().HasAttachmentSlot(batterySlot) && !item.GetInventory().FindAttachment(batterySlot))
        item.GetInventory().CreateAttachment("Battery9V");
}

static EntityAI COM_LoadoutAttach(EntityAI parent, string type, string slot = "")
{
    if (!parent) { Print("[COM_LOADOUT_FAILED] No parent for " + type); return NULL; }
    EntityAI item;
    if (slot == "") item = parent.GetInventory().CreateAttachment(type);
    else item = parent.GetInventory().CreateAttachmentEx(type, InventorySlots.GetSlotIdFromString(slot));
    if (!item) Print("[COM_LOADOUT_FAILED] Attachment " + type + " on " + parent.GetType() + " slot " + slot);
    COM_LoadoutPrepare(item);
    return item;
}

static EntityAI COM_LoadoutCargo(EntityAI parent, string type)
{
    if (!parent) return NULL;
    EntityAI item = parent.GetInventory().CreateInInventory(type);
    if (!item) Print("[COM_LOADOUT_FAILED] Cargo " + type + " in " + parent.GetType());
    COM_LoadoutPrepare(item);
    return item;
}

static Weapon_Base COM_LoadoutWeapon(EntityAI parent, string type, string slot, string magazineType, TStringArray attachments)
{
    Weapon_Base weapon = Weapon_Base.Cast(COM_LoadoutAttach(parent, type, slot));
    if (!weapon) return NULL;
    foreach (string attachment : attachments) COM_LoadoutAttach(weapon, attachment);
    if (!weapon.SpawnAmmo(magazineType, WeaponWithAmmoFlags.CHAMBER))
        Print("[COM_LOADOUT_FAILED] Ammunition for " + type);
    return weapon;
}

static void COM_LoadoutRefill(PlayerBase player)
{
    array<EntityAI> items = new array<EntityAI>;
    player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
    foreach (EntityAI entity : items)
    {
        ItemBase item = ItemBase.Cast(entity);
        if (!item) continue;
        item.SetHealth01("", "", 1);
        item.SetWet(0);
        if (item.GetType() == "Canteen") item.SetLiquidType(LIQUID_WATER);
        if (item.HasQuantity()) item.SetQuantity(item.GetQuantityMax());
        Magazine magazine = Magazine.Cast(item);
        if (magazine) magazine.ServerSetAmmoCount(magazine.GetAmmoMax());
        if (item.HasEnergyManager()) item.GetCompEM().SetEnergy(item.GetCompEM().GetEnergyMax());
    }
}

static void COM_EquipStartingLoadout(PlayerBase player)
{
    if (!player) return;
    COM_LoadoutAttach(player, "BDUJacket", "Body");
    COM_LoadoutAttach(player, "BDUPants", "Legs");
    COM_LoadoutAttach(player, "TacticalGloves_Green", "Gloves");
    COM_LoadoutAttach(player, "Armband_Green", "Armband");
    COM_LoadoutAttach(player, "TacticalGoggles", "Eyewear");
    EntityAI mask = COM_LoadoutAttach(player, "AirborneMask", "Mask");
    COM_LoadoutAttach(mask, "GasMask_Filter");

    EntityAI helmet = COM_LoadoutAttach(player, "Mich2001Helmet", "Headgear");
    COM_LoadoutAttach(helmet, "NVGoggles", "NVG");
    COM_LoadoutAttach(helmet, "UniversalLight", "helmetFlashlight");
    EntityAI boots = COM_LoadoutAttach(player, "MilitaryBoots_Brown", "Feet");
    COM_LoadoutAttach(boots, "CombatKnife", "Knife");

    EntityAI vest = COM_LoadoutAttach(player, "PlateCarrierVest_Green", "Vest");
    COM_LoadoutAttach(vest, "PlateCarrierPouches_Green", "VestPouch");
    EntityAI vestHolster = COM_LoadoutAttach(vest, "PlateCarrierHolster_Green", "VestHolster");
    COM_LoadoutAttach(vest, "M67Grenade", "VestGrenadeA");
    COM_LoadoutAttach(vest, "M67Grenade", "VestGrenadeB");
    COM_LoadoutAttach(vest, "RDG2SmokeGrenade_White", "VestGrenadeC");
    COM_LoadoutAttach(vest, "FlashGrenade", "VestGrenadeD");

    EntityAI belt = COM_LoadoutAttach(player, "MilitaryBelt", "Hips");
    EntityAI canteen = COM_LoadoutAttach(belt, "Canteen", "Belt_Left");
    EntityAI sheath = COM_LoadoutAttach(belt, "NylonKnifeSheath", "Belt_Back");
    EntityAI knife = COM_LoadoutAttach(sheath, "CombatKnife", "Knife");
    EntityAI beltHolster = COM_LoadoutAttach(belt, "PlateCarrierHolster_Green", "Belt_Right");

    EntityAI backpack = COM_LoadoutAttach(player, "AliceBag_Camo", "Back");
    COM_LoadoutAttach(backpack, "Chemlight_Green", "Chemlight");
    COM_LoadoutAttach(backpack, "PersonalRadio", "WalkieTalkie");
    COM_LoadoutAttach(backpack, "Hatchet", "Backpack_1");
    EntityAI nbc = COM_LoadoutAttach(backpack, "NBCPack_Grey", "NBCBag");
    COM_LoadoutAttach(nbc, "NBCHoodGray", "NBCHead");
    COM_LoadoutAttach(nbc, "NBCJacketGray", "NBCTop");
    COM_LoadoutAttach(nbc, "NBCGlovesGray", "NBCGloves");
    COM_LoadoutAttach(nbc, "NBCPantsGray", "NBCPants");
    COM_LoadoutAttach(nbc, "NBCBootsGray", "NBCBoots");

    Weapon_Base m4 = COM_LoadoutWeapon(player, "M4A1", "Shoulder", "Mag_STANAG_60Rnd", {"M4_MPBttstck_Green", "M4_RISHndgrd_Green", "ACOGOptic_6x", "UniversalLight", "M4_Suppressor"});
    Weapon_Base svd = COM_LoadoutWeapon(player, "SVD", "Melee", "Mag_SVD_10Rnd", {"PSO1Optic", "AK_Suppressor"});
    Weapon_Base pistol = COM_LoadoutWeapon(vestHolster, "FNX45", "Pistol", "Mag_FNX45_15Rnd", {"FNP45_MRDSOptic", "TLRLight", "PistolSuppressor"});
    COM_LoadoutWeapon(beltHolster, "Glock19", "Pistol", "Mag_Glock_15Rnd", {"FNP45_MRDSOptic", "TLRLight", "PistolSuppressor"});

    // Largest cargo items first to avoid fragmenting the available clothing space.
    EntityAI blowtorch = COM_LoadoutCargo(player, "Blowtorch");
    COM_LoadoutAttach(blowtorch, "SmallGasCanister", "GasCanister");
    TStringArray repairKits = {"TireRepairKit", "WeaponCleaningKit", "ElectronicRepairKit", "LeatherSewingKit", "SewingKit", "EpoxyPutty", "DuctTape", "Whetstone"};
    foreach (string repairKit : repairKits) COM_LoadoutCargo(player, repairKit);

    // One spare magazine per weapon, separate from its loaded magazine.
    COM_LoadoutCargo(player, "M9A1_Bayonet");
    EntityAI m4Magazine = COM_LoadoutCargo(player, "Mag_STANAG_60Rnd");
    EntityAI svdMagazine = COM_LoadoutCargo(player, "Mag_SVD_10Rnd");
    COM_LoadoutCargo(player, "Mag_FNX45_15Rnd");
    COM_LoadoutCargo(player, "Mag_Glock_15Rnd");
    TStringArray ammunition = {"Ammo_556x45", "Ammo_762x54", "Ammo_45ACP", "Ammo_9x19"};
    foreach (string ammo : ammunition) COM_LoadoutCargo(player, ammo);
    for (int battery = 0; battery < 4; battery++) COM_LoadoutCargo(player, "Battery9V");
    COM_LoadoutCargo(player, "GasMask_Filter");
    COM_LoadoutCargo(player, "GasMask_Filter");

    // One full package/item of each usable medicine and wound-treatment supply.
    EntityAI bandage = COM_LoadoutCargo(player, "BandageDressing");
    TStringArray medicines = {"Morphine", "Epinephrine", "AntiChemInjector", "SalineBagIV", "TetracyclineAntibiotics", "CharcoalTablets", "PainkillerTablets", "VitaminBottle", "ChelatingTablets", "PurificationTablets", "IodineTincture", "DisinfectantSpray", "DisinfectantAlcohol"};
    foreach (string medicine : medicines) COM_LoadoutCargo(player, medicine);

    // Water only: one canteen on the belt and one additional canteen in cargo.
    COM_LoadoutCargo(player, "Canteen");
    EntityAI rangefinder = COM_LoadoutCargo(player, "Rangefinder");
    EntityAI compass = COM_LoadoutCargo(player, "Compass");
    COM_LoadoutRefill(player);
    player.UpdateQuickBarExtraSlots();
    if (m4) player.SetQuickBarEntityShortcut(m4, 0, true);
    if (svd) player.SetQuickBarEntityShortcut(svd, 1, true);
    if (pistol) player.SetQuickBarEntityShortcut(pistol, 2, true);
    if (knife) player.SetQuickBarEntityShortcut(knife, 3, true);
    if (bandage) player.SetQuickBarEntityShortcut(bandage, 4, true);
    if (canteen) player.SetQuickBarEntityShortcut(canteen, 5, true);
    if (rangefinder) player.SetQuickBarEntityShortcut(rangefinder, 6, true);
    if (compass) player.SetQuickBarEntityShortcut(compass, 7, true);
    if (m4Magazine) player.SetQuickBarEntityShortcut(m4Magazine, 8, true);
    if (svdMagazine) player.SetQuickBarEntityShortcut(svdMagazine, 9, true);
}