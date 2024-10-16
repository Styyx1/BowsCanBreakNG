#pragma once
#include "Utility.h"
#include "Settings.h"

namespace Events
{
    class OnHitHandler : public EventSingleton<OnHitHandler, RE::TESHitEvent>
    {
    public:
        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) noexcept override;

        static void Register()
        {
            RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
            eventHolder->AddEventSink(OnHitHandler::GetSingleton());
        }

    };

} // namespace Events
