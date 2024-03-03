file(GLOB xclipHeaders *.h)
file(GLOB xclipSources *.cpp)
target_sources(xclip PUBLIC ${xclipHeaders} ${xclipSources})
