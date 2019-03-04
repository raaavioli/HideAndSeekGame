#pragma once

#include "Event.h"

namespace Engine {

	class ENGINE_API KeyEvent : public Engine::Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode;  }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard);

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {};

		int m_KeyCode;
	};

	class ENGINE_API KeyPressedEvent : public Engine::KeyEvent
	{
	public: 
		KeyPressedEvent(int keycode, int repeatcount)
			: KeyEvent(keycode), m_RepeatCount(repeatcount) {};

		inline int getRepeatCount() { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount;
	};

	class ENGINE_API KeyReleasedEvent : public Engine::KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {};

		inline int getRepeatCount() { return m_RepeatCount; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)

	private:
		int m_RepeatCount;
	};
}
