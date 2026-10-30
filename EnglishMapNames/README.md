# English map names

The complete English/Latin-script label set and four ready-to-load terrain add-ons are bundled here. The launcher loads the selected terrain's `@COMEnglishMapNames_<terrain>` folder relative to the project root. Copy this whole folder with the project; no external translation files, build step or DayZ Tools installation are required.

`Names.json` is the editable source table. Each terrain folder also includes its generated `config.cpp` and configuration-only PBO. Proper names are romanized; familiar English names such as Green Mountain are used where appropriate. The 477 named locations keep their original positions and types.

Set `EnglishMapNames` to `false` in `LauncherSettings.json` to use the original labels. For maintenance after editing `Names.json`, rebuild the bundled add-on with `Tools/Build-EnglishMapNames.ps1 -World ChernarusPlus` (or Enoch, Sakhal, Nasdara). Normal launching only reads the bundled files.
