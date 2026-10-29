# DayZCommunityOfflineMode 4.1

The DayZ Community Offline Mode offers the ability, to explore the world of DayZ in a singleplayer environment. While this mode does not represent the actual DayZ gameplay it offers other possibilites for developers, content creators and player who do want to experiment around but not loose all their progress from online.

This is UNOFFICIAL, so don't blame the DayZ development team if this is not working at all for you!

# Info for server owners & mappers
There is now a wiki page available on how to use the community offline mode to spawn map objects on your server or mission you are working on: [Add custom objects to your server or mission](https://github.com/Arkensor/DayZCommunityOfflineMode/wiki/Add-custom-objects-to-your-server-or-mission)

## DayZ 1.30 compatibility fixes

The missions were checked against the locally extracted DayZ 1.30 scripts and the installed 1.30.164200 diagnostic client.

- Use the global GetMousePos function for preview rotation. Remove invalid new ref construction in the Livonia module manager.
- Export scenes using CreateObjectEx, with a guard if an object cannot be created.
- Guard empty raycast results, missing input bindings, failed weapon/optic creation, and invalid object-spawner classnames.
- Bound float-formatting substrings to the available digits.
- Set mission-controlled weather explicitly; keep overcast/rain/fog limits within 0..1. Fix wind slider units, zero maximum wind speed, the date slider month index, and weather update cleanup.
- Resolve launcher paths relative to the batch file, quote paths, check for the executable, and accept extra launch arguments. Launchers preserve storage and existing DayZ processes.

Use a root deploy launcher to copy the latest mission source into the configured DayZ installation and start it.

Character persistency remains disabled as shipped; its unfinished module is not covered by these fixes. Do not enable it by uncommenting its include.

Validation: Chernarus and Livonia compiled in the native 1.30.164200 client. Both initialized the offline economy and spawned a player. The diagnostic executable has an additional vanilla PluginInventoryDebug null reference; the regular client passed both Chernarus and Livonia startups without script errors. Interactive editor, camera, export, weather and inventory behavior still require hands-on testing. The four-map source tree has 236 resolvable include references. Diagnostic test copies and logs are stored under D:\Codex\OfflineMode-1.30.
## Four deployable offline maps

These launch the existing single-player Community Offline Mode, with its local economy and admin tools.

| Map | Root launcher | Mission folder |
| --- | --- | --- |
| Chernarus | `deployChernarus.bat` | `Missions\DayZCommunityOfflineMode.ChernarusPlus` |
| Livonia | `deployLivonia.bat` | `Missions\DayZCommunityOfflineMode.Enoch` |
| Sakhal | `deploySakhal.bat` | `Missions\DayZCommunityOfflineMode.Sakhal` |
| Nasdara | `deployNasdara.bat` | `Missions\DayZCommunityOfflineMode.Nasdara` |

All four share `LauncherSettings.json` and the external `Mods` folder. Each mission retains separate economy storage. Deployment copies source without deleting existing storage. `ChernarusPlus` and `Enoch` remain accepted launcher aliases for the engine's terrain names.

Sakhal and Nasdara use vanilla terrain economy data from the locally extracted DayZ 1.30 sources (`DZ\worlds\<terrain>\ce`). Their offline player spawns use the terrain's fresh-spawn candidates, filter sea/pond locations, and set the position to terrain height. Sakhal starts on February 1 and Nasdara on April 30 at noon, matching the vanilla mission season. The GUI visibility, Escape-close, camera lock, and mod-folder fixes are included in all four missions.

Validation on 2026-10-29: Sakhal and Nasdara both compiled and loaded in the regular DayZ 1.30.164200 client with no script errors. Their local economies initialized, players spawned on land at terrain height, and the admin GUI opened with aiming blocked. All four launcher previews passed; 236 include references, 136 XML files, and 14 JSON files passed source checks. Full gameplay and every spawn candidate have not been exercised.

Vanilla mission data is by Bohemia Interactive; see [DayZ Central Economy](https://github.com/BohemiaInteractive/DayZ-Central-Economy) and its [ADPL-SA license](https://www.bohemia.net/community/licenses/arma-and-dayz-public-license-share-alike-adpl-sa).

Namalsk is retired from the active project. A local backup of its previous source and old root launchers is under `D:\Codex\OfflineMode-MapMigration-2026-10-29\Retired`. Previously installed Namalsk missions are not deleted automatically.
## Mods outside the game folder

The project root contains `LauncherSettings.json`. Its initial settings are:

```json
{
  "GamePath": "D:\\SteamLibrary\\steamapps\\common\\DayZ",
  "ModsPath": "P:\\DayZCommunityOfflineMode\\Mods",
  "EnabledMods": null
}
```

Place complete mod folders inside `Mods`, for example `Mods\@CF\Addons\...` and `Mods\@MyMod\Addons\...`. Keep each mod's normal folder structure. Mods stay in this folder; the launcher passes their absolute paths to DayZ. This does not compile or pack mod source into PBOs.

- `EnabledMods: null` loads every immediate subfolder containing an `Addons` folder, in alphabetical order.
- `EnabledMods: []` loads no optional mods.
- `EnabledMods: ["@CF", "@MyMod"]` loads only those mods, in the order listed. List dependencies first. Absolute mod paths are also accepted.
- Change `ModsPath` to use another folder. Relative paths are resolved from the settings file; names in `EnabledMods` are resolved from `ModsPath`.
- Sakhal and Nasdara require their corresponding installed DayZ DLC. No Namalsk or Workshop map dependency is used.

Run `deployChernarus.bat`, `deployLivonia.bat`, `deploySakhal.bat`, or `deployNasdara.bat` from the project root. They copy the current mission source into the configured game installation without deleting storage, then start DayZ with the selected mods. The mission batch files use the same launcher, falling back to `P:\DayZCommunityOfflineMode` when run from the game installation.

To preview the selected mods and command without copying files or starting DayZ:

```powershell
.\LaunchOfflineMode.ps1 -Map Chernarus -DryRun
```

Extra game arguments can be passed to the batch launcher, such as `-window`. Configure mods through `EnabledMods` rather than adding a second `-mod` argument. If the project is moved, use its root deploy launcher; update the project fallback path in the mission batch files if you also use those directly.
# Installation

Use the complete current project folder, including `LaunchOfflineMode.ps1`, `LauncherSettings.json`, the four root `deploy*.bat` launchers, `Missions`, and `Mods`. These instructions apply to this modified version of Community Offline Mode.

## Requirements

- A Windows installation of DayZ. This project has been tested with DayZ **1.30.164200**; compatibility with other builds has not been verified.
- The corresponding installed DLC to play Sakhal or Nasdara.
- Windows PowerShell, which the batch launchers use automatically.

## Setup and launch

1. Keep the project in its own folder. The current location is `P:\DayZCommunityOfflineMode`; the deploy launcher copies the selected mission into DayZ for you.
2. Open `LauncherSettings.json` in the project root and check the paths:

   ```json
   {
     "GamePath": "D:\\SteamLibrary\\steamapps\\common\\DayZ",
     "ModsPath": "P:\\DayZCommunityOfflineMode\\Mods",
     "EnabledMods": null
   }
   ```

   `GamePath` must point to the folder containing `DayZ_x64.exe`. `ModsPath` must point to an existing folder for your test mods. If you move the project, update these paths as needed.
3. Place any complete mods inside the configured mods folder, retaining their structure, for example `Mods\@MyMod\Addons\...`. Leave the folder empty to start without additional mods. `EnabledMods: null` loads all detected mods alphabetically; an explicit list such as `["@CF", "@MyMod"]` selects mods and their load order. Use `[]` to disable optional mods. See **Mods outside the game folder** above for details.
4. Close an existing DayZ session, then run the launcher for your chosen map from the project root:

   | Map | Launcher |
   | --- | --- |
   | Chernarus | `deployChernarus.bat` |
   | Livonia | `deployLivonia.bat` |
   | Sakhal | `deploySakhal.bat` |
   | Nasdara | `deployNasdara.bat` |

Each launch copies the current source for that map into `GamePath\Missions\DayZCommunityOfflineMode.<terrain>` and starts the regular DayZ client directly in the offline environment with your selected mods. Mods remain in `ModsPath`. Livonia uses the internal terrain name `Enoch`; Chernarus uses `ChernarusPlus`.

Deployment preserves existing mission storage and does not force-close running DayZ processes. Character-save persistence is disabled: a new session creates the configured military starting character. Each map has its own economy storage.

To check the configured paths, selected mods and launch command without copying mission files or starting DayZ, run a root launcher with `-DryRun`, for example:

```bat
deployChernarus.bat -DryRun
```

## Updating this version

Close DayZ, update the project source, and run the appropriate root deploy launcher again. Keep your `LauncherSettings.json`, test mods and any custom source edits when replacing project files. The launcher deploys the selected map on every normal launch; there is no separate manual mission-copy step.

## Uninstalling

Close DayZ and back up any offline economy storage or custom mission files you want to keep. Remove only this project's deployed mission folders from your configured `GamePath\Missions` folder:

- `DayZCommunityOfflineMode.ChernarusPlus`
- `DayZCommunityOfflineMode.Enoch`
- `DayZCommunityOfflineMode.Sakhal`
- `DayZCommunityOfflineMode.Nasdara`

Keep other missions and unrelated game files. You can also remove the separate project folder when you no longer need it, after retaining any test mods, settings or custom source you want to keep.

## Military starting gear

All four maps use the same `core/StartingLoadout.c`. The kit is equipped immediately after vanilla player initialization, so face protection and NVGs do not access uninitialized player effects.

- BDU jacket and trousers, tactical gloves/goggles, green armband, airborne mask with filter, tactical helmet with NVGs and light, military boots with knife, green plate carrier with pouches/holster and four grenade attachments, military belt with filled canteen, knife/sheath and holster, and camouflage field backpack with chemlight, battery-powered radio, hatchet and fully populated NBC pack.
- **M4A1 on Shoulder:** green MP stock, green RIS handguard, 6x ACOG, weapon light, NATO suppressor, full 60-round magazine and chambered round.
- **SVD on the second shoulder (Melee slot):** powered PSO-1, Eastern suppressor, full 10-round magazine and chambered round.
- Fully equipped and loaded FNX-45 in the vest holster and Glock in the belt holster, with compatible optics, lights, suppressors and charged batteries.
- Exactly one full spare magazine per weapon and one full loose-ammo stack per caliber (5.56x45, 7.62x54, .45 ACP, 9x19). Four spare batteries, spare filters, powered rangefinder and compass. No rifle wraps, food, or ordinary drinks. Two full water canteens: one on the belt and one in cargo.
- One each of all nine vanilla repair types: weapon cleaning kit, sewing kit, leather sewing kit, whetstone, duct tape, tire repair kit, electronics repair kit, epoxy putty and blowtorch with a full gas canister.
- One each: bandage dressing, morphine, epinephrine, PO-X antidote, saline IV, tetracycline, charcoal, painkillers, vitamins, chelating tablets, purification tablets, iodine, disinfectant spray and alcohol tincture. Tablet bottles and other consumables start at full quantity.

All equipment starts pristine and dry. Quantities, magazines, ammo stacks and battery/energy reserves are filled at creation; the canteen contains clean water. Consumables then deplete normally during use. Vanilla prevents attaching an M4 suppressor and bayonet together, so the bayonet is carried as a spare. Internal character placeholders, empty hands and the injury-only splint slot are intentionally left alone.

| Shortcut | Item |
| --- | --- |
| 1 | M4A1 |
| 2 | SVD |
| 3 | FNX-45 |
| 4 | Combat knife |
| 5 | Bandage |
| 6 | Canteen |
| 7 | Rangefinder |
| 8 | Compass |
| 9 | Spare 60-round M4 magazine |
| 0 | Spare SVD magazine |
# Controls
* Y (Z for QWERTZ-Keyboard) - Open the COM toolbar menu.
* X - Toggle Autojog/walk/run
* X + SHIFT - Enable Autorun (Just X to disable it again)
* X + CTRL - Enable Autowalk (Just X to disable it again)
* END - Teleport at the position you are looking at
* O - Spawn a random infected (Zombies)
* O + CTRL - Spawn a wolf (Agressive and will fight both players and infected)
* O + SHIFT - Spawn a random animal
* R - Reload the weapon and refill the ammo (Infinite ammo)
* P - Display your current position in the chat and print it to your logfiles (See logfiles section for their location)
* B - Toggle debug monitor
* INSERT - Toggle free camera. This teleports your player to the position you looked at when turning it off

# Object Editor Controls
* Click objects to select them.  
* Click and drag objects to move them.
* Click on nothing to deselect the current object.
* Middle Click to snap to ground (Might not be accurate)
* Spawn in new items using the object spawner menu that can be found in the toolbar.
* You can either enter values on the onject editor gui or hover above the value with your mouse and use the scroll wheel to in-/decrease them.

# Loot and infected spawn
By default the "Hive" that is repsonsible for spawning loot and infected is **enabled**. If you want to prevent loot and infected from spawning you need to follow this guide: [Toggle loot and infected spawn](https://github.com/Arkensor/DayZCommunityOfflineMode/wiki/Toggle-loot-and-infected-spawn)  
> Disabling the hive increases the game performace.

# Logfiles
In case you want to report errors to us or the offical dayz dev team, you might need logfile info.
We also save the positions you printed ingame in it so that you might revisit them later on by saving them in some textfile.
Locations are stored inside the script.log for now.

You find your logfiles here: ```Press WINDOWS + R  -> Type in %localappdata%/DayZ -> Hit enter```. 

# Contributors:
* [DannyDog](https://github.com/DannyDog) - Inital core developer
* [Jacob_Mango](https://github.com/Jacob-Mango) - Inital core developer
* [gallexme](https://github.com/gallexme) - Suggestion for the mission based version
* [DuhOneZ](https://twitter.com/DuhOneZ) - Code snippets
* [Watchman](https://twitter.com/watchman113) - Documentation
* [n8m4re](https://github.com/n8m4re) - Code: SaveManager
* [wriley](https://github.com/wriley) - Code snippets beards
* [PR9INICHEK](https://github.com/PR9INICHEK) - Object spawner additions
* [LiPeK](https://github.com/LiPeK) - Object editor additions
* [Cypher](https://github.com/CypherMediaGIT) - Livonia positions
* [BigBenREDUX](https://twitter.com/BigBenREDUX) - Namalsk positions
* [LieutenantMaster](https://github.com/LieutenantMaster) - Maintainance support
