#include "Components/ShipFlightControl.hpp"
#include "World/Entity.hpp"
#include <stdio.h>
#include <stdlib.h> 
#include "Util.hpp"

#include "Ship/Directions.hpp"

ShipFlightControl::ShipFlightControl(SDL_GameController* controllerToUse)
{
	m_controller = controllerToUse;
	turnSpeeds = vector3D(0.5, 0.5, 0.75);
}

ShipFlightControl::~ShipFlightControl()
{

}

void ShipFlightControl::update(double deltaTime)
{

	//Max Speed in meters per second
	double maxSpeed[] = { 15.0, 15.0, 125.0, 15.0, 15.0, 15.0 };

	//Acceleration in meters per second per second
	double accelerations[] = { 15.0, 15.0, 25.0, 15.0, 15.0, 15.0 };

	bool FlightAssistEnabled = true;

	Entity* parent = getParent();
	if (parent == nullptr)
	{
		printf("No parent!!!\n");
		exit(1);
	}

	if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_A))
	{
		FlightAssistEnabled = false;
	}

	//Forward Backward Movment
	if (true)
	{
		if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
		{
			m_throttle = 1.0;
		}
		else if (SDL_GameControllerGetButton(m_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
		{
			m_throttle = -1.0;
		}
		else
		{
			m_throttle = 0.0;
		}

		double currentForwardVelocity = glm::dot(parent->m_transform.getForward(), parent->m_Velocity.linearVelocity);
		double desiredForwardVelocity = m_throttle * (m_throttle > 0 ? maxSpeed[FORWARD] : maxSpeed[BACKWARD]);
		double diffrence = abs(currentForwardVelocity - desiredForwardVelocity);

		if (currentForwardVelocity < desiredForwardVelocity)
		{
			if (FlightAssistEnabled || m_throttle > 0.0)
			{
				double velocitytoAdd = accelerations[FORWARD] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				parent->m_Velocity.linearVelocity += parent->m_transform.getForward() * velocitytoAdd;
			}
		}
		else if (currentForwardVelocity > desiredForwardVelocity)
		{

			if (FlightAssistEnabled || m_throttle < 0.0)
			{
				double velocitytoAdd = accelerations[BACKWARD] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				parent->m_Velocity.linearVelocity += -parent->m_transform.getForward() * velocitytoAdd;
			}
		}
	}

	//Up Down Movement
	if (true)
	{
		double currentUpVelocity = glm::dot(parent->m_transform.getUp(), parent->m_Velocity.linearVelocity);
		double desiredUpVelocity = m_upDownThrottle * (m_upDownThrottle > 0 ? maxSpeed[UP] : maxSpeed[DOWN]);
		double diffrence = abs(currentUpVelocity - desiredUpVelocity);

		if (currentUpVelocity < desiredUpVelocity)
		{
			if (FlightAssistEnabled || m_upDownThrottle > 0.0)
			{
				double velocitytoAdd = accelerations[UP] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				parent->m_Velocity.linearVelocity += parent->m_transform.getUp() * velocitytoAdd;
			}
		}
		else if (currentUpVelocity > desiredUpVelocity)
		{

			if (FlightAssistEnabled || m_upDownThrottle < 0.0)
			{
				double velocitytoAdd = accelerations[DOWN] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				parent->m_Velocity.linearVelocity += -parent->m_transform.getUp() * velocitytoAdd;
			}
		}
	}

	//Left Right Movement
	if (true)
	{
		double currentLeftVelocity = glm::dot(parent->m_transform.getLeft(), parent->m_Velocity.linearVelocity);
		double desiredLeftVelocity = m_leftRightThrottle * (m_leftRightThrottle > 0 ? maxSpeed[LEFT] : maxSpeed[RIGHT]);
		double diffrence = abs(currentLeftVelocity - desiredLeftVelocity);

		if (currentLeftVelocity < desiredLeftVelocity)
		{
			if (FlightAssistEnabled || m_leftRightThrottle > 0.0)
			{
				double velocitytoAdd = accelerations[LEFT] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				parent->m_Velocity.linearVelocity += parent->m_transform.getLeft() * velocitytoAdd;
			}
		}
		else if (currentLeftVelocity > desiredLeftVelocity)
		{

			if (FlightAssistEnabled || m_leftRightThrottle < 0.0)
			{
				double velocitytoAdd = accelerations[RIGHT] * deltaTime;
				if (velocitytoAdd >= diffrence)
				{
					velocitytoAdd = diffrence;
				}

				parent->m_Velocity.linearVelocity += -parent->m_transform.getLeft() * velocitytoAdd;
			}
		}

		int deadzone = 8000;

		int pitchAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTY);
		if (pitchAxis > deadzone || pitchAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)pitchAxis) / 32767.0f;
			double angle = amount * deltaTime * (turnSpeeds.x * 2.0 * M_PI);

			//Negitive angle because the joystick layout is backwards
			quaternionD pitchQuat = glm::normalize(glm::angleAxis(angle, parent->m_transform.getRight()));

			parent->m_transform.m_orientation = pitchQuat * parent->m_transform.m_orientation;
		}

		int yawAxis = -SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_RIGHTX);
		if (yawAxis > deadzone || yawAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)yawAxis) / 32767.0f;
			double angle = amount * deltaTime * (turnSpeeds.y * 2.0 * M_PI);

			//Negitive angle because the joystick layout is backwards
			quaternionD yawQuat = glm::normalize(glm::angleAxis(angle, parent->m_transform.getUp()));

			parent->m_transform.m_orientation = yawQuat * parent->m_transform.m_orientation;
		}

		int rollAxis = SDL_GameControllerGetAxis(m_controller, SDL_CONTROLLER_AXIS_LEFTX);
		if (rollAxis > deadzone || rollAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)rollAxis) / 32767.0f;
			double angle = amount * deltaTime * (turnSpeeds.z * 2.0 * M_PI);

			//Negitive angle because the joystick layout is backwards
			quaternionD rollQuat = glm::normalize(glm::angleAxis(angle, parent->m_transform.getForward()));

			parent->m_transform.m_orientation = rollQuat * parent->m_transform.m_orientation;
		}

	}
}