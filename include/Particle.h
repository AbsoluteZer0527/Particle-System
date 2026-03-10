#pragma once
#include "core.h"

class Particle {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    glm::vec3 normal;
    float mass;
    float life;
    float radius;
    float maxLife;


    Particle(float life,float radius);

    void ApplyForce(const glm::vec3& f);
    void Integrate(float deltaTime);
    void CheckGroundCollision(float groundHeight, float elasticity, float friction);
    bool HaveLife(){return life>0;};
};