XClip keyframe animation transfert
==================================

XClip is a tool for storing scene animations for interchange between graphics applications.
The goal is 
- to allow the editing 2d and 3d keyframe animation, by keeping the original curves intact.
- to focus on fast and easy animation and parameters transfert.
XClip is not a scene assembly tool by itself, but can help scene assembly with easy name remapping methods.

# Motivation

There are alternative libraries to transfert animations, but...

- Pixar's [USD](https://github.com/PixarAnimationStudios/OpenUSD) (Universal Scene Description) bakes curve at each frame
- Kronos Group's [gLTF](https://github.com/KhronosGroup/glTF) (GL Transmission Format) supports bezier splines, but with limitations that makes it unmanageable for an animator (tangents are always at 1/3 from neighbors keyframes). Example in  Tahoma2D [gltfio](https://github.com/janimatic/tahoma2d/blob/48c10581bdb847ef615e405b625b798c4e1b8ee0/toonz/sources/toonz/gltfio.cpp) branch
 - Autodesk's [FBX](https://www.autodesk.com/products/fbx/overview) complete 3d scene description is closed source and heavy
 - [OCA](https://rxlaboratory.org/tools/oca/) Open Cell Animation for 2d animation's roadmap mentioned the idea of a new [OKA](http://oca.rxlab.guide/roadmap.html#keyframe-animation) Open Keyframe Animation json specification, but it's not done.
- [Collada](https://www.khronos.org/collada/) might be a valid alternative
    
All those libraries tends to transfert the complete scene.
In many case, we already have good file formats to transfert geometry and images (like usd for 3d and OCA Open Cell Animation for 2d animation)
Most technical directors keep writing scripts to transfert cameras, for example.
XClip provides the transfert of parameters and animations, including transformations, and attributes of objects, cameras and any custom node. I don't see why cubic bezier couldn't be transfered between different applications.

# XCLIP
## XClip c++ library

The library supports 

- fast binary files i/o
- cubic bezier with 2d handles, linear and step (mixed) keyframes.
- a simple hierarchy (XClip>XNodes>XTracks>XKey)
- easy name remapping of nodes and parameters :
    wildcard strings allow to deal with grouped parameters  (ex: Center.X, Center.Y can be gathered with Center.\*). Wildcards support single '\?' or multi '\*' character patterns
    This allows to deal easily with different grouped parameters structures, without making the library too much complicated (fusion ex: Transform.Center.X, Camera3D.Transform3DOp.Translate.X...) 
- a user data container which can store arbitrary string and number data that at the top level (such as start/end frames, scene scale, ...)
- crossplatform x64 for number data types
- basic math utility methods

## XClip swig wrappers

Lua and python wrappers are available
It's very easy to support more languages through [SWIG](https://www.swig.org/compare.html)

## Tahoma2D c++ plugin

[xclipio](https://github.com/tahoma2d/tahoma2d/compare/master...janimatic:tahoma2d:xclipio) for [Tahoma2D](https://github.com/tahoma2d/tahoma2d) complements the [OCA](https://rxlaboratory.org/tools/oca/) Open Cell Animation ocaio
Roadmap :
- [x] xclip importer
- [x] xclip exporter
- [ ] Store a multiplier in  xclip's userdata "scale", in order to match fusion coordinates (this was hard-coded in gltfio to match blender coordinates, but should probably be defined with the dpi). 


## Blackmagic fusion module

Thanks to the lua swig wrapper, a lua module for fusion has been made.
Roadmap :
- [x] xclip importer parses the parameters in a recursive method, so that by default xclip could transfert absolutely everything, and only limit io with name filters/wildcards.
- [ ] xclip exporter
- [ ] a new Multiplane fuse node, to match the 2d camera camera and make precomp easier

# contributions are welcome

[XClip first version](https://app.assembla.com/spaces/xclip/wiki) was released in 2008
and used on big productions at Cube. It was used as the base to create nice tools for 3d animators :
Reusable 3d Animation banks/libraries, copy/paste/paste opposite tools, name remapping to transfert animation clips between different characters.
Xclip rebirth is in early stage and the design is still adjusted, as the integration to other applications opens new questions to solve.
Your contribution is most welcome !

# Roadmap
- [ ] Fix the cmake multi-platform workflow error : Could NOT find Lua (see Github worflow at the bottom of this page

- [ ] Support xclipio in open and close source apps with the xclip c++ library
 	- [x] Tahoma2D / OpenToonz
	- [ ] Natron

- [ ] Support xclipio in open and close source apps with the xclip lua module
 	- [x] Resolve / Fusion
	 	- [x] importer
	 	- [ ] exporter
	 	- [ ] precomp

- [ ] Support xclipio in open and close source apps with the xclip python module :
	- [ ] Blender
	- [ ] Maya
	- [ ] Nuke

- [ ] Add complementary tools to help scenes translations:
	- [ ]  coordinates system conversions
	- [ ]  recursive scene parsers for each
- [ ] Create nodes that make translation easier :
	- [ ]  fusion multiplane camera node / fuse
- [ ] implement recursive methods to parse the scenes (so that by default xclip could transfert every parameter from every node with minimal code) and limit io with name filters
- [ ] add internal curve evaluation 
See
[Collada forum](https://community.khronos.org/t/animation-bezier-interpolation/5891/6) 
[A Primer on Bezier Curves](https://pomax.github.io/bezierinfo/) 
[Practical Guide to Bezier Curves](https://www.gamedev.net/tutorials/programming/math-and-physics/practical-guide-to-bezier-curves-r3166/)
- [ ] add bezier curve fitting to rebuild a cubic bezier curve from a sampled curve. See 
[Krita Bezier curve fitting](https://github.com/KDE/krita/blob/01a4713225479b752013269df63f4ef96e605adb/libs/flake/KoCurveFit.cpp)
[Fitting cubic Bezier curves article](https://raphlinus.github.io/curves/2021/03/11/bezier-fitting.html)

# my personal usage

As a traditional animation movie director, my need is to make 2d cell animation easier.
This tool will complement OCA traditional animation transfert :
- [OCA resolve/fusion importer](https://github.com/janimatic/OCA-fusion-importer)
- For krita ocaio is working :
    - [Krita importer](https://github.com/janimatic/OCA-krita-importer)
    - [Krita exporter](https://github.com/RxLaboratory/OCA-Krita) by Duduf how defined OCA
-  To implement xclipio in Krita, the api is too limited for now (but if you have access to linux, you can contribute to the krita animation api)...


Supported Platforms
-------------------

XClip is primarily developed under windows 10 x64, but it should build easily on macOS and linux. Contributors are most welcome on any platform...

Getting the binary release
--------------------------
TODO

Installing the binary release
-----------------------------

- Fusion lua module
Under windows, you can run xclip/lua/install.bat 
(which copies xclip\xclip.dll and fusion\xclipioFusion.lua in %ProgramData%\Blackmagic Design\DaVinci Resolve\Fusion\Modules\Lua\ and copies exportXClip.lua and importXClip.lua in%AppData%\Blackmagic Design\DaVinci Resolve\Support\Fusion\Scripts\Utility)
        
- Tahoma2D plugin
    XClip is not yet part of the official tahoma2d master build
    A new PR will be submited soon and that will make binaries available for windows, mac and linux
    The work in progress can be found in the [tahoma2D xclipio branch](https://github.com/tahoma2d/tahoma2d/compare/master...janimatic:tahoma2d:xclipio)

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
Cloning into 'xclip'...
```

#### 3. Run cmake
    
    Here is the procedure with CMake-gui under windows :

    Browse source : /xclip
    Browse build : /xclip/bin (That is the new folder, where the vs solution and projects will be generated)
    Configure
    You can choose the default generator (vs2019 in my case) and just clic finish
    Unless added you've added python, swig and lua to the path environment variable,
    an error message appears :
    Set swig include dir if necessary
    Set Python3 include dir if necessary
    Set LUA_INCLUDE_DIR if necessary (pointing to the include folder)
    Set LUA_LIBRARY if necessary (pointing to the file lua5.1.lib or lua5.1.a)
    Configure until there is no error in red
    Generate
    Open the xclip solution (xclip.sln in my case) found in /xclip/bin
    Switch to Release mode
    Build all the projects
    
    
- XClip C++ library example application
    the xclip test application located in /xclip/bin/Release
    let you test the library in c++ by editing the main.cpp file
    it use a example header simulation Tahoma2D data types
    For a real c++ application integration example, check the [tahoma2D xclipio branch](https://github.com/tahoma2d/tahoma2d/compare/master...janimatic:tahoma2d:xclipio)
    
- XClip python
    /xclip/python/build/Release/_pyxclip.pyd is the python binary library.
    This file is copied in /xclip/python/pyxclip (SWIG_OUTFILE_DIR) by cmake.
    You can test a sample python script /xclip/python/xclipTool.py
    
- XClip lua
    /xclip/lua/build/Release/luaxclip.dll (or .so) is the lua binary library.
    To test the lua wrapper outside of fusion, the lua 5.1 interpreter should be installed
    You can test a sample lua script /xclip/lua/luaxclip/xclipTool.lua
    The dll is copied in the same folder (SWIG_OUTFILE_DIR) by cmake.
    This command should be adjusted if your os is not windows (to copy .so instead of .dll)
    See Installing the binary release if you want to use the lua module in fusion


Github worflow
--------------

Trying to fix this error in github workflow:

Could NOT find Lua (missing: LUA_LIBRARIES LUA_INCLUDE_DIR) 

![cmake multi-platform workflow](https://github.com/janimatic/xclip/actions/workflows/cmake-multi-platform.yml/badge.svg)
