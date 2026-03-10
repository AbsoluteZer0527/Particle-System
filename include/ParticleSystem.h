#pragma once
#include "core.h"
#include <vector>
#include "Particle.h"

/* 
Spawn particles in a initial random position using Guassian
assign a random life span
for each particle, mannage gravity, aerodynamic drag, 
ground collision constrant
Integrated motion
*/

class ParticleSystem
{
public:
    std::vector<Particle> Particles;

    glm::vec3 emitPosition;
    glm::vec3 initialVelocity;
    float emitRate;
    float baseLife; //+lifeVariance = assign particle maxlife
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


    
    void Update(float deltaTime);
    void SpawnParticles();
    void ApplyGravity(Particle& p); //basic gravity
    void ApplyDrag(Particle& p); //compute drag and call apply force
    void DrawGUI();
    void Draw();//draw each particles

};
