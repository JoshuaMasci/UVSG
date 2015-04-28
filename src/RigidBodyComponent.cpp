#include "RigidBodyComponent.hpp"

RigidBodyComponent::RigidBodyComponent(GameObject* object, btTransform& offsetTransform, btCollisionShape* shape, float mass)
: Component(object)
{
    offset = offsetTransform;

    btVector3 boxInertia;
    shape->calculateLocalInertia(mass, boxInertia);
    btDefaultMotionState* MotionState = new btDefaultMotionState(offsetTransform);
    btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, MotionState, shape, boxInertia);
    rigidBody = new btRigidBody(boxRigidBodyCI);
    rigidBody->setUserPointer(parent);

    parent->worldPtr->worldPhysics->addRigidBody(rigidBody);
}

void RigidBodyComponent::update()
{
    btTransform temp;
    rigidBody->getMotionState()->getWorldTransform(temp);
    parent->transform = temp;
}

RigidBodyComponent::~RigidBodyComponent()
{
    parent->worldPtr->worldPhysics->removeRigidBody(rigidBody);
    delete rigidBody->getMotionState();
    delete rigidBody->getCollisionShape();
    delete rigidBody;
}