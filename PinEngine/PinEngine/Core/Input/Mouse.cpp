#include "Mouse.h"

namespace PinEngine
{
	void Mouse::OnLeftPressed(int x, int y)
	{
		leftIsDown = true;
		MouseEvent me(MouseEvent::EventType::LPress, x, y);
		eventBuffer.push(me);
	}

	void Mouse::OnLeftReleased(int x, int y)
	{
		leftIsDown = false;
		eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
	}

	void Mouse::OnRightPressed(int x, int y)
	{
		rightIsDown = true;
		eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
	}

	void Mouse::OnRightReleased(int x, int y)
	{
		rightIsDown = false;
		eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
	}

	void Mouse::OnMiddlePressed(int x, int y)
	{
		mbuttonDown = true;
		eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
	}

	void Mouse::OnMiddleReleased(int x, int y)
	{
		mbuttonDown = false;
		eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
	}

	void Mouse::OnWheelUp(int x, int y)
	{
		eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
	}

	void Mouse::OnWheelDown(int x, int y)
	{
		eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
	}

	void Mouse::OnMouseMove(int x, int y)
	{
		this->x = x;
		this->y = y;
		eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
	}

	void Mouse::OnMouseMoveRaw(int x, int y)
	{
		eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
	}

	bool Mouse::IsLeftDown()
	{
		return leftIsDown;
	}

	bool Mouse::IsMiddleDown()
	{
		return mbuttonDown;
	}

	bool Mouse::IsRightDown()
	{
		return rightIsDown;
	}

	int Mouse::GetPosX()
	{
		return x;
	}

	int Mouse::GetPosY()
	{
		return y;
	}

	MousePoint Mouse::GetPos()
	{
		return{ x, y };
	}

	bool Mouse::EventBufferIsEmpty()
	{
		return eventBuffer.empty();
	}

	MouseEvent Mouse::ReadEvent()
	{
		if (eventBuffer.empty())
		{
			return MouseEvent();
		}
		else
		{
			MouseEvent e = eventBuffer.front(); //Get first event from buffer
			eventBuffer.pop(); //Remove first event from buffer
			return e;
		}
	}
}