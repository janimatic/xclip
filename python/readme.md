xclip python swig wrapper.

* build
Requieres swig + cmake (tested with visual studio 2019) :
- cmake configure
- point cmake to swig executable, configure & generate
- build the xclip project in Release mode
The _xclip.pyd compiled python module file will be copied to SWIG_OUTFILE_DIR (python/xclip directory)

* usage
cd in python/xclip directory and run :
python xclipTool.py
http://web.mit.edu/ghudson/trac/src/swig-1.3.25/Doc/Manual/Python.html#Python_nn9
