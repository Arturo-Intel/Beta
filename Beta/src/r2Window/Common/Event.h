#pragma once
namespace r2 {
    class Window;
    
    enum class EventType : size_t
    {
        None = 0,
        Close
    };

	class Event
	{
    public:
        // The event's type
        EventType type;

        // Pointer to a CrossWindow window
        Window* window;

        Event(EventType type = EventType::None, Window* window = nullptr);

        ~Event();

        bool operator==(const Event& other) const
        {
            return type == other.type && window == other.window;
        }
	};
}

