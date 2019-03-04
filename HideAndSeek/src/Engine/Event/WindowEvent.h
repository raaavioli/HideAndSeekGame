#pragma once

#include "Event.h"

namespace Engine {

	class ENGINE_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_Width << "x" << m_Height;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
			EVENT_CLASS_TYPE(WindowResized)

	private:
		unsigned int m_Width, m_Height;
	};

	class ENGINE_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int xPos, int yPos)
			: m_PositionX(xPos), m_PositionY(yPos) {}

		inline int GetPositionX() const { return m_PositionX; }
		inline int GetPositionY() const { return m_PositionY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMovedEvent: (" << m_PositionX << "," << m_PositionY << ")";
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowMoved)

	private:
		int m_PositionX, m_PositionY;
	};

	class ENGINE_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClosed)
	};
}