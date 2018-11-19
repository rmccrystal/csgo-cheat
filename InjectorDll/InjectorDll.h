#pragma once

#include "stdafx.h"

bool file_exists(const wchar_t* name);
bool process_exists(const wchar_t* name, uint32_t& pid);
bool enable_debug_privilege(HANDLE process);
bool process_open(uint32_t pid, HANDLE& handle);
bool inject(HANDLE process, const wchar_t* dll);
int injectToCsgo();