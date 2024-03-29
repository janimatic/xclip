-- file: runme.lua
-- This file illustrates XClip C++ library lua interface generated by SWIG.

---- importing ----
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	-- lua5.0 doesn't have a nice way to do this
	lib=loadlib('luaxclip.dll','luaopen_example') or loadlib('luaxclip.so','luaopen_example')
	assert(lib)()
else
	-- lua 5.1 does
	require('luaxclip')
end

local function trackToFusionSpline(track)
    local keys = track.getKeys(track)
    local splinedata = {}
    for i = 0, keys.size(keys)-1 do
        print (keys[i].TAG)
        --[[
        -- typecasting not working
        if(keys[i].TAG == "KBFL") then
            xkey = xclip.XBezierFloatKey :: keys[i]
            splinedata[keys[i].time] = {
                keys[i].val,
                LH = { keys[i].inTangent.x, keys[i].inTangent.y },
                RH = { keys[i].outTangent.x, keys[i].outTangent.y }
            }
        elseif(keys[i].interpolation == "KLFL") then
            splinedata[keys[i].time] = {
                keys[i].val,
                LH = { 0, 0 },
                RH = { 0, 0 }
            }
        elseif(keys[i].interpolation == "KSFL") then
            splinedata[keys[i].time] = {
                keys[i].val,
                LH = { 0, 0 },
                RH = { 0, 0 }
            }
        else
            print ("unsupported keyframe type: " .. tostring(keys[i].interpolation) .. " with TAG: " .. keys[i].TAG)
        end
        ]]--
        keyData = keys[i].data
        if(keys[i].TAG == "KBFL") then
            splinedata[keyData[1]] = {
                keyData[2],
                LH = { keyData[3], keyData[4] },
                RH = { keyData[5], keyData[6] }
            }
        elseif(keyData.interpolation == "KLFL") then
            splinedata[keyData[1]] = {
                keyData[2],
                LH = { 0, 0 },
                RH = { 0, 0 }
            }
        elseif(keyData.interpolation == "KSFL") then
            splinedata[keyData[1]] = {
                keyData[2],
                LH = { 0, 0 },
                RH = { 0, 0 }
            }
        else
            print ("unsupported keyframe type: " .. tostring(keys[i].interpolation) .. " with TAG: " .. keys[i].TAG)
        end
    end
    return splinedata
    -- local spline = BezierSpline()
    -- spline:SetKeyFrames(splinedata)
end

local function testCase(test)
----- Object creation -----
    if test == 0 then
        print("Creating xclip:")
        clip = luaxclip.XClip()
        root = clip.addNode(clip, "root")
        tx = root.addTrack(root, "tx")
        -- ty = root.addTrack(root, "ty")
        -- tz = root.addTrack(root, "tz")
        key = luaxclip.XBezierFloatKey(0.0, 1.0, 0.1, 0.5, 0.1, 0.5)
        tx.addKey(tx, key)
        -- tx.addKey(1.0, {1.0, 0.1, 0.5, 0.1, 0.5}, luaxclip.BEZIER)
        print (clip.dump(clip))
        print (trackToFusionSpline(tx))
        -- print(keys)
        
    elseif test == 1 then
        -- use a simple vector of doubles...
        -- type time value LX LY RX RY
        print("Creating xclip:")
        clip = luaxclip.XClip()
        root = clip.addNode(clip, "root")
        tx = root.addTrack(root, "tx")
        local txCurve = {0.0, 2.0, 12.666667, 2.0, 0, 0}
        txCurveV = luaxclip.floatVector(6)
        txCurveV.push_back(txCurveV, 1.0)
        txCurveV.push_back(txCurveV, 0.0)
        txCurveV.push_back(txCurveV, 2.0)
        txCurveV.push_back(txCurveV, 12.0)
        txCurveV.push_back(txCurveV, 2.0)
        txCurveV.push_back(txCurveV, 0.0)
        txCurveV.push_back(txCurveV, 0.0)
        
        txCurveV.push_back(txCurveV, 1.0)
        txCurveV.push_back(txCurveV, 1.0)
        txCurveV.push_back(txCurveV, 2.0)
        txCurveV.push_back(txCurveV, 12.0)
        txCurveV.push_back(txCurveV, 2.0)
        txCurveV.push_back(txCurveV, 0.0)
        txCurveV.push_back(txCurveV, 0.0)
        tx.addKeysFromTable(tx, txCurveV, luaxclip.BEZIER)
        print (clip.dump(clip))
    end
end

testCase(0)
testCase(1)

-- Note: this invokes the virtual destructor
clip=nil
collectgarbage()
print "Goodbye"
