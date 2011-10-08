#pragma once

using namespace System;

namespace KeyMagicDotNet {
	public ref struct LayoutOptions
	{
		/**
		 * consider CAPSLOCK key in matching
		 */
		bool trackCaps;
		/**
		 * smart backspace
		 */
		bool autoBksp;
		/**
		 * eat the key if there is no match
		 */
		bool eat;
		/**
		 * layout is based on the US keyboard Layout
		 */
		bool posBased;
		/**
		 * treat CTRL+ALT as RALT
		 */
		bool rightAlt;
	};
}