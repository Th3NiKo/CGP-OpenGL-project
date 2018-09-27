#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

class RigidBody
{

    public:
        RigidBody (float m, glm::mat3 inertia);
        virtual ~RigidBody();

        void Update ( float t, float dt);

        void ClearEverything();

        void AddForce(glm::vec3 force);
        void AddForce(glm::vec3 force, glm::vec3 position);
        void AddTorque(glm::vec3 _torque);

        void Convert ( glm::quat Q, glm::vec3 P, glm::vec3 L, glm::mat3 & R, glm::vec3 & V, glm::vec3 & W) const ;
        // constant quantities
        float m_mass , m_invMass ;
        glm::mat3 m_inertia , m_invInertia ;
        // state variables
        glm::vec3 m_X; // position
        glm::quat m_Q; // orientation
        glm::vec3 m_P; // linear momentum
        glm::vec3 m_L; // angular momentum
        // derived state variables

        glm::mat3 m_R; // orientation matrix
        glm::vec3 m_V; // linear velocity vector
        glm::vec3 m_W; // angular velocity

        // force and torque functions
        glm::vec3 m_force;
        glm::vec3 m_torque;

    private:
};

#endif // RIGIDBODY_H
