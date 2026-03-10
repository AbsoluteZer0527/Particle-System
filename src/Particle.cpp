#include "Particle.h"

Particle::Particle(float maxLife,float radius){
    position = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
    force = glm::vec3(0.0f);
    mass = 1.0f;
    life = maxLife;
    maxLife = maxLife;
    radius = radius;

}

//add all the forces, called by other classes
void Particle::ApplyForce(const glm::vec3& f){
    force += f;
}

//Core physics
void Particle::Integrate(float deltaTime){
    //Newton 2nd law
    glm::vec3 acceleration = force / mass;

    //Euler
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    force = glm::vec3(0.0f);
}

void Particle::CheckGroundCollision(float groundHeight, float elasticity, float friction){
    //push position on ground
    if (position.y - radius < groundHeight){
        position.y = groundHeight+radius;
    }
    //velocity: break it to downward + other
    glm::dot(velocity,glm::vec3(0,1,0)); //groundNormal= vec3(0,1,0)
}

