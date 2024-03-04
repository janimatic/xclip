file(GLOB xclipHeaders *.h)
file(GLOB xclipSources *.cpp)
target_sources(libxclip PUBLIC ${xclipHeaders} ${xclipSources})
