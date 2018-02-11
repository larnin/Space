#include "WindowEventsHolder.h"
#include "WindowEventArgs.h"
#include "Event.h"

WindowEventsHolder::WindowEventsHolder(Nz::EventHandler & e)
{
	OnGainedFocus.Connect(e.OnGainedFocus,
		[](const Nz::EventHandler*) {Event<GainedFocusEvent>::send({}); });
	OnLostFocus.Connect(e.OnLostFocus,
		[](const Nz::EventHandler*) {Event<LostFocusEvent>::send({}); });
	OnKeyPressed.Connect(e.OnKeyPressed,
		[](const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent& event) {Event<KeyPressedEvent>::send({ event }); });
	OnKeyReleased.Connect(e.OnKeyReleased,
		[](const Nz::EventHandler*, const Nz::WindowEvent::KeyEvent& event) {Event<KeyReleasedEvent>::send({ event }); });
	OnMouseButtonDoubleClicked.Connect(e.OnMouseButtonDoubleClicked,
		[](const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event) {Event<MouseButtonDoubleClickedEvent>::send({ event }); });
	OnMouseButtonPressed.Connect(e.OnMouseButtonPressed,
		[](const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event) {Event<MouseButtonPressedEvent>::send({ event }); });
	OnMouseButtonReleased.Connect(e.OnMouseButtonReleased,
		[](const Nz::EventHandler*, const Nz::WindowEvent::MouseButtonEvent& event) {Event<MouseButtonReleasedEvent>::send({ event }); });
	OnMouseEntered.Connect(e.OnMouseEntered,
		[](const Nz::EventHandler*) {Event<MouseEnteredEvent>::send({}); });
	OnMouseLeft.Connect(e.OnMouseLeft,
		[](const Nz::EventHandler*) {Event<MouseLeftEvent>::send({}); });
	OnMouseMoved.Connect(e.OnMouseMoved,
		[](const Nz::EventHandler*, const Nz::WindowEvent::MouseMoveEvent & event) {Event<MouseMovedEvent>::send({ event }); });
	OnMouseWheelMoved.Connect(e.OnMouseWheelMoved,
		[](const Nz::EventHandler*, const Nz::WindowEvent::MouseWheelEvent& event) {Event<MouseWheelMovedEvent>::send({event}); });
	OnMoved.Connect(e.OnMoved,
		[](const Nz::EventHandler*, const Nz::WindowEvent::PositionEvent & event) {Event<MovedEvent>::send({ event }); });
	OnQuit.Connect(e.OnQuit,
		[](const Nz::EventHandler*) {Event<QuitEvent>::send({}); });
	OnResized.Connect(e.OnResized,
		[](const Nz::EventHandler*, const Nz::WindowEvent::SizeEvent & event) {Event<ResizedEvent>::send({ event }); });
	OnTextEntered.Connect(e.OnTextEntered,
		[](const Nz::EventHandler*, const Nz::WindowEvent::TextEvent & event) {Event<TextEnteredEvent>::send({ event }); });
}
