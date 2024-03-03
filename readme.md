XClip keyframe animation transfert
==================================

XClip is a tool for storing scene animations for
interchange between graphics applications.
The goal is 
    - to enable the editing 2d and 3d keyframe animation, 
by keeping the original curves intact.
    - to focus on fast and easy animation and parameters transfert

* Motivation

There are alternative libraries to transfert animations, but...
    - Pixar's [USD](https://github.com/PixarAnimationStudios/OpenUSD) (Universal Scene Description) 
    bakes curve at each frame
    - Kronos Group's [gLTF](https://github.com/KhronosGroup/glTF) (GL Transmission Format) 
    supports bezier splines, but with limitations that makes it unmanageable for an animator
    (tangents are always at 1/3 from neighbors keyframes). 
    Example in Tahoma2D [gltfio] (https://github.com/janimatic/tahoma2d/blob/48c10581bdb847ef615e405b625b798c4e1b8ee0/toonz/sources/toonz/gltfio.cpp#L350) branch
    - Autodesk's [FBX](https://www.autodesk.com/products/fbx/overview) 
    complete 3d scene description is closed source and heavy
    - [OCA](https://rxlaboratory.org/tools/oca/) Open Cell Animation for 2d animation's roadmap mentioned the idea of 
    a new OKA Open Keyframe Animation json specification but it's not done.
I don't see why cubic bezier couldn't be transfered between different applications.
    - [Collada](https://www.khronos.org/collada/) might be an alternative
    
All those libraries tends to transfert the complete scene.
In many case, we already have good file formats to transfert geometry
(like usd for 3d and OCA Open Cell Animation for 2d animation)
Most technical directors keep writing scripts to transfert cameras, for example.
XClip provides the transfert of parameters and animations,
including transformations, and attributes of objects, cameras and any custom node.

* XClip c++ library

The library supports 
    - fast binary files i/o
    - cubic bezier with 2d handles, linear and step (mixed) keyframes.
    - a simple hierarchy (XClip>XNodes>XTracks>XKey)
    - easy name remapping of nodes and parameters :
    wildcard strings allow to deal with grouped parameters 
    (ex: Center.X, Center.Y can be gathered with Center.\*)
    Wildcards support single '\?' or multi '\*' character patterns
    This allows to deal easily with different grouped parameters structures,
    without making the library too much complicated
    (fusion ex: Transform.Center.X, Camera3D.Transform3DOp.Translate.X...) 
    - a user data container which can store any type of 
    custom data that at the top level (such as start/end frames, scene scale, ...)
    - crossplatform x64 for number data types
    - basic math utility methods

* XClip swig wrappers

Lua and python wrappers are available
It's very easy to support more languages through [SWIG](https://www.swig.org/compare.html)

* Tahoma2D c++ plugin

xclipio complements the open cell animation ocaio
Both importer and exporter are working.
TODO : A multiplier, to be defined, should be applied to point coordinates, 
in order to match fusion coordinates.


* Blackmagic fusion module

Thanks to the lua swig wrapper, a lua module for fusion has been made.
The importer is working and the exporter is still being worked on.
The actual precomp is work in progress (implying a new Multiplane fuse node)

* contributions are welcome

[XClip first version](https://app.assembla.com/spaces/xclip/wiki) was released in 2008
and used on big productions at Cube.
It was used as the base to create nice tools for 3d animators :
Reusable 3d Animation banks/libraries, copy/paste/paste opposite tools, 
name remapping to transfert animation clips between different characters.
Xclip rebirth is in early stage and the design is still adjusted,
as the integration to other applications opens new questions to solve.
Your contribution is most welcome !

* Roadmap

    - Support xclipio in more open source applications (Blender, Natron, etc...)
    - Support xclipio in more commercial applications (Maya, Nuke, etc...)
    with the xclip python module, this should be easy enough...
    - Add complementary tools to help scenes translations:
        - coordinates system conversions
        - Custom nodes that make translation easier :
            - fusion multiplane camera node / fuse
    - implement procedural recursive methods to parse the scenes 
    (so that by default xclip could transfert absolutely everything)
    and limit io with name filters
    - add bezier curve fitting to rebuild bezier from a sampled curve
    - add internal curve evaluation 
    [Collada forum](https://community.khronos.org/t/animation-bezier-interpolation/5891/6) could be a good ressource

* my personal usage

As a traditional animation movie director, my need is to make 2d cell animation easier.
This tool will complement OCA traditional animation transfert :
    - OCA resolve/fusion importer https://github.com/janimatic/OCA-fusion-importer
    - For krita ocaio is working 
    Krita importer https://github.com/janimatic/OCA-krita-importer
    Krita exporter https://github.com/RxLaboratory/OCA-Krita
    But the api is too limited for now, to implement xclipio in Krita
    (but if you have access to linux, you can contribute to the krita animation api!)


Supported Platforms
-------------------

XClip is primarily developed under windows 10 x64, 
but it should build easily on macOS and linux.
Contributors are most welcome on any platform...

Getting the binary release
--------------------------
TODO

Installing the binary release
-----------------------------
    - Fusion module
    Under windows, you can either run xclip/lua/install.bat, or follow this procedure : 
        - copy manually xclip\xclip.dll and fusion\xclipioFusion.lua in 
        %ProgramData%\Blackmagic Design\DaVinci Resolve\Fusion\Modules\Lua\
        - copy manually exportXClip.lua and importXClip.lua in
        %AppData%\Blackmagic Design\DaVinci Resolve\Support\Fusion\Scripts\Utility
        
    - Tahoma2D plugin
    XClip is not yet part of the official tahoma2d master build
    A new PR will be submited soon and that will make binaries available 
    for windows mac and linux
    The work in progress can be found here
    https://github.com/tahoma2d/tahoma2d/compare/master...janimatic:tahoma2d:xclipio

Getting and Building the Code
-----------------------------

#### 1. Install prerequisites
- Required:
    - C++ compiler:
        - gcc
        - Xcode
        - Microsoft Visual Studio (tested on vs2019)
    - CMake https://cmake.org/download/
    - SWIG https://www.swig.org/download.html
- Required to build the lua wrapper :
    - lua Lua51 includes and libraries (use version 5.1, in order to match fusion version of lua)
    lua includes and libraries found here
    https://luabinaries.sourceforge.net/download.html
    Make sure you use x64 lib, if targeting an x64 build...
    Tested with lua-5.1.5_Win64_dll17_lib

- Required to build the python wrapper :
    - python (tested with Python 3.7.0 64 bit, but it should work with Python 2 too)
    
#### 2. Download the XClip source code
You can download source code archives from [GitHub](https://www.github.com/janimatic/XClip) or use `git` to clone the repository.
```
> git clone https://www.github.com/janimatic/XClip
Cloning into 'OpenUSD'...
```

#### 3. Run cmake
Or CMake-gui...

    - XClip C++ libray
    browse source : xclip/src
    browse build : xclip/build (new folder)
    Configure
    Configure
    until there is no error
    Generate
    build the xclip project, located in the build folder,  in Release mode
    the xclip test application located in /build/Release
    let you test the library in c++ by editing the main.cpp file
    it use a exemple header simulation Tahoma2D data types
    For a real c++ application integration example, check xclipio in tahoma2D
    https://github.com/tahoma2d/tahoma2d/compare/master...janimatic:tahoma2d:xclipio
    
    - XClip python3
    browse source : xclip/python
    browse build : xclip/python/build
    Configure
    Set swig include dir if necessary
    Configure
    Set Python3 include dir if necessary
    Configure
    until there is no error
    Generate
    build the xclip project in Release mode
    python/build/Release/_xclip.pyd is the python binary library.
    You can test a sample python script python/xclipTool.py
    The pyd file is copied in python/xclip (SWIG_OUTFILE_DIR) by cmake.
    
    - XClip lua
    browse source : xclip/lua
    browse build : xclip/lua/build
    Configure
    Set swig include dir if necessary
    Configure
    Set LUA_INCLUDE_DIR if necessary (pointing to the include folder)
    Set LUA_LIBRARY if necessary (pointing to the file lua5.1.lib or lua5.1.a)
    until there is no error
    Generate
    build the xclip project in Release mode
    lua/build/Release/xclip.dll / so is the lua binary library.
    To test the lua wrapper outside of fusion
    the lua 5.1 interpreter should be installed
    You can test a sample lua script lua/xclip/runme.lua
    The dll is copied in the same folder (SWIG_OUTFILE_DIR) by cmake.
    This command should be adjusted if your os is not windows 
    (to copy xclip.so instead of xclip.dll)
    