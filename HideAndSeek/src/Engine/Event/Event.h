#pragma once

#include "Engine/Core.h"

#include <string>
#include <sstream>
#include <functional>

namespace Engine {

	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResized, WindowFocused, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, 
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4),
	};


// Declares the derived functions GetEventType and GetName using the 'type' specified
// Also implements GetStaticType to return the static type of a specific Event.
#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

// Every event deriving from Event needs to assign a couple of Category Flags.
// This #define makes this easier. Just send for example "EventCategoryInput || EventCategoryKeyboard"
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class ENGINE_API Event 
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType()	const = 0;
		virtual const char* GetName()		const = 0;
		virtual int GetCategoryFlags()		const = 0;
		virtual std::string ToString()		const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

	protected:
		bool m_Handled = false;
	};

	/**
	 *	EventDispatcher takes an event and then provides with
	 * the function Dispatch to dispatch and call the specified
	 * eventfunction.
	 */
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {};

		
		// Takes: a function returning a bool with one argument,
		// The argument is a derived Event. (No type safety)
		// If the event function's parameter-event-type is
		// the same as that of m_Event, then call the Event-function
		// ---------------------------------
		// Return true if the Event was handled or not.
		// ---------------------------------
		template<typename T>
		inline bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

}

