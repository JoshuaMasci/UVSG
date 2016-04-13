#ifndef COMPONETS_HPP
#define COMPONETS_HPP

#include "Rendering/Mesh.hpp"
#include "Rendering/TexturedMesh.hpp"
#include <vector>

class Transform
{
public:
	Transform(){};
	Transform(vector3D position){ m_position = position; };
	Transform(vector3D position, quaternionD orientation){ m_position = position; m_orientation = orientation; };

	vector3D m_position;
	quaternionD m_orientation;
	vector3D m_scale = vector3D(1.0f); //Start m_scale with a default of 1.0f

	//Getters for the directional vectors.
	vector3D getPos() const { return m_position; };
	vector3D getForward() const { return m_orientation * vector3D(0.0f, 0.0f, 1.0f); };
	vector3D getUp() const { return m_orientation * vector3D(0.0f, 1.0f, 0.0f); };
	vector3D getRight() const { return m_orientation * vector3D(-1.0f, 0.0f, 0.0f); };
	vector3D getLeft() const { return m_orientation * vector3D(1.0f, 0.0f, 0.0f); };
	quaternionD getOrientation() const { return m_orientation; };
	vector3D getScale() const { return m_scale; };

	void setPos(const vector3D& vec){ m_position = vec; };
	void setOrientation(const quaternionD& quat){ m_orientation = quat; }
	void setScale(const vector3D& vec){ m_scale = vec; }

	void setTransform(const Transform& transform)
	{
		this->setPos(transform.getPos());
		this->setOrientation(transform.getOrientation());
		this->setScale(transform.getScale());
	};

	void setPositionAndRotationFromTransform(const Transform& transform)
	{
		this->setPos(transform.getPos());
		this->setOrientation(transform.getOrientation());
	};

	matrix4 getModleMatrix() const
	{
		matrix4 positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();

		positionMatrix = glm::translate(matrix4(1.0F), (vector3F)m_position);
		rotationMatrix = glm::toMat4((quaternionF) m_orientation);
		scaleMatrix = glm::scale(matrix4(1.0F), (vector3F)m_scale);

		return positionMatrix * rotationMatrix * scaleMatrix;
	};

	matrix4 getModleMatrix(vector3D cameraPos) const
	{
		matrix4 positionMatrix = matrix4();
		matrix4 rotationMatrix = matrix4();
		matrix4 scaleMatrix = matrix4();

		positionMatrix = glm::translate(matrix4(1.0F), (vector3F)(m_position - cameraPos));
		rotationMatrix = glm::toMat4((quaternionF)m_orientation);
		scaleMatrix = glm::scale(matrix4(1.0F), (vector3F)m_scale);

		return positionMatrix * rotationMatrix * scaleMatrix;
	};

	matrix3 getNormalMatrix() const
	{
		return glm::toMat3((quaternionF) m_orientation);
	};
};

class Velocity
{
public:
	vector3D linearVelocity;
	vector3D angularVelocity;
};

#endif //COMPONETS_HPP
