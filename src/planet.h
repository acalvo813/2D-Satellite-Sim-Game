#ifndef __PLANET_AGENT__H
#define __PLANET_AGENT__H 

#include "enviro.h"

using namespace enviro;

class PlanetController : public Process, public AgentInterface {
// a planet object which is completely static. Provides a place for the satellites to land on rotate around crash etc.... 

    public:
    PlanetController() : Process(), AgentInterface() {}

    void init() {}

    void start() {}

    void update() {
        
    }

    void stop() {}


};

class Planet : public Agent {
    public:
    Planet(json spec, World& world) : Agent(spec, world) {
        add_process(p);
    }
    private:
    PlanetController p;
};

DECLARE_INTERFACE(Planet)

#endif