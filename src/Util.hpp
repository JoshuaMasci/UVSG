#ifndef UTIL_HPP
#define UTIL_HPP

#include "glmInclude.hpp"
#include "physxInclude.hpp"

using namespace std;

//Just a basic util file for some common functions

void printQuat(const quaternion quat);
void printVec(const vector3 quat);
quaternion fromAxes(const vector3 forward, const vector3 up);

PxVec3 toPxVec3(vector3 vec);
vector3 toGlmVec3(PxVec3 vec);
PxExtendedVec3 toPxExtendedVec3(vector3 vec);

PxQuat toPxQuat(quaternion quat);
quaternion toGlmQuat(PxQuat quat);



#endif //UTIL_HPP