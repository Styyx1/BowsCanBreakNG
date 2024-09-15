#pragma once
#include "Settings.h"

class Utility : public Singleton<Utility>
{
public:
    inline bool IsPowerAttacking(RE::Actor* a_actor)
    {		
        auto atk_data = a_actor->GetActorRuntimeData().currentProcess->high->attackData.get();
        if (atk_data != nullptr && atk_data->data.flags == RE::AttackData::AttackFlag::kPowerAttack) {
            return true;
        } else {
            return false;
        }
    }
    inline static RE::TESObjectWEAP* getWieldingWeapon(RE::Actor* a_actor)
    {
        bool dual_wielding = false;
        auto weapon        = a_actor->GetAttackingWeapon();
        if (weapon) {
            dual_wielding = false;
            return weapon->object->As<RE::TESObjectWEAP>();
        }
        auto rhs = a_actor->GetEquippedObject(false);
        if (rhs && rhs->IsWeapon()) {
            dual_wielding = false;
            return rhs->As<RE::TESObjectWEAP>();
        }
        auto lhs = a_actor->GetEquippedObject(true);
        if (lhs && lhs->IsWeapon()) {
            dual_wielding = false;
            return lhs->As<RE::TESObjectWEAP>();
        }

        return nullptr;
    }
    inline static void ProcessWeaponLoss(RE::Actor* defender,  RE::TESObjectWEAP* defender_weap) {
        Settings* settings = Settings::GetSingleton();
        if (settings->destry) {
            defender->RemoveItem(defender_weap, 1, RE::ITEM_REMOVE_REASON::kRemove, nullptr, nullptr, nullptr);
            logger::debug("destroyed {}", defender_weap->GetName());
            if (settings->spawn_wood) {
                defender->PlaceObjectAtMe(settings->firewood, false);
            }
        }
        else {
            defender->RemoveItem(defender_weap, 1, RE::ITEM_REMOVE_REASON::kDropping, nullptr, nullptr, nullptr);
            logger::debug("dropped {}", defender_weap->GetName());
        }        
    }
};
