#pragma once

#include <RmlUi/Core/Traits.h>

#include "Defs.hpp"
#include "Geometry.hpp"

#define BASED_RML_ID_DECL(C) extern template class BASED_API Rml::Family<C>

BASED_RML_ID_DECL(Based::Rect2D<int>);

#undef BASED_RML_ID_DECL
