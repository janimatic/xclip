from pyxclip import pyxclip
import pprint
# import mathutils

# https://docs.blender.org/api/current/bpy.types.Keyframe.html
class blenderKeyframe():
    BEZIER = 1
    LINEAR = 2
    CONSTANT = 3
    co = [0.0, 0.0]
    handle_left = [0.0, 0.0]
    handle_right = [0.0, 0.0]
    interpolation = BEZIER
# https://docs.blender.org/api/current/bpy.types.FCurveKeyframePoints.html
class FCurveKeyframePoints():
    keyframes = []
    def insert(self, frame, value, options={}, keyframe_type='KEYFRAME'):
        kf = blenderKeyframe()
        kf.co = [frame, value]
        self.keyframes.append(kf)
        return kf
fcurves = {}

def trackToBlenderSpline(track):
    # no dynamic_cast in python/swig...
    # the simplest solution is to have complete data in base class
    # and just switch with the TAG...
    # or use the data member!
    fcurve = FCurveKeyframePoints()
    keys = track.getKeys()
    for k in keys:
        if(k.TAG == "KBFL"):
            k.data
            # bezierKey = (xclip.XBezierFloatKey)(k)
            # kf = blenderKeyframe
            # kf.co = [k.data[0], k.data[1]]
            # kf.handle_left = [k.data[2], k.data[3]]
            # kf.handle_right = [k.data[4], k.data[5]]
            kf = fcurve.insert(k.data[0], k.data[1])
            kf.handle_left = [k.data[2], k.data[3]]
            kf.handle_right = [k.data[4], k.data[5]]
    fcurves[track.name] = fcurve

def testCase(test):
    if(test == 0):
        # keys.xcl : simple example : write and read
        clip=pyxclip.XClip()
        root = clip.addNode("root")
        tx = root.addTrack("tx")
        key = pyxclip.XBezierFloatKey(0.0, 1.0, 0.1, 0.5, 0.1, 0.5)
        tx.addKey(key)
        tx.addKey([1.0, 1.0, 1.0, 0.1, 0.5, 0.1, 0.5])
        clip.updateTimeRange()
        filename = "keys.xcl"
        clip.write(filename)
        clip.read(filename)
        print(clip.dump())
        trackToBlenderSpline(tx)
        # pprint.pprint(fcurves)
        
    elif (test == 1):
        # spline.xcl : using some spline data
        clip=pyxclip.XClip()
        root = clip.addNode("root")
        tx = root.addTrack("tx")
        ty = root.addTrack("ty")
        tz = root.addTrack("tz")
        txCurve = {
        '0.0':[2.0, 12.666667, 2.0, 0, 0],
        '0.5':[0.0, 12.666667, 2.0, 0, 0],
        '1.0':[2.0, 12.666667, 2.0, 0, 0],
        '1.5':[2.0, 12.666667, 2.0, 0, 0],
        '2.0':[0.0, 12.666667, 2.0, 0, 0],
        '2.5':[2.0, 12.666667, 2.0, 0, 0],
        }
        tyCurve = {
        '0.0':[2.0],
        '0.5':[0.0],
        '1.0':[2.0],
        '1.5':[2.0],
        '2.0':[0.0],
        '2.5':[2.0],
        }
        tzCurve = {
        '0.0':[2.0],
        '0.5':[0.0],
        '1.0':[2.0],
        '1.5':[2.0],
        '2.0':[0.0],
        '2.5':[2.0],
        }
        tx.addKeys(pyxclip.keyMap(txCurve), pyxclip.BEZIER)
        ty.addKeys(pyxclip.keyMap(tyCurve), pyxclip.LINEAR)
        tz.addKeys(pyxclip.keyMap(tzCurve), pyxclip.STEP)
        clip.updateTimeRange()
        filename = "spline.xcl"
        clip.write(filename)
        clip.read(filename)
        print(clip.dump())

if __name__ == '__main__':
    testCase(0)
    testCase(1)
    