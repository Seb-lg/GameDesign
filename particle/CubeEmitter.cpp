#include <GL/gl.h>
#include "ParticleSystemPCH.hpp"
#include "Random.hpp"
#include "CubeEmitter.hpp"
CubeEmitter::CubeEmitter()
: MinWidth(-1)
, MaxWidth(1)
, MinHeight(-1)
, MaxHeight(1)
, MinDepth(-1)
, MaxDepth(1)
, MinSpeed(10)
, MaxSpeed(20)
, MinLifetime(3)
, MaxLifetime(5)
, Origin(0)
{}

void CubeEmitter::EmitParticle( Particle& particle )
{
    float X = RandRange( MinWidth, MaxWidth );
    float Y = RandRange( MinHeight, MaxHeight );
    float Z = RandRange( MinDepth, MaxDepth );

    float lifetime = RandRange( MinLifetime, MaxLifetime );
    float speed = RandRange( MinSpeed, MaxSpeed );

    glm::vec3 vector( X, Y, Z );

    particle.m_Position = vector + Origin;
    particle.m_Velocity = glm::normalize(vector) * speed;

    particle.m_fLifeTime = lifetime;
    particle.m_fAge = 0;
}

void CubeEmitter::DebugRender()
{
	std::cout << "ca marche pas" << std::endl;
}
