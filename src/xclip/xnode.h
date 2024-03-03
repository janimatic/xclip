#pragma once

#ifndef _XNODE_H_
#define _XNODE_H_

#include "xtrack.h"

class XTrack;
//class XClip;

class XNode {
protected:
  //XClip* parentClip;

public:
  // IO DATA
  std::string name      = "";
  std::string path      = "";
  u64 trackCount        = 0;
  u64 id                = 0;
  // std::vector<XTrack*> tracks;
  std::map<std::string, XTrack*> tracks;

  XNode();
  XNode(std::string _name);
  void write(std::fstream& file);
  void read(std::fstream& file);
  void readAll(std::fstream& file);
  inline void setPath(std::string _path) { path = _path; }
  inline void setId(u64 _id) { id = _id; }
  XTrack* addTrack(std::string name, f64 value = 0.0);
  XTrack* getTrack(std::string name);
  bool hasTrack(std::string name, bool withKeys = false);
  std::map<std::string, XTrack*> getTracks(std::string name, bool withKeys = false);
  float getTracksStartTime(std::string name, bool withKeys = false);
};

#endif
