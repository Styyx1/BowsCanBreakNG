#pragma once

class Settings : public Singleton<Settings>
{
public:
    static void LoadSettings() noexcept;
    static void LoadForms() noexcept;

    inline static bool debug_logging{};
    inline static bool destry{};
    inline static bool spawn_wood{};
    inline static bool use_power_attack{};
    inline static float mass_threshold;
    inline static RE::TESObjectMISC* firewood;
};
