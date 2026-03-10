#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
    emitPosition = glm::vec3(0,0,0);
    initialVelocity = glm::vec3(0,0,0);
    emitRate = 10.0f;
    baseLife = 3.0f;
    lifeVariance = 1.0f;
    spawnAccumulator = 0.0f; //leftover from last frame
    particleRadius = 0.1f;

    //physics, can modify in Imgui
    gravity = 9.8f;
    airDensity = 1.225f;
    dragCoefficient = 0.47f;
    elasticity = 0.5f;
    friction = 0.3f;
    groundHeight = 0.0f;

}

// When a particle is created, one must set its initial position, velocity, and other attributes
//It is often nice to be able to specify some type of geometry of the 
//Particle source, along with a particle creation rate
//This source geometry could also animate over time
void ParticleSystem::SpawnParticles(){
    float randomLife = baseLife + lifeVariance * randomFloat(-1, 1);
    Particle p(baseLife, particleRadius);
    //xyz
    
    Particles.push_back(p);
}

void ParticleSystem::Update(float deltaTime){
    // Determine how many new particles to create this frame
    float num=deltaTime*emitRate+SpawnAccumulator;
    int newParticles=int(num); //round up and check if spawnning 
    SpawnAccumulator=num-float(newParticles);

    // Create particles
    SpawnParticles();
}

void ParticleSystem::ApplyGravity(Particle& p){
    float f = p.mass * gravity *p->position.y;
}

//f_drag = ½ ρ |v|² c_d · a · (-v̂)
void ParticleSystem::AppyDrag(Particle& p){
    float normalVelocity = -initialVelocity;
    if (normalVelocity < 0.1){
        return
    }
    float f_drag = 0.5 * airDensity * initialVelocity* initialVelocity * dragCoefficient * normalVelocity;

}

ParticleSystem:: CheckGround