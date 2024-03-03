/* File : xcl.i */
%module xcl
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

/*
// https://stackoverflow.com/questions/50563371/wrapping-c-function-to-take-lua-table-of-strings-using-swig
void readTable(std::vector<float> values) {
    for (size_t i=0; i<values.size(); ++i) {
        std::cout << "VALUE : " << values[i] << '\n';
    }
}
*/
%}

%include stl.i
namespace std {
    %template(nodeVector) vector<XNode *>;
    %template(trackMap) map<std::string, XTrack>;
    %template(floatVector) vector<f64>;
    %template(keyVector) vector<XKey *>;
    %template(keyMap) map<std::string, std::vector<f64>>;
    //%template(keyMap) map<std::string, floatVector>;
}

/*
%include "exception.i"
%typemap(in) std::vector<float>
{
    if (!lua_istable(L,1)) {
      SWIG_exception(SWIG_RuntimeError, "argument mismatch: table expected");
    }
    lua_len(L,1);
    size_t len = lua_tointeger(L,-1);
    $1.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        lua_pushinteger(L,i+1);
        lua_gettable(L,1);
        $1.push_back(lua_tonumber(L,-1));
    }
}
void readTable(std::vector<float> values);
*/

%include "../src/xclip/io.h"
%include "../src/xclip/mathutils.h"
%include "../src/xclip/stringutils.h"
%include "../src/xclip/xclip.h"
%include "../src/xclip/xkey.h"
%include "../src/xclip/xnode.h"
%include "../src/xclip/xtrack.h"
%include "../src/xclip/xuserdata.h"
