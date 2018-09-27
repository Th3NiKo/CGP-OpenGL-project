#include "RigidBody.h"

RigidBody::RigidBody (float m, glm::mat3 inertia)
{
    m_mass = m;
    m_inertia = inertia;
    m_invInertia =  glm::inverse(inertia);
    m_invMass = 1.0f / m;
}

RigidBody::~RigidBody()
{
    //dtor
}

void RigidBody :: Convert ( glm::quat Q, glm::vec3 P, glm::vec3 L, glm::mat3 & R, glm::vec3 & V, glm::vec3 & W) const {
    R = glm::mat3_cast (Q);
    V = P * m_invMass;
    W = R* m_invInertia * glm::transpose (R)*L;
}

void RigidBody :: Update ( float t, float dt) {
    float halfdt = 0.5f * dt , sixthdt = dt / 6.0f;
    float tphalfdt = t + halfdt , tpdt = t +dt;
    glm::vec3 XN , PN , LN , VN , WN;
    glm::quat QN;
    glm::mat3 RN;
    // A1 = G(t,S0), B1 = S0 + (dt / 2) * A1
    glm::vec3 A1DXDT = m_V;
    glm::quat A1DQDT = 0.5f * m_W * m_Q;
    glm::vec3 A1DPDT = m_force;
    glm::vec3 A1DLDT = m_torque;
    XN = m_X + halfdt * A1DXDT;
    QN = m_Q + halfdt * A1DQDT;
    PN = m_P + halfdt * A1DPDT;
    LN = m_L + halfdt * A1DLDT;
    Convert (QN ,PN ,LN ,RN ,VN ,WN);
    // A2 = G(t + dt / 2,B1), B2 = S0 + (dt / 2) * A2
    glm::vec3 A2DXDT = VN;
    glm::quat A2DQDT = 0.5f * WN * QN;
    glm::vec3 A2DPDT = m_force;
    glm::vec3 A2DLDT = m_torque;
    XN = m_X + halfdt * A2DXDT;
    QN = m_Q + halfdt * A2DQDT;
    PN = m_P + halfdt * A2DPDT;
    LN = m_L + halfdt * A2DLDT;
    Convert (QN ,PN ,LN ,RN ,VN ,WN);
    // A3 = G(t + dt / 2,B2), B3 = S0 + dt * A3
    glm::vec3 A3DXDT = VN;
    glm::quat A3DQDT = 0.5f * WN * QN;
    glm::vec3 A3DPDT = m_force;
    glm::vec3 A3DLDT = m_torque;
    XN = m_X + dt * A3DXDT;
    QN = m_Q + dt * A3DQDT;
    PN = m_P + dt * A3DPDT;
    LN = m_L + dt * A3DLDT;
    Convert (QN ,PN ,LN ,RN ,VN ,WN);
    // A4 = G(t + dt, B3),
    // S1 = S0 + (dt / 6) * (A1 + 2 * A2 + 2 * A3 + A4)
    glm::vec3 A4DXDT = VN;
    glm::quat A4DQDT = 0.5f * WN * QN;
    glm::vec3 A4DPDT = m_force;
    glm::vec3 A4DLDT = m_torque;
    m_X = m_X + sixthdt *( A1DXDT + 2.0f*( A2DXDT + A3DXDT ) + A4DXDT );
    m_Q = m_Q + sixthdt *( A1DQDT + 2.0f*( A2DQDT + A3DQDT ) + A4DQDT );
    m_P = m_P + sixthdt *( A1DPDT + 2.0f*( A2DPDT + A3DPDT ) + A4DPDT );
    m_L = m_L + sixthdt *( A1DLDT + 2.0f*( A2DLDT + A3DLDT ) + A4DLDT );
    Convert (m_Q ,m_P ,m_L ,m_R ,m_V ,m_W );

    m_force = glm::vec3(0.0f, 0.0f, 0.0f);
    m_torque = glm::vec3(0.0f, 0.0f, 0.0f);

}

void RigidBody::ClearEverything(){
    glm::quat tempQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::mat3 jednostkowa = glm::mat3(1.0f);
    glm::vec3 nullVector = glm::vec3(0.0f);
    m_X = nullVector; // position
    m_Q = tempQuat; // orientation
    m_P = nullVector; // linear momentum
    m_L = nullVector; // angular momentum
    // derived state variables

    m_R = jednostkowa; // orientation matrix
    m_V = nullVector; // linear velocity vector
    m_W = nullVector; // angular velocity

    m_force = glm::vec3(0.0f, 0.0f, 0.0f);
    m_torque = glm::vec3(0.0f, 0.0f, 0.0f);

}

void RigidBody::AddForce(glm::vec3 force){
    AddForce(force, m_X);
}

void RigidBody::AddForce(glm::vec3 force, glm::vec3 position){
    m_force += force;

    glm::vec3 taui = glm::cross(position - m_X, force);
    m_torque += taui;
}


void RigidBody::AddTorque(glm::vec3 _torque){
    m_torque += _torque;
}
