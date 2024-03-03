#include "xnode.h"
#include "stringutils.h"

std::vector<std::string> xform3dChannels = {
    "tx", "ty", "tz", "rw", "rx", "ry", "rz", "sx", "sy", "sz",
};
std::vector<f64> xform3dValues = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,
};
XNode::XNode() : XNode("") {}

XNode::XNode(std::string _name) : name(_name), path(""), id(0) {
  //debugPrint(this->name);
  for (int i = 0; i < xform3dChannels.size() && i < xform3dValues.size(); i++)
    addTrack(xform3dChannels[i], xform3dValues[i]);
}

void XNode::write(std::fstream& file) {
  //debugPrint(this->name);
  IO::write_TAG(file, "NODE");
  IO::write_STRING(file, name);
  IO::write_STRING(file, path);
  file.write(reinterpret_cast<char*>(&id), sizeof(u64));
  trackCount = tracks.size();
  file.write(reinterpret_cast<char*>(&trackCount), sizeof(u64));
}

void XNode::read(std::fstream& file) {
  std::string TAG = IO::read_TAG(file);
  if (TAG.find("NODE") == std::string::npos && !file.eof()) {
    debugPrint(this->name + " warning : this is not a NODE TAG");
    return;
  }
  name = IO::read_STRING(file);
  path = IO::read_STRING(file);
  file.read(reinterpret_cast<char*>(&id), sizeof(u64));
  file.read(reinterpret_cast<char*>(&trackCount), sizeof(u64));
  //debugPrint(this->name);
}

void XNode::readAll(std::fstream& file) {
  this->read(file);
  // debugPrint(this->name + " trackCount: " + std::to_string(trackCount));
  int tracksFound = 0;
  while (!file.eof() && tracksFound < trackCount) {
    XTrack* track = new XTrack();
    track->readAll(file);
    // this->tracks.push_back(track);
    tracks[track->name] = track;
    tracksFound++;
  }
  //debugPrint(this->name);
}

XTrack* XNode::addTrack(std::string name, f64 value) {
  //debugPrint(this->name);
  XTrack* track;
  if (tracks.find(name) != tracks.end()) {
    debugPrint(this->name + " replacing track: " + name);
    track = tracks[name];
  } else {
    track = new XTrack(name);
  }
  track->value = value;
  // this->tracks.push_back(track);
  tracks[name] = track;
  return track;
}

XTrack* XNode::getTrack(std::string name) {
  if (tracks.find(name) != tracks.end()) return tracks[name];
  debugPrint(this->name + " track " + name + " not found ! creating a new track...");
  //throw std::runtime_error("XNode::getTrack : track " + name + " not found...");
  return addTrack(name);
}

/// <summary>
/// fast method to lookup tracks by name using optional wildcards (? *), and optionally only tracks with keys
/// </summary>
/// <param name="name">track name can optionally use wildcards such as abc*def 
/// (* represents any of characters) or abc?def (* represents any single character)</param>
/// <param name="withKeys">returns only true if found tracks has keys</param>
/// <returns>true if tracks matching conditions have been found</returns>
bool XNode::hasTrack(std::string name, bool withKeys)
{
    if (name.find("?") != std::string::npos || name.find("*") != std::string::npos) {
        for (auto track : tracks) {
            debugPrint(" wildcard search: " + name + " in line: " + track.first);
            if (string_utils::wildmatch(name.c_str(), track.first.c_str()))
                return withKeys ? track.second->getKeys().size() > 0 : true;
        }
    }
    return tracks.find(name) != tracks.end();
}

/// <summary>
/// fast method to lookup tracks by name using optional wildcards (? *), and optionally only tracks with keys
/// </summary>
/// <param name="name">track name can optionally use wildcards such as abc*def 
/// (* represents any of characters) or abc?def (* represents any single character)</param>
/// <param name="withKeys">returns only true if found tracks has keys</param>
/// <returns>a map of name,tracks with tracks matching conditions</returns>
std::map<std::string, XTrack*> XNode::getTracks(std::string name, bool withKeys)
{
    std::map<std::string, XTrack*> matchingTracks;
    if (name.find("?") != std::string::npos || name.find("*") != std::string::npos) {
        for (auto track : tracks) {
            debugPrint(" wildcard search: " + name + " in line: " + track.first);
            if (string_utils::wildmatch(name.c_str(), track.first.c_str())) {
                if(!withKeys)
                    matchingTracks[track.first] = track.second;
                else if (track.second->getKeys().size() > 0)
                    matchingTracks[track.first] = track.second;
            }
        }
    }
    else
        if(tracks.find(name) != tracks.end())
            matchingTracks[name] = tracks[name];
    return matchingTracks;
}

float XNode::getTracksStartTime(std::string name, bool withKeys)
{
    float result = 0.0;
    auto matchingTracks = getTracks(name, withKeys);
    for (auto mt : matchingTracks) {
        return mt.second->getKeys()[0]->time;
    }
    debugPrint("no match: returning value 0.0");
    return 0.0;
}
