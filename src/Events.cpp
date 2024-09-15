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
        if (a_event->flags.any(HitFlag::kPowerAttack) && a_event->target && a_event->source || aggressor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMass) >= 2.0) {
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
            logger::debug("{} has weapon equipped", defender->GetName());
            auto defender_weap = defender->GetEquippedObject(false);

            
            auto rightHand = defender->GetEquippedObject(false);

            if (!rightHand) {
                logger::debug("{} has nothing in right hand", defender->GetName());
                return RE::BSEventNotifyControl::kContinue;
            }
            auto leftHand  = defender->GetEquippedObject(true);
            if (!leftHand) {
                logger::debug("{} has nothing in left hand", defender->GetName());
            }

            if (defender->GetEquippedObject(true) && defender->GetEquippedObject(true)->IsWeapon()) {
                logger::debug("{}'s left hand is a weapon", defender->GetName());
                if (leftHand->As<RE::TESObjectWEAP>()->IsBow()) {
                    logger::debug("{}'s left hand weapon is a bow", defender->GetName());
                    logger::debug("aggressor is {}", aggressor->GetName());
                    logger::debug("{} has {} equipped", defender->GetName(), defender_weap->GetFormID());
                    if (leftHand->As<RE::TESObjectWEAP>()->HasKeywordString("REQ_BowBreakable") && defender->AsActorState()->GetWeaponState() == RE::WEAPON_STATE::kDrawn) {
                        logger::debug("weapon will get damaged");
                        util->ProcessWeaponLoss(defender, leftHand->As<RE::TESObjectWEAP>());
                    } 
                }
            }            
        }
        return RE::BSEventNotifyControl::kContinue;
    }
} // namespace Events
