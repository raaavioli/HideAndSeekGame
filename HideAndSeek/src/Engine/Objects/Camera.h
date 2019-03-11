#pragma once

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_precision.hpp"

#include "Engine/Core.h"
#include "Engine/Input.h"

namespace Engine {
	enum Direction {
		FORWARD		= 1,
		BACKWARD	= 2,
		LEFT		= 4,
		RIGHT		= 8,
	};

	class ENGINE_API Camera
	{
	public:
		Camera(const glm::vec3 position,	// Camera's position in 3D space
			const double yaw,				// Camera's angle going horizontally
			const double pitch,				// Camera's angle going vertically
			const double roll,				// Camera's angle going around facing direction
			const double fov,				// Field of view Value in degrees
			const double near_plane,		// Determines the nearest possible distance for drawing objects 
			const double far_plane,			// Determines how far the camera can see
			const double perspective_ratio)	// ScreenWidth / ScreenHeight
			:	m_Position(position), m_Yaw(yaw),
				m_Pitch(pitch), m_Roll(roll), m_Fov(fov),
				m_NearPlane(near_plane), m_FarPlane(far_plane),
				m_PerspectiveRatio(perspective_ratio) 
		{
			m_IsRotatable = false;
		};	

		~Camera();

		void Move(unsigned char directions, float speed);
		void Rotate(double pitch, double yaw, double roll, float scale_factor);
		glm::vec3 *GetPosition();
		glm::mat4 *GetProjectionMatrix();
		glm::mat4 *GetViewMatrix();
		glm::vec3 GetForwardDirection();

		inline void ToggleRotatable() { m_IsRotatable = !m_IsRotatable; }
		inline bool IsRotatable() { return m_IsRotatable; }

	private:
		glm::vec3 m_Position;		// Position in world-space
		double m_Yaw;				// Rotation around y-axis/up pointing axis
		double m_Pitch;				// Rotation around x-axis/right pointing axis
		double m_Roll;				// Rotation around z-axis/'backwards' pointing axis 
		double m_Fov;				// Field of view (in degrees)
		double m_NearPlane;			// Determines the nearest possible distance for drawing objects 
		double m_FarPlane;			// Determines how far the camera can see
		double m_PerspectiveRatio;	// ScreenWidth / ScreenHeight

		bool m_IsRotatable;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 getRightDirection();

	};

}

#endif