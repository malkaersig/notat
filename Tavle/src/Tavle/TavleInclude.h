#pragma once

#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")


#ifndef UNICODE
#define UNICODE
#endif 

#ifndef _UNICODE
#define _UNICODE
#endif 

#include <atlbase.h> // CComPtr

#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <exception>

#include <Windowsx.h>
#include <Windows.h>
#include <ShObjIdl.h>
#include <comdef.h> // _com_error

#include <d2d1.h> // DIRECT 2D STUFF
#include <dwrite.h>

#include "TavleDefs.h"
#include "COM/Helpers.h"