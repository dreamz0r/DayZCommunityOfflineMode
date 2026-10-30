class CfgPatches
{
    class COMSupport
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts", "DZ_Gear_Navigation"};
    };
};
class CfgMods
{
    class COMSupport
    {
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"COMSupport/Scripts/4_World"};
            };
        };
    };
};
// VPP enables the native terrain's satellite layer and detail/zoom settings.
// Inherit vanilla icons so COM does not need VPP's custom image files.
class MapDefaults;
class RscMapControl: MapDefaults
{
    scaleMin = 0.00005;
    scaleMax = 0.85;
    scaleDefault = 0.3;
    ptsPerSquareSea = 5;
    ptsPerSquareTxt = 20;
    ptsPerSquareCLn = 10;
    ptsPerSquareExp = 10;
    ptsPerSquareCost = 10;
    ptsPerSquareFor = 9;
    ptsPerSquareForEdge = 9;
    ptsPerSquareRoad = 6;
    ptsPerSquareObj = 9;
    maxSatelliteAlpha = 1;
    alphaFadeStartScale = 1;
    alphaFadeEndScale = 1;
    maxUserMapAlpha = 0;
    showCountourInterval = 0;
    colorForest[] = {0.36, 0.78, 0.08, 0};
    colorForestBorder[] = {0.4, 0.8, 0, 0};
    colorRocks[] = {0.5, 0.5, 0.5, 0.2};
    colorRocksBorder[] = {0.5, 0.5, 0.5, 0};
    colorCountlines[] = {0.85, 0.8, 0.65, 0.1};
    colorMainCountlines[] = {0.45, 0.4, 0.25, 0};
    fontNames = "gui/fonts/sdf_MetronBook24";
    sizeExNames = 0.03;
    colorNames[] = {1, 1, 1, 1};
    fontGrid = "gui/fonts/sdf_MetronBook24";
    fontLevel = "gui/fonts/sdf_MetronBook24";
    fontLabel = "gui/fonts/sdf_MetronBook24";
    fontInfo = "gui/fonts/sdf_MetronBook24";
};
// Preserve each vanilla base class so settlement draw styles remain inherited.
// Smooth SDF lettering and a shadow keep white labels legible over satellite imagery.
class CfgLocationTypes
{
    class Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Strategic: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Airport: Strategic { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class BorderCrossing: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Camp: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Capital: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class City: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class CityCenter: Strategic { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class FlatArea: Strategic { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class FlatAreaCity: FlatArea { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class FlatAreaCitySmall: FlatAreaCity { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Hill: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class IndustrialSite: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Local: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class LocalOffice: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Marine: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Mount { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class NameCity: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class NameCityCapital: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class NameLocal: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class NameMarine: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class NameVillage: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class RailroadStation: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class RockArea: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Ruin: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class StrongpointArea: Strategic { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class VegetationBroadleaf: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class VegetationFir: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class VegetationPalm: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class VegetationVineyard: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class ViewPoint: NameIcon { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
    class Village: Name { color[] = {1, 1, 1, 1}; font = "gui/fonts/sdf_MetronBook24"; shadow = 1; };
};
