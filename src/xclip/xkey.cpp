#include "xkey.h"

XKey::XKey() : time(0.0), flags(0), TAG("NONE") {
  // debugPrint("");
  data = {f64(interpolation), time};
}

void XKey::write(std::fstream& file) {
  IO::write_TAG(file, TAG);
  file.write(reinterpret_cast<char*>(&time), sizeof(f64));
  file.write(reinterpret_cast<char*>(&flags), sizeof(u64));
}

void XKey::read(std::fstream& file) {
  // std::string tag = IO::read_TAG(file);
  TAG = IO::read_TAG(file);
  file.read(reinterpret_cast<char*>(&time), sizeof(f64));
  file.read(reinterpret_cast<char*>(&flags), sizeof(u64));
  data = {f64(interpolation), time};
}

XBezierFloatKey::XBezierFloatKey() { TAG = "KBFL"; }

XBezierFloatKey::XBezierFloatKey(f64 _time, f64 _val, f64 inTanX,
                                 f64 inTanY, f64 outTanX, f64 outTanY) {
  TAG          = "KBFL";
  time         = _time;
  value        = _val;
  inTangent.x  = inTanX;
  inTangent.y  = inTanY;
  outTangent.x = outTanX;
  outTangent.y = outTanY;
  data         = {f64(interpolation), time,         value,       inTangent.x,
          inTangent.y,          outTangent.x, outTangent.y};
}

void XBezierFloatKey::write(std::fstream& file) {
  // debugPrint("");
  XKey::write(file);
  file.write(reinterpret_cast<char*>(&value), sizeof(f64));
  file.write(reinterpret_cast<char*>(&inTangent.x), sizeof(f64));
  file.write(reinterpret_cast<char*>(&inTangent.y), sizeof(f64));
  file.write(reinterpret_cast<char*>(&outTangent.x), sizeof(f64));
  file.write(reinterpret_cast<char*>(&outTangent.y), sizeof(f64));
}

void XBezierFloatKey::read(std::fstream& file) {
  // debugPrint("");
  XKey::read(file);
  file.read(reinterpret_cast<char*>(&value), sizeof(f64));
  file.read(reinterpret_cast<char*>(&inTangent.x), sizeof(f64));
  file.read(reinterpret_cast<char*>(&inTangent.y), sizeof(f64));
  file.read(reinterpret_cast<char*>(&outTangent.x), sizeof(f64));
  file.read(reinterpret_cast<char*>(&outTangent.y), sizeof(f64));
  data = {f64(interpolation), time,         value,       inTangent.x,
          inTangent.y,          outTangent.x, outTangent.y};
}

XLinearFloatKey::XLinearFloatKey() { TAG = "KLFL"; }

XLinearFloatKey::XLinearFloatKey(f64 _time, f64 _val) {
  TAG   = "KLFL";
  time  = _time;
  value = _val;
  data  = {f64(interpolation), time, value};
}

void XLinearFloatKey::write(std::fstream& file) {
  // debugPrint("");
  XKey::write(file);
  file.write(reinterpret_cast<char*>(&value), sizeof(f64));
}

void XLinearFloatKey::read(std::fstream& file) {
  // debugPrint("");
  XKey::read(file);
  file.read(reinterpret_cast<char*>(&value), sizeof(f64));
  data = {f64(interpolation), time, value};
}

// only the XStepFloatKey tag (and later evaluation) differs from
// XLinearFloatKey...
XStepFloatKey::XStepFloatKey() { TAG = "KSFL"; }

XStepFloatKey::XStepFloatKey(f64 _time, f64 _value) {
  TAG   = "KSFL";
  time  = _time;
  value = _value;
  data  = {f64(interpolation), time, value};
}

void XStepFloatKey::write(std::fstream& file) {
  // debugPrint("");
  XKey::write(file);
  file.write(reinterpret_cast<char*>(&value), sizeof(f64));
}

void XStepFloatKey::read(std::fstream& file) {
  // debugPrint("");
  XKey::read(file);
  file.read(reinterpret_cast<char*>(&value), sizeof(f64));
  data = {f64(interpolation), time, value};
}
