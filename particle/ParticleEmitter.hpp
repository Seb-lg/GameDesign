#pragma once

#include "Particle.hpp"

class ParticleEmitter
{
public:
    virtual ~ParticleEmitter() {}
    virtual void EmitParticle( Particle& particle ) = 0;

    virtual void DebugRender() {}
};