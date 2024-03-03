#pragma once
#ifndef _XTRACK_H_
#define _XTRACK_H_

#include "xkey.h"

class XTrack {
protected:
  std::vector<XKey*> keys;
  // IO DATA
  u64 keyCount     = 0;
  u64 flags        = 0;
  std::string path = "";
  // use track's value when there is no key in the track.

public:
  std::string name = "";
  f64 value = 0;
  XTrack();
  XTrack(std::string _name);

  void write(std::fstream& file);
  void read(std::fstream& file);
  void readAll(std::fstream& file);
  inline void setPath(std::string _path) { path = _path; }
  //inline f64 getValue() { return value; }
  //inline void setValue(f64 _value) { value = _value; }
  void addKey(XKey* key);
  // void addKey(f64 time, std::vector<f64> const &data, Interpolation
  // interpolation);
  void addKey(std::vector<f64> const& data);
  //void addKey(std::vector<double> const& data);
  void addKeys(std::map<std::string, std::vector<f64>>& data,
               Interpolation interpolation);
   void addKeysFromTable(std::vector<f64> &data, Interpolation
   interpolation);
  std::vector<XKey*> getKeys() const { return keys; }
  // std::map<std::string, std::vector<f64>> getKeysMap();
  // std::string getName() const { return name; }
  f64 evaluateAtTime(f64 time);
};

#endif
