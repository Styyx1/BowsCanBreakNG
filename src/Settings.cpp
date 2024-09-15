#include "Settings.h"

void Settings::LoadSettings() noexcept
{
    logger::info("Loading settings");

    CSimpleIniA ini;

    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\BowsCanBreakNG.ini)");

    debug_logging = ini.GetBoolValue("Log", "Debug");
    destry = ini.GetBoolValue("General", "bDestroyBow");
    spawn_wood = ini.GetBoolValue("General", "bSpawnWood");

    if (debug_logging) {
        spdlog::set_level(spdlog::level::debug);
        logger::debug("Debug logging enabled");
    }

    // Load settings

    logger::info("Loaded settings");
    logger::info("");
}

void Settings::LoadForms() noexcept
{
    const int wood = 0x6f993;

    auto dataHandler = RE::TESDataHandler::GetSingleton();
    firewood = dataHandler->LookupForm<RE::TESObjectMISC>(wood, "Skyrim.esm");
}
