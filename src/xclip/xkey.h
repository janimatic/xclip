#pragma once

#ifndef _XKEY_H_
#define _XKEY_H_

#include "io.h"
#include "mathutils.h"

enum Interpolation {NONE, BEZIER, LINEAR, STEP};

class XKey {
public:
  Interpolation interpolation = Interpolation::NONE;
  // IO DATA
  std::string TAG = "";
  // signed int time;
  f64 time         = 0.0;  // time in seconds (so we can change fps)
  u64 flags  = 0;
  XKey();

  virtual void write(std::fstream& file);
  virtual void read(std::fstream& file);
  // virtual std::vector<f64> asVector();
  std::vector<f64> data;
  const std::vector<f64>& asVector() { return data; }
};

class XBezierFloatKey : public XKey {
public:
  Interpolation interpolation = Interpolation::BEZIER;
  // IO DATA
  f64 value = 0.0;
  XPoint2D inTangent, outTangent;
  XBezierFloatKey();
  XBezierFloatKey(f64 _time, f64 _val, f64 inTanX, f64 inTanY,
                  f64 outTanX, f64 outTanY);
  void write(std::fstream& file);
  void read(std::fstream& file);
};

class XLinearFloatKey : public XKey {
public:
  Interpolation interpolation = Interpolation::LINEAR;
  // IO DATA
  f64 value;
  XLinearFloatKey();
  XLinearFloatKey(f64 _time, f64 _val);
  void write(std::fstream& file);
  void read(std::fstream& file);
};

class XStepFloatKey : public XKey {
public:
  Interpolation interpolation = Interpolation::STEP;
  // IO DATA
  f64 value;
  XStepFloatKey();
  XStepFloatKey(f64 _time, f64 _val);
  void write(std::fstream& file);
  void read(std::fstream& file);
};

#endif
