#pragma once

#ifndef TURTLELUAREPLWIDGET_H
#define TURTLELUAREPLWIDGET_H

#include "LuaReplWidget.hpp"
#include "PicoTurtle.hpp"
#include "PicoTurtleLua.hpp"

#define TURTLE_LUA_DIR_ENV_VAR "TURTLE_LUA_DIR"

namespace turtle
{


	class TurtleLuaReplWidget : public LuaReplWidget
	{

	public:
		TurtleLuaReplWidget(QWidget* parent = nullptr);
		~TurtleLuaReplWidget();

		int init_turtle_lua_binding();
		void cleanup_turtle_lua_binding();

		static std::function<void(PicoTurtle* t)> notify_turtle_created_fn;
		static std::function<void(PicoTurtle* t)> notify_turtle_update_fn;
		static std::function<void(PicoTurtle* t)> notify_turtle_paint_fn;
		static std::function<void(PicoTurtle* t, int tm)> turtle_delay_fn;

		static void set_notify_turtle_created_fn(std::function<void(turtle::PicoTurtle* t)> notifyfn);
		static void set_notify_turtle_update_fn(std::function<void(turtle::PicoTurtle* t)> notifyfn);
		static void set_notify_turtle_paint_fn(std::function<void(turtle::PicoTurtle* t)> paintfn);
		static void set_turtle_delay_fn(std::function<void(turtle::PicoTurtle* t, int tm)> delayfn);

		/**
		 * @brief Initialize lua state with libraries,
		 * and some global stuff.
		 */
		virtual int init_lua();

		/**
		 * cleanup lua state and other items related to it.
		 */
		virtual void cleanup_lua();

	private:
		static void turtle_init_cb(PicoTurtle* t, void* cb_args);
		static void turtle_update_cb(PicoTurtle* t, void* cb_args);
		static void turtle_paint_cb(PicoTurtle* t, void* cb_args);
		static void turtle_destroy_cb(PicoTurtle* t, void* cb_args);
		static void turtle_delay(PicoTurtle* t, int tm);

	};

};


#endif // TURTLELUAREPLWIDGET_H
