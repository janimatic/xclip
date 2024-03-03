/* File : xcl.i */
%module xclip
typedef double f64;

%{
#include "../src/xclip/io.h"
#include "../src/xclip/mathutils.h"
#include "../src/xclip/stringutils.h"
#include "../src/xclip/xclip.h"
#include "../src/xclip/xkey.h"
#include "../src/xclip/xnode.h"
#include "../src/xclip/xtrack.h"
#include "../src/xclip/xuserdata.h"
%}

%include stl.i
namespace std {
    %template(nodeVector) vector<XNode *>;
    %template(trackMap) map<std::string, XTrack>;
    %template(floatVector) vector<f64>;
    %template(keyVector) vector<XKey *>; // track.getKeys()
    %template(keyMap) map<std::string, std::vector<f64>>;
}

%include "../src/xclip/io.h"
%include "../src/xclip/mathutils.h"
%include "../src/xclip/stringutils.h"
%include "../src/xclip/xclip.h"
%include "../src/xclip/xkey.h"
%include "../src/xclip/xnode.h"
%include "../src/xclip/xtrack.h"
%include "../src/xclip/xuserdata.h"
