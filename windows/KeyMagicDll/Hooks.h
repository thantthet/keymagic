#ifndef HOOKS_H_
#define HOOKS_H_

#pragma once

#include <windows.h>

#include "../KeyMagic/KeyMagic.h"
#include "common.h"

LRESULT CALLBACK HookKeyProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookGetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HookWndProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif