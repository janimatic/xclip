#include <iostream>
#include <string>
#include "xclipConfig.h"
#include "xclip/xclip.h"
#include "tests/tahoma2dKeyframes.h"


// TODO : define the best data set for animation tools...
// animation name : node.track
// float data : one array + dimension or a collection of array ?
// key type or track type ?
static std::map<std::string, std::vector<float>> sceneAnimations;
//bool importTrack(std::string nodeName, std::string trackName, std::map<std::string, std::vector<float>> const& keys) {
bool importTahomaTrack(std::string nodeName, std::string trackName, std::vector<XKey*> const& keys) {
    std::vector<TDoubleKeyframe> tahomaKeyframes;
    for (auto key : keys) {
        //sceneAnimations[nodeName + "." + trackName].push_back(key.second.);
        auto t2dkf = TDoubleKeyframe();
        t2dkf.m_frame = (key->time * tahomaFps) + 1;
        if (key->TAG.find("KBFL") != std::string::npos) {
            t2dkf.m_type = TDoubleKeyframe::Type::SpeedInOut;
            auto xKey = dynamic_cast<XBezierFloatKey*>(key);
            t2dkf.m_value = xKey->value;
            // if tangent x are 0 it could be easeInOut or not...
            // if tangent x,y are 0 it could be constant or not...
            // just keep the full bezier keyframe...
            t2dkf.m_speedIn.x = xKey->inTangent.x;
            t2dkf.m_speedIn.y = xKey->inTangent.y;
            t2dkf.m_speedOut.x = xKey->outTangent.x;
            t2dkf.m_speedOut.y = xKey->outTangent.y;
        }
        else if (key->TAG.find("KLFL") != std::string::npos) {
            t2dkf.m_type = TDoubleKeyframe::Type::Linear;
            auto xKey = dynamic_cast<XLinearFloatKey*>(key);
            t2dkf.m_value = xKey->value;
        }
        else if (key->TAG.find("KSFL") != std::string::npos) {
            t2dkf.m_type = TDoubleKeyframe::Type::Constant;
            auto xKey = dynamic_cast<XStepFloatKey*>(key);
            t2dkf.m_value = xKey->value;
        }
        else {
            debugPrint(" unsupported keyframe type: " + key->TAG + " skipping...");
            continue;
        }
        //// or just use float data..
        //t2dkf.m_frame = (key->data[0] * tahomaFps) + 1;
        //t2dkf.m_value = key->data[1];
        //if (t2dkf.m_type == TDoubleKeyframe::Type::SpeedInOut) {
        //    t2dkf.m_speedIn = TPointD(key->data[2], key->data[3]);
        //    t2dkf.m_speedOut = TPointD(key->data[4], key->data[5]);
        //}

        tahomaKeyframes.push_back(t2dkf);
    }
    addKeyframes(tahomaKeyframes);
}

static void testCase(int test) {
    XClip xclip;
    XNode* root;
    XTrack* tx;
    XTrack* ty;
    XTrack* tz;
    std::string filename;
    std::vector<double> bezierKeyData = { 1.0, 0.0, 1.0, 0.1, 0.5, 0.1, 0.5 };
    // some bezier data
    std::map<std::string, std::vector<double>> txCurve =
    {
        {"0.0", { 2.0, 12.666667, 2.0, 0, 0}},
        {"0.5", {0.0, 12.666667, 2.0, 0, 0}},
        {"1.0", { 2.0, 12.666667, 2.0, 0, 0}},
        {"1.5", { 2.0, 12.666667, 2.0, 0, 0}},
        {"2.0", { 0.0, 12.666667, 2.0, 0, 0}},
        {"2.5", { 2.0, 12.666667, 2.0, 0, 0}},
    };
    // some linear data
    std::map<std::string, std::vector<double>> tyCurve =
    {
        {"0.0", { 2.0}},
        {"0.5", { 0.0}},
        {"1.0", { 2.0}},
        {"1.5", { 2.0}},
        {"2.0", { 0.0}},
        {"2.5", { 2.0}},
    };
    // some step data
    std::map<std::string, std::vector<double>> tzCurve =
    {
        {"0.0", { 2.0}},
        {"0.5", { 0.0}},
        {"1.0", { 2.0}},
        {"1.5", { 2.0}},
        {"2.0", { 0.0}},
        {"2.5", { 2.0}},
    };
    std::string wild;
    std::string line;
    bool result;
    switch (test)
    {
    case 0:
        std::cout << std::endl << "XClip testCase " << test << " simple example : write and read" << std::endl;
        // keys.xcl : simple example : write and read
        xclip = XClip();
        root = xclip.addNode("root");
        tx = root->addTrack("tx");
        tx->addKey(new XBezierFloatKey(0.0, 1.0, 0.1, 0.5, 0.1, 0.5));
        tx->addKey(bezierKeyData);
        xclip.updateTimeRange();
        filename = "keys.xcl";
        xclip.write(filename);
        xclip.read(filename);
        std::cout << xclip.dump();;
        break;
    case 1:
        std::cout << std::endl << "XClip testCase " << test << " using some spline data" << std::endl;
        // spline.xcl : using some spline data
        xclip = XClip();
        root = xclip.addNode("root");
        tx = root->addTrack("tx");
        ty = root->addTrack("ty");
        tz = root->addTrack("tz");
        tx->addKeys(txCurve, Interpolation::BEZIER);
        ty->addKeys(tyCurve, Interpolation::LINEAR);
        tz->addKeys(tzCurve, Interpolation::STEP);
        xclip.updateTimeRange();
        filename = "spline.xcl";

        std::cout << "write dump:" << std::endl;
        xclip.write(filename);
        std::cout << xclip.dump();

        std::cout << "read dump:" << std::endl;
        xclip.read(filename);
        std::cout << xclip.dump();
        break;
    case 2:
        std::cout << std::endl << "XClip testCase " << test << " reading back spline data in tahoma" << std::endl;
        // spline.xcl : reading back spline data
        xclip = XClip();
        filename = "spline.xcl";
        xclip.read(filename);
        for (auto node : xclip.nodes) {
            for (auto track : node->tracks) {
                importTahomaTrack(node->name, track.first, track.second->getKeys());
                //importTrack(node->name, track.first, track.second->getKeysMap());
            }
        }
        break;
    case 3:
        std::cout << std::endl << "XClip testCase " << test << " string wildcard matching using * (any characters) and ? (any single character)" << std::endl;
        wild = "Center.*";
        line = "Center.X";
        result = string_utils::wildmatch(wild.c_str(), line.c_str());
        std::cout << "wildmatch " << wild << " " << line << " : " << result << std::endl;
        line = "Center_X";
        result = string_utils::wildmatch(wild.c_str(), line.c_str());
        std::cout << "wildmatch " << wild << " " << line << " : " << result << std::endl;
        wild = "Center.?";
        result = string_utils::wildmatch(wild.c_str(), line.c_str());
        std::cout << "wildmatch " << wild << " " << line << " : " << result << std::endl;
        line = "Center.X";
        result = string_utils::wildmatch(wild.c_str(), line.c_str());
        std::cout << "wildmatch " << wild << " " << line << " : " << result << std::endl;
        wild = "*nter.*";
        result = string_utils::wildmatch(wild.c_str(), line.c_str());
        std::cout << "wildmatch " << wild << " " << line << " : " << result << std::endl;
        break;
    default:
        break;
    }
}
#define TESTCASE 1
int main(int argc, char* argv[])
{
    std::cout << argv[0] << " Version " << xclip_VERSION_MAJOR << "." << xclip_VERSION_MINOR << std::endl;
    testCase(0);
    testCase(1);
    testCase(2);
    testCase(3);
    return 0;
}
