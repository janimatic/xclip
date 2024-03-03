#include "xclip.h"

XClip::XClip() {
  //std::cout << "XClip version " << XCLIP_VERSION << std::endl;
  // debugPrint("");
  _NodeState = NodeUndefined;
  nodeCount  = 0;
  // scale = 1.0f;
  // userDataString = "";
  uData.setIntVariable("version", XCLIP_VERSION);
  uData.setFloatVariable("scale", 1.0);
  uData.setFloatVariable("startTime", 0.0);
  uData.setFloatVariable("endTime", 0.0);
  header = XCLIP_HEADER;
  // deprecate ?
  // startFrame = 0.0f;
  // endFrame = 100.0f;
  /*
  modelName = "";
  searchByName = true;
  SetNodeFiltersString("*");
  SetNodeIgnoreFiltersString("");
  SetAttributeFiltersString("*");
  SetAttributeIgnoreFiltersString("");
  SetUserDefinedFiltersString("*");
  isPose = false;
  autokeyStore = true;
  autokeyApply = false;
  offsetTicks = 0;
  using_UDP_filters = false;
  storeNodePathAndID = false;
  onlyAttributeHolderDeformers = true;
  */
}

bool XClip::write(std::string filename) {
  // debugPrint(filename);
  userDataString = uData.buildString();
  file.open(filename.c_str(), std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    debugPrint("can't write XClip to file " + filename);
    return false;
  }
  // HEADER
  IO::write_STRING(file, header);
  IO::write_SINT(file, XCLIP_VERSION);

  // USERDATA
  IO::write_STRING(file, userDataString);

  // GLOBALS
  // IO::write_FLOAT(file, this->scale);

  // NODES
  nodeCount = nodes.size();
  IO::write_SINT(file, nodeCount);
  if (NodeState() == NodeDefined) {
    for (auto node : nodes) {
      node->write(file);
      for (auto track : node->tracks) {
        track.second->write(file);
        for (auto key : track.second->getKeys()) {
          key->write(file);
        }
      }
    }
  }

  file.close();
  return true;
}

bool XClip::read(std::string filename) {
  clear();
  file.open(filename.c_str(), std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    debugPrint("can't XClip read from file " + filename);
    return false;
  }

  // HEADER
  std::string fileHeader = IO::read_STRING(file);
  if (header != fileHeader)
    std::cout << "warning : this is not an XCLIP binary file !" << std::endl;
  u64 version = IO::read_SINT(file);
  if (version < XCLIP_VERSION)
    std::cout << "xclip file version " << version
              << " is older than plugin version " << XCLIP_VERSION << std::endl;
  if (version > XCLIP_VERSION)
    std::cout << "xclip file version " << version
              << " is newer than plugin version " << XCLIP_VERSION
              << ". please upgrade xclip plugin!" << std::endl;

  // USERDATA
  userDataString = IO::read_STRING(file);

  // GLOBALS
  // scale = IO::read_FLOAT(file);

  // update user data map
  // UserData uData;
  uData.buildMap(this->userDataString);
  // string clipModelName = uData.getVariable("modelName");
  // debugPrint("clipModelName:"+clipModelName);

  // NODES
  u64 nNodes = IO::read_SINT(file);
  // debugPrint(std::to_string(nNodes));
  int nodesFound = 0;
  while (!file.eof() && nodesFound < nNodes) {
    XNode* node = new XNode();
    node->readAll(file);
    this->addNode(node);
    nodesFound++;
  }
  file.close();
  return true;
}

XNode* XClip::addNode(std::string nodeName, std::string path, u64 id) {
  XNode* node = new XNode(nodeName);
  node->setPath(path);
  node->setId(id);
  this->nodes.push_back(node);
  this->_currentNode = node;
  this->_NodeState   = NodeDefined;
  return node;
}

XNode* XClip::addNode(XNode* node) {
  this->nodes.push_back(node);
  this->_currentNode = node;
  this->_NodeState   = NodeDefined;
  return node;
}

XNode* XClip::findNode(std::string name)
{
    for (auto node : nodes) {
        if (node->name == name)
            return node;
    }
    return nullptr;
}

void XClip::clear() {
  for (auto node : nodes) delete node;
  nodes.clear();
}

std::string XClip::dump() {
  std::stringstream str;
  str << "xclip header  " << header << std::endl;
  str << "xclip user data " << std::endl << userDataString << std::endl;
  for (auto node : nodes) {
    str << " xclip node " << node->name << std::endl;
    for (auto track : node->tracks) {
      str << "\t xclip track " << track.second->name << " value: " << track.second->value << std::endl;
      for (auto key : track.second->getKeys()) {
        // str << "xclip key " << key->TAG << std::endl;
        if (key->TAG.find("KBFL") != std::string::npos) {
          auto xKey = dynamic_cast<XBezierFloatKey*>(key);
          str << "\t\t xclip key XBezierFloatKey " << xKey->time << " "
              << xKey->value << " " << xKey->inTangent.x << " "
              << xKey->inTangent.y << " " << xKey->outTangent.x << " "
              << xKey->outTangent.y << " " << std::endl;
        } else if (key->TAG.find("KLFL") != std::string::npos) {
          auto xKey = dynamic_cast<XLinearFloatKey*>(key);
          str << "\t\t xclip key XLinearFloatKey " << xKey->time << " "
              << xKey->value << " " << std::endl;
        } else if (key->TAG.find("KSFL") != std::string::npos) {
          auto xKey = dynamic_cast<XStepFloatKey*>(key);
          str << "\t\t xclip key XStepFloatKey " << xKey->time << " "
              << xKey->value << " " << std::endl;
        } else {
          str << "\t\t xclip key unknown key type! " << key->TAG << " "
              << key->time << std::endl;
        }
      }
    }
  }
  return str.str();
}

void XClip::updateTimeRange() {
  for (auto node : nodes) {
    for (auto track : node->tracks) {
      for (auto key : track.second->getKeys()) {
        if (key->time < uData.getFloatVariable("startTime"))
          uData.setFloatVariable("startTime", key->time);
        if (key->time > uData.getFloatVariable("endTime"))
          uData.setFloatVariable("endTime", key->time);
      }
    }
  }
}
