#include "ParticleSystem.h"
#include <cstdlib>
#include <ctime>
#include <glm/gtc/type_ptr.hpp>

ParticleSystem::ParticleSystem(){
    srand(time(0));
    emitPosition = glm::vec3(0,0,0);
    emitPositionVariance = glm::vec3(0,0,0);
    
    initialVelocity = glm::vec3(0,0,0);
    initialVelocityVariance = glm::vec3(0,0,0);
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
    float randomLife = baseLife + lifeVariance * ((float)rand() / RAND_MAX* 2.0f - 1.0f);
    Particle p(randomLife, particleRadius);
    //Position variance
    
    float s  = (float)rand() / RAND_MAX;
    float t = (float)rand() / RAND_MAX;

    float u = 2* glm::pi<float>() * s; //convert s into a random Y axis angle 0 to 2pi
    float v = sqrt(t / (1 - t)); //stretch factor from t that controls dx and dz
    
    float dx = 2 * v * cos(u);
    float dy = 1 - 2*t;
    float dz = 2 * v * sin(u);

    p.position.x = emitPosition.x + dx * emitPositionVariance.x;
    p.position.y = emitPosition.y + dy * emitPositionVariance.y;
    p.position.z = emitPosition.z + dz * emitPositionVariance.z;

    p.velocity.x = initialVelocity.x + dx * initialVelocityVariance.x;
    p.velocity.y = initialVelocity.y + dy * initialVelocityVariance.y;
    p.velocity.z = initialVelocity.z + dz * initialVelocityVariance.z;

    particles.push_back(p);
}

void ParticleSystem::ApplyGravity(Particle& p){
    glm::vec3 f = p.mass * gravity *glm::vec3(0,-1,0);
    p.ApplyForce(f);
}

//f_drag = ½ ρ |v|² c_d · a · (-v̂)
void ParticleSystem::ApplyDrag(Particle& p){
    //|v|
    float speed = glm::length(p.velocity);
    if (speed < 0.0001f) return;

    glm::vec3 dragDirection = -(p.velocity / speed);
    //a sphere right now
    float area = glm::pi<float>() * p.radius* p.radius;
    glm::vec3 f_drag = 0.5f * airDensity * speed*speed * dragCoefficient * area * dragDirection;

    p.ApplyForce(f_drag);

}

void ParticleSystem::Update(float deltaTime){

    // Determine how many new particles to create this frame
    float num=deltaTime*emitRate+spawnAccumulator;
    int newParticles=int(num); //round up and check if spawnning 
    spawnAccumulator=num-float(newParticles);

    // Create particles
    for(int i = 0; i < newParticles; i++)
        SpawnParticles();

    //update all particles
    for(Particle& p:particles){
        ApplyGravity(p);
        ApplyDrag(p);
        p.Integrate(deltaTime);
        p.CheckGroundCollision(groundHeight, elasticity, friction);
        
    }
    //swap with last removal of dead particles
    int i = 0;
    while (i< particles.size()){
        particles[i].life-=deltaTime;
        if (!particles[i].HaveLife()){
            Particle temp = particles[i];
            particles[i] = particles[particles.size()-1];
            particles[particles.size()-1] = temp;
            particles.pop_back();
        }else{
            i++;
        }
    }
}

void ParticleSystem::DrawSphere(float radius, int slices, int stacks) {
    for (int i = 0; i < stacks; i++) {
        float phi0 = glm::pi<float>() * ((float)i / stacks);
        float phi1 = glm::pi<float>() * ((float)(i + 1) / stacks);

        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; j++) {
            float theta = 2.0f * glm::pi<float>() * ((float)j / slices);

            float x0 = radius * sin(phi0) * cos(theta);
            float y0 = radius * cos(phi0);
            float z0 = radius * sin(phi0) * sin(theta);

            float x1 = radius * sin(phi1) * cos(theta);
            float y1 = radius * cos(phi1);
            float z1 = radius * sin(phi1) * sin(theta);

            glNormal3f(x0/radius, y0/radius, z0/radius);
            glVertex3f(x0, y0, z0);
            glNormal3f(x1/radius, y1/radius, z1/radius);
            glVertex3f(x1, y1, z1);
        }
        glEnd();
    }
}


void ParticleSystem::Draw(glm::mat4 viewProjection){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    for(Particle& p : particles){
        glPushMatrix();
        glLoadMatrixf(glm::value_ptr(viewProjection));
        glTranslatef(p.position.x, p.position.y, p.position.z);

        // fade from bright to dark as particle ages
        float lifeRatio = p.life / p.maxLife;
        glColor3f(0.2f + 0.8f * lifeRatio, 0.4f * lifeRatio, 0.8f);

        DrawSphere(p.radius, 12, 12);
        glPopMatrix();
    }
    glDisable(GL_LIGHTING);
    //draw ground
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    glVertex3f (-10, groundHeight, -10);
    glVertex3f ( 10, groundHeight, -10);
    glVertex3f ( 10, groundHeight,  10);
    glVertex3f (-10, groundHeight,  10);
    glEnd();

    glEnable(GL_LIGHTING);
}

void ParticleSystem::DrawGUI(){

}