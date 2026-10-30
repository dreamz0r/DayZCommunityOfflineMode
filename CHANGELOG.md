# Changelog

## 2026-10-30

### Revision 2

#### Changed
- Autorun uses Z, Shift+Z for sprint and Ctrl+Z for walk; manual forward/backward input or opening COM cancels it. X is no longer bound to autorun.
- City, town, village and other map names use smooth white lettering with preserved label visibility; removed the Destination marker and Teleport selected button.
- Changelog submissions have numbered revisions within each date, with the latest revision first.

#### Fixed
- COM blocks character controls and interrupts ongoing actions while open; closing restores only its own input restrictions.

### Revision 1

#### Added
- Infinite stamina and unlimited ammo beside God mode in Y > G > Scripts; independent toggles survive respawn, magazines stay full and firing wear remains visible.
- Map teleport in a 25% smaller window with VPP-style satellite detail/deep zoom and bundled English labels; double-click/H/End teleport with destination checks and silent H/map success.

#### Fixed
- Death-menu Restart respawns without rebuilding the world or economy and keeps corpse interactions working.

#### Changed
- Starting kit keeps only its attached canteen, mask filter and equipment batteries; loose spares removed.

## 2026-10-29

### Revision 1

#### Fixed
- Invisible admin windows, Escape closing and camera movement behind the GUI.
- Object-browser freezes from survivor/animal previews; scrolling now delays safe item previews.
- Camera bounds, missed raycasts, weapon refills and vehicle presets.
- Scene save/load, character persistence, inventory slots and menu cleanup.
- Leap-year dates and weather controls changing the clock.

#### Changed
- Four deployable maps: Chernarus, Livonia, Sakhal and Nasdara.
- Configurable external mod folder, initially `P:\DayZCommunityOfflineMode\Mods`.
- Full military starting kit, equipped M4A1/SVD, populated quickbar and full consumables.
- One spare magazine per weapon, one stack per caliber, repair kits and medicines.
- Installation instructions updated for the standalone launchers and current setup.
