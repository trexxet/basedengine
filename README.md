# basedengine

## Why Based?

* It's written in C++ using conservative way - no `std::sometimes_constexprs_if(auto prxlvalue &&&)`. Some convenient and proven modern features are still in use, e.g., `std::format()`. The #1 rule is Keeping It Simple and Stupid.
* It does not treat user as an idiot. Compared to "conventional" engines & frameworks, this one does not care if you want to shoot yourself in the foot. Instead of making shittons of interfaces, Based makes member variables public. Instead of wasting CPU clock ticks on 900000 checks, Based trusts that you won't be passing garbage.
* I lied. Based is not an engine. It's the glue that just binds underlying components together and provides some convenient tools to implement common features.
* To be continued

## Components
* OpenGL 3.3
* SDL 3.0
* Lua 5.4.7
* Sol 3.3.0
* Nuklear
