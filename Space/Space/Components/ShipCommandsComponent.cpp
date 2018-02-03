#include <Nazara\Platform\Keyboard.hpp>
#include "ShipCommandsComponent.h"
#include "Env.h"

Ndk::ComponentIndex ShipCommandsComponent::componentIndex;

ShipCommandsComponent::ShipCommandsComponent()
{

}

Nz::Vector2f ShipCommandsComponent::evaluateControles()
{
	if (!Env::instance().window().HasFocus())
		return Nz::Vector2f(0, 0);

	Nz::Vector2f dir(0, 0);

	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Left))
		dir.x--;
	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Right))
		dir.x++;
	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Up))
		dir.y--;
	if (Nz::Keyboard::IsKeyPressed(Nz::Keyboard::Down))
		dir.y++;
	return dir;
}
