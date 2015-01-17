/*
 * Copyright (c) 2013, Hernan Saez
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CRIMILD_GL_TASKS_UPDATE_INPUT_STATE_
#define CRIMILD_GL_TASKS_UPDATE_INPUT_STATE_

#include <Crimild.hpp>

#include <GLFW/glfw3.h>

#define CRIMILD_INPUT_KEY_SPACE GLFW_KEY_SPACE
#define CRIMILD_INPUT_KEY_ESCAPE GLFW_KEY_ESC
#define CRIMILD_INPUT_KEY_F1 GLFW_KEY_F1
#define CRIMILD_INPUT_KEY_F2 GLFW_KEY_F2
#define CRIMILD_INPUT_KEY_F3 GLFW_KEY_F3
#define CRIMILD_INPUT_KEY_F4 GLFW_KEY_F4
#define CRIMILD_INPUT_KEY_F5 GLFW_KEY_F5
#define CRIMILD_INPUT_KEY_F6 GLFW_KEY_F6
#define CRIMILD_INPUT_KEY_F7 GLFW_KEY_F7
#define CRIMILD_INPUT_KEY_F8 GLFW_KEY_F8
#define CRIMILD_INPUT_KEY_F9 GLFW_KEY_F9
#define CRIMILD_INPUT_KEY_F10 GLFW_KEY_F10
#define CRIMILD_INPUT_KEY_F11 GLFW_KEY_F11
#define CRIMILD_INPUT_KEY_F12 GLFW_KEY_F12
#define CRIMILD_INPUT_KEY_F13 GLFW_KEY_F13
#define CRIMILD_INPUT_KEY_F14 GLFW_KEY_F14
#define CRIMILD_INPUT_KEY_F15 GLFW_KEY_F15
#define CRIMILD_INPUT_KEY_F16 GLFW_KEY_F16
#define CRIMILD_INPUT_KEY_F17 GLFW_KEY_F17
#define CRIMILD_INPUT_KEY_F18 GLFW_KEY_F18
#define CRIMILD_INPUT_KEY_F19 GLFW_KEY_F19
#define CRIMILD_INPUT_KEY_F20 GLFW_KEY_F20
#define CRIMILD_INPUT_KEY_F21 GLFW_KEY_F21
#define CRIMILD_INPUT_KEY_F22 GLFW_KEY_F22
#define CRIMILD_INPUT_KEY_F23 GLFW_KEY_F23
#define CRIMILD_INPUT_KEY_F24 GLFW_KEY_F24
#define CRIMILD_INPUT_KEY_F25 GLFW_KEY_F25
#define CRIMILD_INPUT_KEY_UP GLFW_KEY_UP
#define CRIMILD_INPUT_KEY_DOWN GLFW_KEY_DOWN
#define CRIMILD_INPUT_KEY_LEFT GLFW_KEY_LEFT
#define CRIMILD_INPUT_KEY_RIGHT GLFW_KEY_RIGHT
#define CRIMILD_INPUT_KEY_LSHIFT GLFW_KEY_LEFT_SHIFT
#define CRIMILD_INPUT_KEY_RSHIFT GLFW_KEY_RIGHT_SHIFT
#define CRIMILD_INPUT_KEY_LCTRL GLFW_KEY_LCTRL
#define CRIMILD_INPUT_KEY_RCTRL GLFW_KEY_RCTRL
#define CRIMILD_INPUT_KEY_LALT GLFW_KEY_LALT
#define CRIMILD_INPUT_KEY_RALT GLFW_KEY_RALT
#define CRIMILD_INPUT_KEY_TAB GLFW_KEY_TAB
#define CRIMILD_INPUT_KEY_ENTER GLFW_KEY_ENTER
#define CRIMILD_INPUT_KEY_BACKSPACE GLFW_KEY_BACKSPACE
#define CRIMILD_INPUT_KEY_INSERT GLFW_KEY_INSERT
#define CRIMILD_INPUT_KEY_DEL GLFW_KEY_DEL
#define CRIMILD_INPUT_KEY_PAGEUP GLFW_KEY_PAGEUP
#define CRIMILD_INPUT_KEY_PAGEDOWN GLFW_KEY_PAGEDOWN
#define CRIMILD_INPUT_KEY_HOME GLFW_KEY_HOME
#define CRIMILD_INPUT_KEY_END GLFW_KEY_END
#define CRIMILD_INPUT_KEY_KP_0 GLFW_KEY_KP_0
#define CRIMILD_INPUT_KEY_KP_1 GLFW_KEY_KP_1
#define CRIMILD_INPUT_KEY_KP_2 GLFW_KEY_KP_2
#define CRIMILD_INPUT_KEY_KP_3 GLFW_KEY_KP_3
#define CRIMILD_INPUT_KEY_KP_4 GLFW_KEY_KP_4
#define CRIMILD_INPUT_KEY_KP_5 GLFW_KEY_KP_5
#define CRIMILD_INPUT_KEY_KP_6 GLFW_KEY_KP_6
#define CRIMILD_INPUT_KEY_KP_7 GLFW_KEY_KP_7
#define CRIMILD_INPUT_KEY_KP_8 GLFW_KEY_KP_8
#define CRIMILD_INPUT_KEY_KP_9 GLFW_KEY_KP_9
#define CRIMILD_INPUT_KEY_KP_DIVIDE GLFW_KEY_KP_DIVIDE
#define CRIMILD_INPUT_KEY_KP_MULTIPLY GLFW_KEY_KP_MULTIPLY
#define CRIMILD_INPUT_KEY_KP_SUBTRACT GLFW_KEY_KP_SUBTRACT
#define CRIMILD_INPUT_KEY_KP_ADD GLFW_KEY_KP_ADD
#define CRIMILD_INPUT_KEY_KP_DECIMAL GLFW_KEY_KP_DECIMAL
#define CRIMILD_INPUT_KEY_KP_EQUAL GLFW_KEY_KP_EQUAL
#define CRIMILD_INPUT_KEY_KP_ENTER GLFW_KEY_KP_ENTER
#define CRIMILD_INPUT_KEY_KP_NUM_LOCK GLFW_KEY_KP_NUM_LOCK
#define CRIMILD_INPUT_KEY_CAPS_LOCK GLFW_KEY_CAPS_LOCK
#define CRIMILD_INPUT_KEY_SCROLL_LOCK GLFW_KEY_SCROLL_LOCK
#define CRIMILD_INPUT_KEY_PAUSE GLFW_KEY_PAUSE
#define CRIMILD_INPUT_KEY_LSUPER GLFW_KEY_LSUPER
#define CRIMILD_INPUT_KEY_RSUPER GLFW_KEY_RSUPER
#define CRIMILD_INPUT_KEY_MENU GLFW_KEY_MENU

#define CRIMILD_INPUT_MOUSE_BUTTON_1 GLFW_MOUSE_BUTTON_1
#define CRIMILD_INPUT_MOUSE_BUTTON_2 GLFW_MOUSE_BUTTON_2
#define CRIMILD_INPUT_MOUSE_BUTTON_3 GLFW_MOUSE_BUTTON_3
#define CRIMILD_INPUT_MOUSE_BUTTON_4 GLFW_MOUSE_BUTTON_4
#define CRIMILD_INPUT_MOUSE_BUTTON_5 GLFW_MOUSE_BUTTON_5
#define CRIMILD_INPUT_MOUSE_BUTTON_6 GLFW_MOUSE_BUTTON_6
#define CRIMILD_INPUT_MOUSE_BUTTON_7 GLFW_MOUSE_BUTTON_7
#define CRIMILD_INPUT_MOUSE_BUTTON_8 GLFW_MOUSE_BUTTON_8
#define CRIMILD_INPUT_MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_LEFT
#define CRIMILD_INPUT_MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_RIGHT
#define CRIMILD_INPUT_MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_MIDDLE

namespace crimild {

	class UpdateInputStateTask : public Task {
	public:
		UpdateInputStateTask( int priority, GLFWwindow *window );
		virtual ~UpdateInputStateTask( void );

		virtual void start( void ) override;
		virtual void update( void ) override;
		virtual void stop( void ) override;

	private:
		GLFWwindow *_window;
		int _windowWidth;
		int _windowHeight;
	};

	typedef SharedPointer< UpdateInputStateTask > UpdateInputStateTaskPtr;

}

#endif

