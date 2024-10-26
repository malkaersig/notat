#pragma once

#include <memory>

template<typename T>
using SHPTR = std::shared_ptr<T>;

template<typename T>
using UNPTR = std::unique_ptr<T>;