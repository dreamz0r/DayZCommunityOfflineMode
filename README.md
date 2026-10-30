# DayZ Community Offline Mode

A single-player DayZ environment for testing mods, exploring maps and using COM admin tools. Supports Chernarus, Livonia, Sakhal and Nasdara.

## Installation

1. Keep the complete project folder together. Requires Windows, DayZ and the corresponding DLC for Sakhal or Nasdara.
2. Edit `LauncherSettings.json` to match your DayZ installation and mods folder.
3. Close DayZ, then run a launcher from the project root:

| Map | Launcher | Mission terrain name |
| --- | --- | --- |
| Chernarus | `deployChernarus.bat` | `ChernarusPlus` |
| Livonia | `deployLivonia.bat` | `Enoch` |
| Sakhal | `deploySakhal.bat` | `Sakhal` |
| Nasdara | `deployNasdara.bat` | `Nasdara` |

The launcher deploys the mission, prepares COM support automatically and loads your selected mods. Existing economy storage is preserved. After updating the project, restart through the same root launcher.

## LauncherSettings.json

```json
{
  "GamePath": "D:\\SteamLibrary\\steamapps\\common\\DayZ",
  "ModsPath": "P:\\DayZCommunityOfflineMode\\Mods",
  "EnabledMods": null,
  "EnglishMapNames": true
}
```

| Setting | Meaning |
| --- | --- |
| `GamePath` | DayZ folder containing `DayZ_x64.exe`. |
| `ModsPath` | Existing folder containing your test mods. Relative paths resolve from the settings file. |
| `EnabledMods` | `null`: load detected mods alphabetically. `[]`: no optional mods. `["@CF", "@MyMod"]`: load only those mods, in that order. Names resolve from `ModsPath`; absolute paths also work. |
| `EnglishMapNames` | `true`: load the bundled English/Latin location names. `false`: use original map labels. |

To preview deployment paths and selected mods without launching, run `deployChernarus.bat -DryRun` (or another map's launcher).

## Using mods

Place complete, packed mod folders inside `ModsPath`, keeping their structure, for example `Mods\@MyMod\Addons\...`. Mods stay outside the game directory. To control load order, list them in `EnabledMods`, with dependencies first.

COM support and bundled map labels are managed separately from optional mods. No VPP or CF installation is required for COM itself.

## Hive settings

Hive initializes the local economy, including loot and infected. It is enabled by default.

Edit the selected mission's `Missions\DayZCommunityOfflineMode.<terrain>\core\CommunityOfflineClient.c`:

```c
protected bool HIVE_ENABLED = true;
```

Set it to `false` to skip COM's hive initialization, or `true` to enable it. Restart through the root launcher after changing it.

## Keybinds

| Key | Action |
| --- | --- |
| Y | Open the COM toolbar. |
| Escape | Close the current COM window, then the toolbar. |
| Z | Toggle autojog; press again to stop. |
| Shift + Z / Ctrl + Z | Enable sprint / autowalk. |
| Forward / backward (W/S by default) | Cancel autorun. Opening COM also cancels it. |
| H / End | Teleport to the mouse cursor, or crosshair when the cursor is hidden. |
| Insert | Toggle free camera; returning teleports the player to the camera's target position. |
| O / Ctrl + O / Shift + O | Spawn an infected / wolf / random animal. |
| R | Refill and chamber the held weapon. |
| P | Show and log player position. |
| B | Toggle debug monitor. |
| Home | Toggle HUD. |

Free camera: **WASD** moves, **Q/Z** moves up/down, **Shift** increases movement speed, and the **mouse wheel** adjusts speed. **Ctrl + right mouse** zooms, **\** freezes the camera, **Caps Lock** freezes the player, **middle mouse** selects a target, **[** follows it and **]** toggles orbit.

## Added COM features

- **Y > G > Scripts:** independent God mode, Infinite stamina and Unlimited ammo. Ammo stays full while normal weapon/suppressor wear and jams remain. Toggles survive respawn and reset with a new session.
- **Y > MP:** detailed satellite map with smooth white location names. Double-click to teleport; only the player marker is shown. H and map teleports are silent on success.
- Character actions, movement and aiming are blocked while COM is open. Ongoing actions and autorun are interrupted.
- Object spawning/editing, vehicle presets, weather/time controls and free camera. Click objects to select them, drag to move and middle-click to snap to ground.
- Full military starting kit with equipped M4A1/SVD, sidearms, quickbar, repair kits and medicines. One spare magazine per weapon and one ammo stack per caliber; batteries, canteen and filter stay attached to their equipment.
- **Respawn** in the pause menu replaces the character and restores the kit without rebuilding the world. Each map has separate economy storage; character-save persistence is disabled by default.
