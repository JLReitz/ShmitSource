#pragma once

#include "../../StdIncludes.hpp"

namespace shmit
{

template <typename T>
static size_t size_in_bytes();

template <typename T, typename U>
struct is_same
{
    bool operator bool();
    bool operator();

};

// Primary type categories
template <typename T>
struct is_void
{

    const bool value = decltype(bool, T)
};

template <typename T>
static bool is_integral();

template <typename T>
static bool is_floating_point();

// Composite type categories
template <typename T>
static bool is_fundamental();

template <typename T>
static bool is_arithmetic();
}