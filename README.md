# basedengine

## Why Based?

* It's written in C++ using conservative way - no `std::sometimes_constexprs_if_otherwise_move(auto prxlvalue &&&)`. Some convenient and proven modern features are still in use, e.g., `std::format()`. The #1 rule is Keeping It Simple and Stupid.
* It does not treat user as an idiot. Compared to "conventional" engines & frameworks, this one does not care if you want to shoot yourself in the foot. Instead of making shittons of interfaces, Based makes member variables public. Instead of wasting CPU clock ticks on 900000 checks, Based trusts that you won't be passing garbage.
* I lied. Based is not an engine. It's the glue that just binds underlying components together and provides some convenient tools to implement common features.
* Tabs, not spaces.
* To be continued

## Components
* OpenGL >= 3.3
* glad 2.0.8
* glm 1.0.1
* SDL 3.1.7
* SDL_image 3.1.0
* Lua 5.4.7
* Sol 3.5.0
* RmlUi 6.1

#### Code style (which I may forget)
* Tabs are used for indentation. Spaces are used for alignment. Thus, tab width is not important.
* Types are PascalCase. Variables are camelCase. Methods and functions are snake_case.
* There must always be exactly one space between the function name and the opening parenthesis. The exceptions are **calling** a function with no arguments - must have no space before parenthesis, and constructor initializer list - whatever looks better.
* Every file must end with an empty line
* Includes are sorted in 3 groups separated by empty line: system headers (`<string>`), dependency library headers (`<glad/gl.h>`) and Based headers (`"Logger.hpp"`). Each group is sorted alphabetically and per-subdirectory. Source file starts with a corresponding include, header file starts with `#pragma once` if needed.
* No new line for opening curly bracket with the exception of function/method + initializer list taking multiple lines.
* Doxygen comments use triple slash and same indentation as the related code. Commands use @.
