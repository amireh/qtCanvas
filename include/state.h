#ifndef STATE_H
#define STATE_H

#include "canvas/session.hpp"

class State : public cnvs::logger
{
public:
    virtual ~State();
    static State& singleton();

    cnvs::session session;
private:
    State();

    static State* __instance;
};

#endif // STATE_H
