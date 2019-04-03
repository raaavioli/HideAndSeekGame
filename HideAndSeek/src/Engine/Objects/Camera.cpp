#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Engine/Application.h"

namespace Engine {

	Camera::~Camera()
	{
	}

	glm::vec3 *Camera::GetPosition() 
	{
		return &m_Position;
	}

	void Camera::Move(unsigned char directions, float speed) 
	{ 
		static auto inDir = [&](Direction d) { return directions & d; };

		if (inDir(FORWARD)) {
			m_Position += speed * GetForwardDirection();
		}
		if (inDir(BACKWARD)) {
			m_Position -= speed * GetForwardDirection();
		}
		if (inDir(RIGHT)) {
			m_Position += speed * getRightDirection();
		}
		if (inDir(LEFT)) {
			m_Position -= speed * getRightDirection();
		}
	}

	////////////////////////////////////////////////////////////////////
	// Rotation is only applied locally to the camera. That is to say,//
	// A rotation does rotate the camera locally, which can appear odd//
	// if the camera has a roll != 0                                  // 
	void Camera::Rotate(double pitch, double yaw, double roll, float scale_factor) 
	{
		if(m_IsRotatable)
		{
			m_Pitch	+= scale_factor * -pitch;
			m_Yaw	+= scale_factor * -yaw;
			m_Roll	+= scale_factor * -roll;

			if (m_Pitch > glm::half_pi<float>())
				m_Pitch = glm::half_pi<float>();
			else if(m_Pitch < -glm::half_pi<float>())
				m_Pitch = -glm::half_pi<float>();
		}
	}

	glm::mat4 *Camera::GetProjectionMatrix() 
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_PerspectiveRatio, m_NearPlane, m_FarPlane);
		return &m_ProjectionMatrix;
	}

	glm::mat4 *Camera::GetViewMatrix() 
	{
		glm::vec3 direction = GetForwardDirection();
		glm::vec3 right = getRightDirection();
		glm::vec3 up = glm::cross(right, direction);

		// Returns the View-matrix consisting of: The camera position, it's direction and the up-vector.
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + direction, up);
		return &m_ViewMatrix;
	}

	// Camera's forward facing direction depending on it's angles
	glm::vec3 Camera::GetForwardDirection() 
	{
		return getCameraBaseRotation() * glm::vec3(
			cos(m_Pitch) * sin(m_Yaw),
			sin(m_Pitch),
			cos(m_Pitch) * cos(m_Yaw)
		);
	}

	glm::vec3 Camera::GetUpDirection()
	{
		glm::vec3 direction = GetForwardDirection();
		glm::vec3 right = getRightDirection();
		return glm::cross(right, direction);
	}

	// Camera's local right pointing direction. i.e camera's local z-vector.
	glm::vec3 Camera::getRightDirection() 
	{
		glm::vec3 right(
			sin(m_Yaw - 3.14f / 2.0f),
			0,
			cos(m_Yaw - 3.14f / 2.0f)
		);

		//Rotate to apply m_Roll onto the right-axis
		return getCameraBaseRotation() * glm::rotate(right, (glm::mediump_float32)glm::degrees(m_Roll), GetForwardDirection());
	}

	glm::mat3 Camera::getCameraBaseRotation()
	{
		if (m_2DView)
			return glm::mat3(1.0);
		else
			return glm::mat3(
				glm::vec3(1, 0, 0),
				glm::vec3(0, cos(-3.1415 / 2), -sin(-3.1415 / 2)),
				glm::vec3(0, sin(-3.1415 / 2), cos(-3.1415 / 2)));
	}

}
