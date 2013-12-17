#include "include/state.h"
#include "canvas/settings.hpp"

State* State::__instance = nullptr;

State::State() : logger("State") {
    cnvs::settings::set_defaults();
    cnvs::settings::set("canvas_host", "http://localhost");
    cnvs::settings::set("canvas_port", "3000");
}

State::~State() {
    info() << "Cleaning up.";
}

State& State::singleton() {
    if (!__instance) {
        __instance = new State();
    }

    return *__instance;
}
