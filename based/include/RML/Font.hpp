#pragma once

#include <string>

#include <RmlUi/Core/StyleTypes.h>

namespace Based::RML {

struct Font {
	std::string family;
	Rml::Style::FontStyle style;
	Rml::Style::FontWeight weight;
	int size_px;
	std::string color;
};

}
