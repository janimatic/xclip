#pragma once

#ifndef _XCLIP_H_
#define _XCLIP_H_
#include "xnode.h"
#include "xuserdata.h"

#define XCLIP_VERSION 100
#define XCLIP_HEADER "XCLIP BINARY VERSION";
class XNode;

class XClip {
protected:
  // IO DATA
  std::string header;
  std::fstream file;
  std::string userDataString;

  // TODO : move in XUserData ?
  // f64 scale;
  // f64 startFrame;
  // f64 endFrame;

  u64 nodeCount;
  int _NodeState;
  enum NodeState { NodeDefined, NodeUndefined };
  inline int NodeState() { return _NodeState; }
  XNode* _currentNode;

  // map storing user data associative string array (model name, etc...)
  XUserData uData;

  /*
  // deprecate ?
  std::string modelName;
  std::string nodeFiltersString; // "*character1*;*_L_HAND_*;*_R_HAND_*";
  std::string nodeIgnoreFiltersString;
  std::string nodeSearchString;
  std::string nodeReplaceString;
  std::vector<std::string> nodeFilters;
  std::vector<std::string> nodeIgnoreFilters;
  std::vector<std::string> nodeSearch;
  std::vector<std::string> nodeReplace;

  std::string attributeFiltersString;
  std::string attributeIgnoreFiltersString;
  std::string attributeSearchString;
  std::string attributeReplaceString;
  std::vector<std::string> attributeFilters;
  std::vector<std::string> attributeIgnoreFilters;
  std::vector<std::string> attributeSearch;
  std::vector<std::string> attributeReplace;

  std::string userDefinedFiltersString;
  std::string userDefinedSearchString;
  std::string userDefinedReplaceString;
  std::vector<std::string> userDefinedFilters;
  std::vector<std::string> userDefinedSearch;
  std::vector<std::string> userDefinedReplace;

  bool using_UDP_filters;
  bool isPose;
  bool autokeyStore;
  bool autokeyApply;
  int offsetTicks;
  bool searchByName;
  bool storeNodePathAndID;
  bool onlyAttributeHolderDeformers;
  */

public:
  std::vector<XNode*> nodes;

  XClip();
  bool write(std::string filename);
  bool read(std::string filename);
  XNode* addNode(std::string nodeName, std::string path = "",
                 u64 id = -1);
  XNode* addNode(XNode* node);
  XNode* findNode(std::string name);
  void clear();
  std::string dump();
  XUserData* getUserData() { return &uData; }
  void updateTimeRange();
  /*
  inline void SetModelName(std::string _ModelName) {
          modelName = _ModelName;
          uData.setVariable("modelName", modelName);
  }
  inline void SetNodeFiltersString(std::string _NodeFiltersString) {
          nodeFilters.clear();
          nodeFiltersString = _NodeFiltersString;
          string_utils::tokenize(nodeFiltersString, nodeFilters, ";");
  }
  inline void SetNodeIgnoreFiltersString(std::string _NodeIgnoreFiltersString) {
          nodeIgnoreFilters.clear();
          nodeIgnoreFiltersString = _NodeIgnoreFiltersString;
          string_utils::tokenize(nodeIgnoreFiltersString, nodeIgnoreFilters,
  ";");
  }
  inline void SetNodeSearchString(std::string _NodeSearchString) {
          debugPrint(_NodeSearchString);
          nodeSearch.clear();
          nodeSearchString = _NodeSearchString;
          string_utils::tokenize(nodeSearchString, nodeSearch, ";");
          for (int i = 0; i < nodeSearch.size(); i++)
          {
                  debugPrint(nodeSearch.at(i));
          }
  }
  inline void SetNodeReplaceString(std::string _NodeReplaceString) {
          debugPrint(_NodeReplaceString);
          nodeReplace.clear();
          nodeReplaceString = _NodeReplaceString;
          string_utils::tokenize(nodeReplaceString, nodeReplace, ";");
          for (int i = 0; i < nodeReplace.size(); i++)
          {
                  debugPrint(nodeReplace.at(i));
          }
  }
  inline void SetAttributeFiltersString(std::string _AttributeFiltersString) {
          attributeFilters.clear();
          attributeFiltersString = _AttributeFiltersString;
          string_utils::tokenize(attributeFiltersString, attributeFilters, ";");
  }
  inline void SetAttributeIgnoreFiltersString(std::string
  _AttributeIgnoreFiltersString) { attributeIgnoreFilters.clear();
          attributeIgnoreFiltersString = _AttributeIgnoreFiltersString;
          string_utils::tokenize(attributeIgnoreFiltersString,
  attributeIgnoreFilters, ";");
  }
  inline void SetAttributeSearchString(std::string _AttributeSearchString) {
          attributeSearch.clear();
          attributeSearchString = _AttributeSearchString;
          string_utils::tokenize(attributeSearchString, attributeSearch, ";");
  }
  inline void SetAttributeReplaceString(std::string _AttributeReplaceString) {
          attributeReplace.clear();
          attributeReplaceString = _AttributeReplaceString;
          string_utils::tokenize(attributeReplaceString, attributeReplace, ";");
  }
  inline void SetUserDefinedFiltersString(std::string _userDefinedFiltersString)
  { userDefinedFilters.clear(); userDefinedFiltersString =
  _userDefinedFiltersString; string_utils::tokenize(userDefinedFiltersString,
  userDefinedFilters, ";");

          //if default filter : do not use it
          this->using_UDP_filters = true;
          if (this->userDefinedFilters.size() == 0)
                  using_UDP_filters = false;
          if (this->userDefinedFilters.size() == 1)
                  if (userDefinedFilters.at(0) == "*" ||
  userDefinedFilters.at(0).length() == 0) using_UDP_filters = false; if
  (using_UDP_filters) debugPrint("using_UDP_filters"); for (int i = 0; i <
  userDefinedFilters.size(); i++)
          {
                  debugPrint(userDefinedFilters.at(i));
          }
  }
  inline void SetUserDefinedSearchString(std::string _userDefinedSearchString) {
          userDefinedSearch.clear();
          userDefinedSearchString = _userDefinedSearchString;
          string_utils::tokenize(userDefinedSearchString, userDefinedSearch,
  ";");
  }
  inline void SetUserDefinedReplaceString(std::string _userDefinedReplaceString)
  { userDefinedReplace.clear(); userDefinedReplaceString =
  _userDefinedReplaceString; string_utils::tokenize(userDefinedReplaceString,
  userDefinedReplace, ";");
  }

  inline void SetStartFrame(f64 _StartFrame) {
          startFrame = _StartFrame;
          uData.setFloatVariable("startFrame", startFrame);
  }
  inline void SetEndFrame(f64 _EndFrame) {
          endFrame = _EndFrame;
          uData.setFloatVariable("endFrame", endFrame);
  }

  inline bool filterMatch(std::string elementName, std::vector <std::string>
  vec, std::string prefix = "") { for (int i = 0; i < vec.size(); i++) {
                  //string filter = vec.at(i);
                  std::string filter = prefix + vec.at(i);
                  if (string_utils::wildmatch(filter.c_str(),
  elementName.c_str())) return true;
          }
          debugPrint("no match found for :" + elementName + " prefix:" +
  prefix); return false;
  }
  */
};

#endif
