#pragma once
#include <array>
#include "IDevice.h"
#include "../dependencies/SDL2/include/SDL_mouse.h"

namespace Z_Engine::Inputs {

	class Mouse : public IDevice {
	public:
		Mouse(const char * name = "mouse_device") 
			: IDevice(name) 
		{
		}

		virtual bool IsKeyPressed(KeyCode key) const override {
			const auto state  =  SDL_GetMouseState(NULL, NULL);
			return (state  & SDL_BUTTON(((int)key))) == 1;
		}

		virtual bool IsKeyReleased(KeyCode key) const override {
			const auto state = SDL_GetMouseState(NULL, NULL);
			return (state & SDL_BUTTON(((int)key))) == 0;
		}

		std::array<int, 2> GetMousePosition() const {
			int x, y;
			const auto state =  SDL_GetMouseState(&x, &y);
			return std::array<int, 2> {x,y};
		} 
	};
}

