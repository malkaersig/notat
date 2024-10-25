#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#ifndef _UNICODE
#define _UNICODE
#endif 

#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>

#include <Windows.h>
#include <ShObjIdl.h>
#include <atlbase.h> // CComPtr
#include <comdef.h> // _com_error