#include "Events.h"

namespace Events
{
    RE::BSEventNotifyControl OnHitHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) noexcept
    {
        using HitFlag = RE::TESHitEvent::Flag;
        if (!a_event || !a_event->target || !a_event->cause || a_event->projectile) {
            return RE::BSEventNotifyControl::kContinue;
        }
        auto defender = a_event->target ? a_event->target->As<RE::Actor>() : nullptr;
        if (!defender) {
            return RE::BSEventNotifyControl::kContinue;
        }
        auto aggressor = a_event->cause ? a_event->cause->As<RE::Actor>() : nullptr;
        if (!aggressor) {
            return RE::BSEventNotifyControl::kContinue;
        }
        Utility* util = Utility::GetSingleton();
        const Settings* settings = Settings::GetSingleton();
        if (a_event->flags.any(HitFlag::kPowerAttack) && a_event->source || aggressor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMass) >= settings->mass_threshold) {
            logger::debug("aggressor mass is {}", aggressor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMass));
            auto attacking_weap = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);
            if (!defender || !attacking_weap || !defender->GetActorRuntimeData().currentProcess || !defender->GetActorRuntimeData().currentProcess->high || !attacking_weap->IsMelee() || !defender->Get3D())
            {
                logger::debug("power attack event, first continue");
                return RE::BSEventNotifyControl::kContinue;
            }
            if (!aggressor || !aggressor->GetActorRuntimeData().currentProcess || !aggressor->GetActorRuntimeData().currentProcess->high) {
                logger::debug("Attack Actor Not Found!");
                return RE::BSEventNotifyControl::kContinue;
            }
            auto data_aggressor = aggressor->GetActorRuntimeData().currentProcess->high->attackData;
            if (!data_aggressor) {
                logger::debug("Attacker Attack Data Not Found!");
                return RE::BSEventNotifyControl::kContinue;
            }
            auto defender_weap = util->getWieldingWeapon(defender);
            if (!defender_weap) {
                logger::debug("{} has nothing equipped", defender->GetName());
                return RE::BSEventNotifyControl::kContinue;
            }
            if (defender_weap && defender_weap->IsBow() || defender_weap->IsCrossbow()) {
                logger::debug("{}'s weapon is a bow", defender_weap->GetName());
                logger::debug("aggressor is {}", aggressor->GetName());
                logger::debug("{} has {} equipped", defender->GetName(), defender_weap->GetName());
                if ((defender_weap->HasKeywordString("REQ_BowBreakable") || defender_weap->HasKeywordString("BowBreakable")) && (defender->AsActorState()->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowDraw || defender->AsActorState()->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowDrawn)) {
                    logger::debug("weapon will get damaged");                    
                    util->ProcessWeaponLoss(defender, defender_weap);                    
                } 
            }          
        }
        return RE::BSEventNotifyControl::kContinue;
    }
} // namespace Events
