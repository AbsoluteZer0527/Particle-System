#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
    std::vector<Particle> Particles;

    glm::vec3 emitPosition;
    glm::vec3 initialVelocity;
    float emitRate;
    float baseLife;
    float lifeVariance;
    float spawnAccumulator; //leftover from last frame

    //physics, can modify in Imgui
    float gravity;
    float airDensity;
    float dragCoefficient;
    float particleRadius;
    float elasticity;
    float friction;
    float groundHeight;

    // When a particle is created, one must set its initial position, velocity, and other attributes
    //It is often nice to be able to specify some type of geometry of the 
    //Particle source, along with a particle creation rate
    //This source geometry could also animate over time
    ParticleSystem::SpawnParticles(){
            particle = new Particle();
            //xyz
            particle.
            
        particle.life = baseLife + LifeVariance * randomFloat(-1, 1);
    }

    ParticleSystem::Update(float deltaTime){
        // Determine how many new particles to create this frame
        float num=deltaTime*emitRate+SpawnAccumulator;
        int newParticles=int(num); //round up and check if spawnning 
        SpawnAccumulator=num-float(newParticles);

        // Create particles
        SpawnParticles();
    }

    ParticleSystem::ApplyGravity(Particle& p){
        float f = p.mass * gravity *p->position.y;
    }

    //f_drag = ½ ρ |v|² c_d · a · (-v̂)
    ParticleSystem::AppyDrag(Particle& p){
        float normalVelocity = -initialVelocity;
        if (normalVelocity < 0.1){
            return
        }
            float f_drag = 0.5 * airDensity * initialVelocity* initialVelocity * dragCoefficient * normalVelocity;

    }

    ParticleSystem:: CheckGround
}