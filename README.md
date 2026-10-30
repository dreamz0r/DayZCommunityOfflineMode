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

Validation: Chernarus and Livonia compiled in the native 1.30.164200 client. Both initialized the offline economy and spawned a player. The diagnostic executable has an additional vanilla PluginInventoryDebug null reference; the regular client passed both Chernarus and Livonia startups without script errors. Interactive editor, camera, export, weather and inventory behavior still require hands-on testing. The four-map source tree has 240 resolvable include references. Diagnostic test copies and logs are stored under D:\Codex\OfflineMode-1.30.
## Script audit against the refreshed DayZ sources

On 2026-10-29 the active mission scripts were compared with the newly extracted vanilla scripts on `P:` and checked in the installed DayZ 1.30.164200 client. Fixes were applied consistently to all four maps:

- Camera targeting uses the required two-vector collision bounds and an actual model-space center; autofocus handles raycast misses.
- Cursor misses retain the player's position, and teleporting into empty sky is refused.
- Scene save/load uses the current JSON API with explicit failure handling. Missing saves, unavailable objects and empty selections are handled safely. The existing save path and data fields are retained.
- Object previews are local and cleaned up when hidden. Only inventory items, weapons and magazines are previewed after a 250 ms pause; survivors, animals, effects and world objects remain selectable for spawning without being instantiated while browsing. Pending previews and drag callbacks are cancelled when the window closes or selection/search changes. The spawner honors magazine ammunition quantities, uses magazine capacity for its preview value, and clears list selection when a classname is entered manually. The list shows public classes.
- Reloading handles attached and internal magazines, chambering, and compatible suppressor repair. Autorun respects the admin menu and free camera; player-dependent updates wait for a player.
- Active vehicle presets no longer request removed/mismatched/duplicate parts; batteries are charged and fluids use each vehicle's configured capacity.
- Weather date controls handle leap years. Overcast changes no longer add five minutes to the clock. Camera guide widgets and update callbacks are cleaned up when their window closes.

The starting kit, four-map selection, spawn candidates, external mod configuration and source-only delivery remain unchanged. The optional legacy modules were subsequently repaired as described below.

Validation: **39 native regression assertions passed**, covering scene round-trip/error handling, camera centering, magazine quantities, weapon refills, UI cleanup, dates, sky raycasts and all five active vehicle presets. The deliberate malformed-JSON case produces an expected engine decoder log; there were no unexpected script errors. All 248 checked include references resolve; 136 XML and 14 JSON files parse. Native regression execution used the Chernarus mission; the common fixes were checked for consistency across all four source trees. Full driving, shooting and hands-on visual testing were not performed.

### Legacy-module fixes

The optional character-save code has also been updated for DayZ 1.30. It uses the current stomach API, checked JSON loading/saving, cargo locations including rotation, explicit attachment slots, and safe handling of missing items. Saves retain consumable quantities, battery energy, canteen liquids, quickbar assignments, hand-held items, chambered rounds and internal magazines. Versionless older character saves remain readable; their obsolete aggregate stomach fields have no direct modern equivalent and are ignored. Character persistence remains disabled by default, preserving the normal military-kit startup.

The character menu no longer depends on removed pause-menu classes or unsafe update callbacks. The legacy scene library now retains its object data, reads every file from `$profile:SceneManager\Scenes`, supports checked save/load and explicit activation, and cleans up spawned objects. Its object-scene format is separate from the regular editor's existing save file. Obsolete bus, van and V3S chassis stubs were removed; the vehicle menu now offers the current M3S cargo truck with valid parts.

Validation: 168 focused assertions passed in the regular 1.30.164200 client with the optional persistence classes compiled. These cover complete kit save/load, item locations, quickbar, consumables, hands, chamber/internal-magazine rounds, scene loading/activation/cleanup, truck parts and character-menu open/close. The final run reported no script errors or VM exceptions. Common sources are synchronized across all four maps; native execution used Chernarus. Full driving/shooting, manual character-creation workflows and multiplayer have not been tested. Test helpers, logs and recovery copies are under `D:\Codex\OfflineMode-LegacyFixes-2026-10-29`.

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

Validation on 2026-10-29: Sakhal and Nasdara both compiled and loaded in the regular DayZ 1.30.164200 client with no script errors. Their local economies initialized, players spawned on land at terrain height, and the admin GUI opened with aiming blocked. All four launcher previews passed; 240 include references, 136 XML files, and 14 JSON files passed source checks. Full gameplay and every spawn candidate have not been exercised.

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
- `EnabledMods: []` loads no optional mods. COM support and the selected bundled English-name add-on are managed separately.
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

Use the complete current project folder, including `LaunchOfflineMode.ps1`, `LauncherSettings.json`, `Tools`, `Support`, `EnglishMapNames`, the four root `deploy*.bat` launchers, `Missions`, and `Mods`. These instructions apply to this modified version of Community Offline Mode.

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

English map labels are enabled by default and bundled in `EnglishMapNames\@COMEnglishMapNames_<terrain>`. The launcher loads the selected map's add-on relative to the project root. Copy the complete project to share it; no external translation folder, generated-data path or build step is required. Set `EnglishMapNames` to `false` to use the original labels. The editable source table and maintenance builder are included, and game terrain files remain unchanged. The launcher also builds/loads the included map and ammunition support sources automatically.

After dying, choose **Respawn** in the pause menu. This replaces the player and restores the starting kit without rebuilding the world or economy. Repeated button presses are coalesced; restarting while alive also replaces the character. To reload the whole world, exit and launch the mission again.

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
- Exactly one full spare magazine per weapon and one full loose-ammo stack per caliber (5.56x45, 7.62x54, .45 ACP, 9x19). Powered rangefinder and compass. No loose batteries, spare filters, rifle wraps, food, or ordinary drinks. One full water canteen in the belt attachment slot; one filter attached to the mask.
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
## Scripts in the G menu

Open the COM toolbar with **Y**, then choose **G**. Its Scripts panel contains three independent checkboxes:

- **God mode** prevents player damage without changing nutrition, ammo or stamina.
- **Infinite stamina** keeps stamina full and disables depletion. Turning it off removes COM's modifier and restores the other stamina modifiers.
- **Unlimited ammo** keeps the held weapon's existing magazine/internal magazine full, including after automatic cycling feeds the next round. Weapons still need their normal magazine/chamber setup. Weapon and suppressor wear, overheating, condition indicators and jams retain vanilla behavior; ammo refills never repair equipment.

Toggles remain active with the menu closed, carry over to an in-place respawn and reset with a new mission session. The separate PL player editor has been removed. No VPP or CF dependency is required; the current VPP reference sources are under `P:\VPP`.

The teleport map window is 25% smaller in width and height, with its center preserved. It uses DayZ's native terrain/satellite renderer with VPP-style detail and zoom settings. It uses the installed terrain's full available detail rather than an exported map image. The title reports whether the bundled English labels are loaded. Keep `EnglishMapNames: true` and restart through a root deploy launcher after updates; directly starting an old game command can omit the map add-ons.

The small runtime support add-on is built automatically from `Support\@COMSupport` by `Tools\Build-COMSupport.ps1`, using only Windows PowerShell. Generated files stay outside the source project: this machine uses `D:\Codex\OfflineMode-Support`; machines without `D:\Codex` use their local application-data cache. The project contains all required sources, and no VPP files or external translation files are needed.

COM blocks character actions, movement and aiming while its toolbar or windows are open. Opening it stops autorun and interrupts an ongoing action; closing restores normal controls.

# Controls
* Y - Open the COM toolbar menu. Choose MP for the teleport map; double-click to teleport. City, village and other location names use smooth white text; only your player marker is shown. Escape closes the map first.
* Z - Toggle autojog; press Z again to stop. Forward/backward movement (W/S by default) or opening COM also stops autorun.
* Z + SHIFT - Enable autorun/sprint (Z to stop).
* Z + CTRL - Enable autowalk (Z to stop).
* H / END - Teleport to the mouse cursor when visible, otherwise the crosshair. H and map teleports are silent on success.
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
