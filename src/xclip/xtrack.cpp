#include "xtrack.h"

XTrack::XTrack() : XTrack("") {}

XTrack::XTrack(std::string _name) : name(_name) {}

void XTrack::write(std::fstream& file) {
  //debugPrint(this->name + " keys.size(): " + std::to_string(keys.size()));
  IO::write_TAG(file, "TRCK");
  file.write(reinterpret_cast<char*>(&flags), sizeof(u64));
  IO::write_STRING(file, name);
  IO::write_STRING(file, path);
  IO::write_FLOAT(file, value);
  keyCount = keys.size();
  file.write(reinterpret_cast<char*>(&keyCount), sizeof(u64));
}

void XTrack::read(std::fstream& file) {
  std::string TAG = IO::read_TAG(file);
  if (TAG.find("TRCK") == std::string::npos && !file.eof()) {
    std::cout << "warning : this is not a TRCK TAG" << std::endl;
    return;
  }
  file.read(reinterpret_cast<char*>(&flags), sizeof(u64));
  name = IO::read_STRING(file);
  path = IO::read_STRING(file);
  value = IO::read_FLOAT(file);
  file.read(reinterpret_cast<char*>(&keyCount), sizeof(u64));
  // debugPrint(this->name + " keyCount: " + std::to_string(keyCount));
}

void XTrack::readAll(std::fstream& file) {
  this->read(file);
  // debugPrint(this->name + " keyCount: " + std::to_string(keyCount));
  int keysFound = 0;
  while (!file.eof() && keysFound < keyCount) {
    XKey* xkey;
    std::string TAG = IO::check_TAG(file);
    if (TAG.find("KBFL") != std::string::npos && !file.eof()) {
      xkey = new XBezierFloatKey();
    } else if (TAG.find("KLFL") != std::string::npos && !file.eof()) {
      xkey = new XLinearFloatKey();
    } else if (TAG.find("KSFL") != std::string::npos && !file.eof()) {
      xkey = new XStepFloatKey();
    } else {
      debugPrint(this->name + " unknown key type...STOP with TAG " + TAG);
      break;
    }
    if (xkey) {
      // debugPrint(this->name + " xkey->read(file);");
      xkey->read(file);
      // debugPrint(this->name + " track->addKey( xkey );");
      this->addKey(xkey);
      ++keysFound;
    } else {
      debugPrint(this->name + " xkey is NULL...");
    }
  }
}

void XTrack::addKey(XKey* key) {
  // debugPrint(this->name + " xkey->TAG: " + key->TAG);
  if (key->TAG.find("KBFL") != std::string::npos) {
    auto xkey = dynamic_cast<XBezierFloatKey*>(key);
    keys.push_back(xkey);
  } else if (key->TAG.find("KLFL") != std::string::npos) {
    auto xkey = dynamic_cast<XLinearFloatKey*>(key);
    keys.push_back(xkey);
  } else if (key->TAG.find("KSFL") != std::string::npos) {
    auto xkey = dynamic_cast<XStepFloatKey*>(key);
    keys.push_back(xkey);
  } else {
    // mprintf("warning ! addKey failed, type not supported : %s",
    // xkey->TAG.c_str());
    debugPrint(this->name +
               " warning ! addKey failed, type not supported :" + key->TAG);
  }
}

/// <summary>
/// addKey from a vector of f64
/// data = { f64(interpolation), time , val, inTangent.x , inTangent.y,
/// outTangent.x , outTangent.y };
/// </summary>
/// <param name="data"></param>
void XTrack::addKey(std::vector<f64> const& data) {
//void XTrack::addKey(std::vector<double> const& data) {
  if (data.size() < 7) {
    debugPrint("empty data !");
  }
  Interpolation interpolation = static_cast<Interpolation>(int(data[0]));
  switch (interpolation) {
  case Interpolation::NONE:
    break;
  case Interpolation::BEZIER:
    if (data.size() < 7)
      debugPrint("missing data for bezier key!");
    else
      addKey(new XBezierFloatKey(data[1], data[2], data[3], data[4], data[5], data[6]));
    break;
  case Interpolation::LINEAR:
    if (data.size() < 3)
      debugPrint("missing data for linear key!");
    else
      addKey(new XLinearFloatKey(data[1], data[2]));
    break;
  case Interpolation::STEP:
    if (data.size() < 3)
      debugPrint("missing data for step key!");
    else
      addKey(new XStepFloatKey(data[1], data[2]));
    break;
  default:
    break;
  }
}

// using string keys to conform to python dictionary...
// but lua is not happy...
void XTrack::addKeys(std::map<std::string, std::vector<f64>>& data,
                     Interpolation interpolation) {
  switch (interpolation) {
  case Interpolation::NONE:
    break;
  case Interpolation::BEZIER:
    for (auto k : data) {
      if (k.second.size() < 5)
        debugPrint(" missing data for bezier key!");
      else
        addKey(new XBezierFloatKey(stof(k.first), k.second[0], k.second[1],
                                   k.second[2], k.second[3], k.second[4]));
    }
    break;
  case Interpolation::LINEAR:
    for (auto k : data) addKey(new XLinearFloatKey(stof(k.first), k.second[0]));
    break;
  case Interpolation::STEP:
    for (auto k : data) addKey(new XStepFloatKey(stof(k.first), k.second[0]));
    break;
  default:
    break;
  }
}

void XTrack::addKeysFromTable(std::vector<f64>& data, Interpolation interpolation)
{
	switch (interpolation)
	{
	case Interpolation::NONE:
		break;
	case Interpolation::BEZIER:
		for (int k = 0; k < data.size(); k += 7)
			addKey(new XBezierFloatKey(data[1 + k], data[2 + k], data[3 + k], data[4 + k], data[5 + k], data[6 + k]));
		break;
	case Interpolation::LINEAR:
        for (int k = 0; k < data.size(); k += 3)
            addKey(new XLinearFloatKey(data[1 + k], data[2 + k]));
        break;
	case Interpolation::STEP:
        for (int k = 0; k < data.size(); k += 3)
            addKey(new XStepFloatKey(data[1 + k], data[2 + k]));
        break;
	default:
		break;
	}
}

f64 XTrack::evaluateAtTime(f64 time) {
  // TODO :
  debugPrint(
      " bezier/step/linear/mixed curve evaluation is not yet implemented!");
  return 0.0f;
}
