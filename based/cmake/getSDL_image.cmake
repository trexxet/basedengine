include(FetchContent)

option(BUILD_SHARED_LIBS "Build the library as a shared library" ON)
option(SDLIMAGE_VENDORED "Use vendored third-party libraries" OFF)

# Save space & time by disabling unused image formats
option(SDLIMAGE_BMP "Support loading BMP images" ON)
option(SDLIMAGE_PNG "Support loading PNG images" ON)
option(SDLIMAGE_AVIF "Support loading AVIF images" OFF)
option(SDLIMAGE_GIF "Support loading GIF images" OFF)
option(SDLIMAGE_JPG "Support loading JPEG images" OFF)
option(SDLIMAGE_JXL "Support loading JXL images" OFF)
option(SDLIMAGE_LBM "Support loading LBM images" OFF)
option(SDLIMAGE_PCX "Support loading PCX images" OFF)
option(SDLIMAGE_PNM "Support loading PNM images" OFF)
option(SDLIMAGE_QOI "Support loading QOI images" OFF)
option(SDLIMAGE_SVG "Support loading SVG images" OFF)
option(SDLIMAGE_TGA "Support loading TGA images" OFF)
option(SDLIMAGE_TIF "Support loading TIFF images" OFF)
option(SDLIMAGE_WEBP "Support loading WEBP images" OFF)
option(SDLIMAGE_XCF "Support loading XCF images" OFF)
option(SDLIMAGE_XPM "Support loading XPM images" OFF)
option(SDLIMAGE_XV "Support loading XV images" OFF)
option(SDLIMAGE_AVIF_SAVE "Add AVIF save support" OFF)
option(SDLIMAGE_JPG_SAVE "Add JPEG save support" OFF)
option(SDLIMAGE_PNG_SAVE "Add PNG save support" OFF)

FetchContent_Declare(
	SDL_image
	GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
	GIT_TAG         main
	GIT_SUBMODULES  ""
	GIT_SHALLOW     TRUE
	GIT_PROGRESS    FALSE
	EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SDL_image)
target_compile_options(SDL3_image-shared PRIVATE -w)
