--[[
 This lua script uses the XClip C++ library interface generated by SWIG.
 * Windows instalation instructions :
 place luaxclip.dll and xclipioFusion.lua in 
 %ProgramData%\Blackmagic Design\DaVinci Resolve\Fusion\Modules\Lua
 place importXClip.lua and exportXClip.lua in 
 %AppData%\Blackmagic Design\DaVinci Resolve\Support\Fusion\Scripts\Utility
 
 * TODO
 - exportXClip
 - real precompositions : create a new Mutliplane fuse to handle 2d camera transformations ?

 
 * DONE
 - importXClip : import tracks keys and values
 - Bezier : 1 st key has no LH (left handle), last key has no RH (right handle)
 
 * SWIG lua/c++ related problems
 -Failed to use trackMap type (std::map<std::string, XTrack*> type)
 
 * Fusion api problems
 -Failed to use deleteKeyFrames()
 
 * REFERENCES
 - SetKeyFrames
 https://www.steakunderwater.com/wesuckless/viewtopic.php?p=45978&hilit=SetKeyFrames#p45978
 https://www.steakunderwater.com/wesuckless/viewtopic.php?p=45460&hilit=SetKeyFrames#p45460
 https://www.steakunderwater.com/wesuckless/viewtopic.php?p=44466&hilit=SetKeyFrames#p44466
 https://github.com/movalex/fusion/blob/main/Scripts/Tool/Import_Animation_From_Text_File.lua
 - fusion scripting api
 https://documents.blackmagicdesign.com/UserManuals/Fusion8_Generation_Scripting_Manual.pdf
 https://emberlightvfx.github.io/Fusion-Script-Docs/#/
 https://www.steakunderwater.com/VFXPedia/96.0.243.189/index8c76.html?title=Eyeon:Script/Reference/Applications/Fusion/Classes
 - lua language
 The colon is for implementing methods that pass self as the first parameter. So x:bar(3,4)should be the same as x.bar(x,3,4).
 https://stackoverflow.com/questions/4911186/difference-between-and-in-lua
 - Bezier.lua 
 https://gist.github.com/Be1zebub/a2d1383a6ba09a2c5d71888dbeedf284
 - fuses with arbitrary number of inputs 
 https://gitlab.com/WeSuckLess/Reactor/-/blob/master/Atoms/com.MuseVFX.MultiMerge/Fuses/Composite/MT_MultiMerge.fuse?ref_type=heads
 https://gitlab.com/WeSuckLess/Reactor/-/blob/master/Atoms/com.AndrewHazelden.ResolveEssentials/Resolve/Fuses/Fusion/Blackmagic/External%20Matte%20Saver.fuse?ref_type=heads
 https://gitlab.com/WeSuckLess/Reactor/-/blob/master/Atoms/com.AndrewHazelden.ParallelIO.pioSaver/Fuses/Kartaverse/Parallel%20IO/IO/pioSaver.fuse?ref_type=heads
 multi io
 https://www.steakunderwater.com/wesuckless/viewtopic.php?p=39895#p39895
 
]]--
print("Import XClip")

---- importing ----
if string.sub(_VERSION,1,7)=='Lua 5.0' then
	-- lua5.0 doesn't have a nice way to do this
	lib=loadlib('luaxclip.dll','luaopen_example') or loadlib('luaxclip.so','luaopen_example')
	assert(lib)()
else
	-- lua 5.1 does
	require('luaxclip')
end

-- choose the kind of message to print in the console...
-- debugLevel = "verbose info warning error"
debugLevel = "info warning error "
local function debugPrint(level, msg)
    if string.find(debugLevel, level) then
        print(level .. " : " .. msg)
    end
end

fusion = Fusion()
composition = fusion:GetCurrentComp()
preferences = composition:GetPrefs()
fps = preferences.Comp.FrameFormat.Rate
width = preferences.Comp.FrameFormat.Width
height = preferences.Comp.FrameFormat.Height

---- LUA UTILS ----

function wait(time)
    -- wait(seconds)
    local duration = os.time() + time
    while os.time() < duration do end
end

local function get_keys(t)
    -- create a table with the keys from another table
    -- usage : comp.CurrentTime = get_keys(splinedata)[1]
  local keys={}
  for key,_ in pairs(t) do
    table.insert(keys, key)
  end
  return keys
end

function tableContains(inTable, value)
  for i = 1,#inTable do
    if (inTable[i] == value) then
      return true
    end
  end
  return false
end

---- XCLIP IO ----
pointChannelsFilter = {"X", "Y", "Z"}

local function remapParameterName_FtoX(fusionName)
    -- assuming all tools use the same naming rules...
    local trackName = ""
    -- wildcard remapping (* means any characters, ? means any single character)
    if(fusionName == "Pivot.*") then
            trackName = "p?"
    elseif(fusionName == "Center.*") then
            trackName = "t?"
    elseif(fusionName == "Transform3DOp.Translate.*") then
            trackName = "t?"
    elseif(fusionName == "Transform3DOp.Rotate.*") then
            trackName = "r?"
    elseif(fusionName == "Transform3DOp.Scale.*") then
            trackName = "s?"
    elseif(fusionName == "Transform3DOp.Pivot.*") then
            trackName = "p?"
    -- strict remapping
    -- Merge...
    elseif(fusionName == "Angle") then
        trackName = "angle"
    elseif(fusionName == "Center.X") then
        trackName = "tx"
    elseif(fusionName == "Center.Y") then
        trackName = "ty"
    elseif(fusionName == "Size") then
        trackName = "scale"
    -- Transform...
    elseif(fusionName == "Angle") then
        trackName = "angle"
    elseif(fusionName == "Center.X") then
        trackName = "tx"
    elseif(fusionName == "Center.Y") then
        trackName = "ty"
    elseif(fusionName == "Pivot.X") then
        trackName = "px"
    elseif(fusionName == "Pivot.Y") then
        trackName = "py"
    elseif(fusionName == "Size") then
        trackName = "scale"
    -- Transform3Dop (Camera3D, ImagePlane3D, ...)
    elseif(fusionName == "Transform3DOp.Translate.X") then
        trackName = "tx"
    elseif(fusionName == "Transform3DOp.Translate.Y") then
        trackName = "ty"
    elseif(fusionName == "Transform3DOp.Translate.Z") then
        trackName = "tz"
    elseif(fusionName == "Transform3DOp.Rotate.X") then
        trackName = "rx"
    elseif(fusionName == "Transform3DOp.Rotate.Y") then
        trackName = "ry"
    elseif(fusionName == "Transform3DOp.Rotate.Z") then
        trackName = "rz"
    -- Camera3D
    elseif(fusionName == "AoV") then
        trackName = "fov"
    elseif(fusionName == "AovType") then
        trackName = "fovType"
    elseif(fusionName == "PerspNearClip") then
        trackName = "znear"
    elseif(fusionName == "PerspFarClip") then
        trackName = "zfar"
    end
    return trackName
end

-- http://lua-users.org/wiki/PointAndComplex
-- require 'class'
-- Point = class(function(pt,x,y,z)
   -- pt:set(x,y,z)
 -- end)

local function convertHCoordsXtoF(xPix)
    return xPix + 0.5
end

local function convertVCoordsXtoF(yPix)
    return yPix + 0.5
end

local function convertPointCoordsXtoF(xPix, yPix)
    local xNdc = convertHCoordsXtoF(xPix);
    local yNdc = convertVCoordsXtoF(yPix);
    return xNdc, yNdc
end

local function convertPointCoordsFtoX(xNdc, yNdc)
    return xNdc - 0.5, yNdc -0.5
end

local function convertKeysXtoF(keys)
    -- keyData array: 0:type 1:timeInSeconds 2:value 3:LH.x 4:LH.y 5:RH.x 6:RH.y
    -- fusion bezier spline : 1st key has no LH (left handle), last key has no RH (right handle)
    -- convert seconds to frames : apply fps to time value... (assuming startTime is 1)
    splinedata = {}
    for i = 0, keys.size(keys)-1 do
        debugPrint("verbose", keys[i].TAG)
        keyData = keys[i].data
        for i = 0, keyData.size(keyData)-1 do
            debugPrint("verbose", "keyData[" .. i .. "]: " ..  keyData[i])
        end
        local frame = (keyData[1] * fps) + 1.0;
        if(keys[i].TAG == "KBFL") then
            -- bezier
            if(i == 0) then
                splinedata[frame] = {
                    -- keyData[2],
                    keyData[2],
                    RH = { keyData[5], keyData[6] }
                }
            elseif(i == keys.size(keys)-1) then
                splinedata[frame] = {
                    keyData[2],
                    LH = { keyData[3], keyData[4] }
                }
            else
                splinedata[frame] = {
                    keyData[2],
                    LH = { keyData[3], keyData[4] },
                    RH = { keyData[5], keyData[6] }
                }
            end
        elseif(keys[i].TAG == "KLFL") then
            -- linear
            splinedata[frame] = {
                keyData[2],
                LH = { 0.0, 0.0 },
                RH = { 0.0, 0.0 }
            }
        elseif(keys[i].TAG == "KSFL") then
            -- step
            splinedata[frame] = {
                keyData[2],
                LH = { 0.0, 0.0 },
                RH = { 0, 0.0 }
            }
        else
            debugPrint("error", "unsupported keyframe type with TAG: " .. keys[i].TAG)
        end
    end
    return splinedata
end

local function importXKeys(node, track, tool, toolInput)
    local keys = track:getKeys()
    local inputAttrs = toolInput:GetAttrs()
    local inputName = inputAttrs.INPS_Name
    if(keys:size() == 0) then
        debugPrint("info", "Track has no keyframes to copy on " .. tool.Name .. "." .. inputName)
        return
    end
    debugPrint("info", "importXKeys " .. node.name .. "." .. track.name .. " keyframes to " .. tool.Name .. "." .. inputName)
    splinedata = convertKeysXtoF(keys)
    if(track.name == "tx") then
        for k, v in pairs(splinedata) do
            v[1] = convertHCoordsXtoF(v[1])
        end
    elseif(track.name == "ty") then
        for k, v in pairs(splinedata) do
            v[1] = convertVCoordsXtoF(v[1])
        end
    end
    -- WHY should i call AddModifier TWICE to get DeleteKeyFrames to remove the keyframe created by this AddModifier ?
    -- hack : go to first keyframe's time before calling AddModifier, because DeleteKeyFrames isn't reliable !
    comp.CurrentTime = get_keys(splinedata)[1]
    tool:AddModifier(toolInput.ID, "BezierSpline")
    -- delete the keyframe created by AddModifier method
    -- toolInput[comp:GetAttrs().COMPN_CurrentTime] = nil -- not working
    splineout = toolInput:GetConnectedOutput()
    if(splineout ~= nil) then
        spline = splineout:GetTool() 
        -- print("delete keyframe created by AddModifier on frame " .. tostring(composition:GetAttrs().COMPN_CurrentTime))
        -- spline:DeleteKeyFrames(composition:GetAttrs().COMPN_CurrentTime) -- not working
        spline:SetKeyFrames(splinedata, true)
    else
        debugPrint("error", "Failed to retreive the spline from " .. tool.Name .. "." .. inputName)
    end
end

local function importXTrack(node, tool, toolInput, prefix)
    local inputAttrs = toolInput:GetAttrs()
    local inputName = inputAttrs.INPS_Name
    if prefix ~= nil then
        debugPrint("verbose", "importXTrack(node:" .. node.name .. ", tool:" .. tool.Name .. ", input:" .. inputName .. ", prefix:" .. prefix .. ")")
        inputName = prefix .. inputName
    else
        debugPrint("verbose", "importXTrack(node:" .. node.name .. ", tool:" .. tool.Name .. ", input:" .. inputName .. ")")
    end
    local keys = nil
    
    if(inputAttrs.INPS_DataType == "Number") then
        local trackName = remapParameterName_FtoX(inputName)
        if(node.hasTrack(node, trackName)) then
            debugPrint("info", "importXTrack INPS_DataType Number : " .. inputName)
            local track = node:getTrack(trackName)
            if(track:getKeys():size() > 0) then
                importXKeys(node, track, tool, toolInput) -- import track's keys
            else
                -- import track's value (normalize tx & ty)
                if(track.name == "tx") then
                    tool:SetInput(toolInput.ID, convertHCoordsXtoF(track.value)) 
                elseif(track.name == "tx") then
                    tool:SetInput(toolInput.ID, convertVCoordsXtoF(track.value)) 
                else
                    tool:SetInput(toolInput.ID, track.value) 
                end
            end
        else
            debugPrint("verbose", "Track not found INPS_DataType Number : " .. inputName)
        end
    elseif(inputAttrs.INPS_DataType == "Point") then
        debugPrint("info", "importXTrack INPS_DataType Point : " .. inputName)
        -- check if track exists, if track has keys AddModifier XYPath, else SetInput
        -- we need to use widcard search to determine if grouped tracks has keys
        trackNameWildcard = remapParameterName_FtoX(inputName  .. ".*")
        local withKeys = true
        local splineout = nil
        if(node:hasTrack(trackNameWildcard , withKeys)) then
            -- hack : go to first keyframe's time because DeleteKeyFrames isn't reliable !
            local startTime = node:getTracksStartTime(trackNameWildcard , withKeys)
            local frame = (startTime * fps) + 1.0;
            comp.CurrentTime = frame
            tool:AddModifier(toolInput.ID, "XYPath")
            -- delete the keyframe created by AddModifier method
            -- toolInput[comp:GetAttrs().COMPN_CurrentTime] = {nil,nil} -- not working...
            splineout = toolInput:GetConnectedOutput()
            if(splineout ~= nil) then
                spline = splineout:GetTool() 
                debugPrint("verbose", "XYPath GetConnectedOutput: " .. spline.Name)
                splineInputs = spline:GetInputList() -- yeah! we could recurse and build a path with this...
                for k, splineInput in pairs(splineInputs) do
                    debugPrint("verbose", spline.Name .. " XYPath splineInput: " .. splineInput.Name)
                    -- INP_External boolean : Whether this input can be animated or connected to a tool or modifier.
                    -- if(splineInput:GetAttrs().INP_External == true ) then 
                    -- inputs with INP_External contains a lot of junk ex: Comment, Start Render Scripts, etc ..
                    -- use a more restrictive filter :
                    if(tableContains(pointChannelsFilter, splineInput.Name) ) then 
                        local path = inputName .. "."
                        debugPrint("info", "attempt to importXTrack INPS_DataType Point with a custom path : " .. inputName .. "." .. splineInput.Name)
                        importXTrack(node, tool, splineInput, path) -- recursion with "Number" INPS_DataType
                    else
                        debugPrint("verbose", "splineInput.Name not pointChannelsFilter : " .. inputName .. "." .. splineInput.Name)
                    end
                end
            else
                print ("Failed to retrieve the spline from " .. tool.Name .. "." .. inputName)
            end
        elseif(node:hasTrack(trackNameWildcard)) then
            -- tracks found but with no keys
            debugPrint("warning", "Track has no keys : applying track values from " .. tool.Name .. "." .. inputName .. ".*")
            local valueTable = {}
            for k, channel in pairs(pointChannelsFilter) do
                local channelPath = inputName .. "." .. channel
                local trackName = remapParameterName_FtoX(channelPath)
                if(node.hasTrack(node, trackName)) then
                    local track = node:getTrack(trackName)
                    table.insert(valueTable, track.value)
                else
                    debugPrint("verbose", "XYPath Track not found: " .. trackName)
                end
            end
            if(#valueTable >= 2) then
                -- sets each Point's channel
                -- tool:SetInput(toolInput.ID, {valueTable[1], valueTable[2]} )
                tool:SetInput(toolInput.ID, convertPointCoordsXtoF(valueTable[1],valueTable[2] ) )
            end
        else
            debugPrint("warning", "No track matches trackNameWildcard " .. trackNameWildcard)
        end
    end
end

local function importXNode(clip, tool)
    local node = clip:findNode(tool.Name)
    if( node ~= nil) then
        debugPrint("info", "importXNode: " .. node.name)
        toolAttrs = tool:GetAttrs()
        toolInputs = tool:GetInputList()
        for k, toolInput in pairs(toolInputs) do
            importXTrack(node, tool, toolInput)
        end
    end
end

function importXClip(filename)
    composition:StartUndo("importXClip")
    local currentFrame = comp.CurrentTime
    local clip = luaxclip.XClip()
    clip:read(filename)
    print (clip:dump())
    tools = composition:GetToolList(false)
    if table.getn(tools) > 0 then
        comp.Lock()
        for k, tool in pairs(tools) do
            if(tool ~= nil) then
                importXNode(clip, tool)
            end
        end
        comp.Unlock()
    end
    comp.CurrentTime = currentFrame
    composition:EndUndo(true)
    clip=nil
    collectgarbage()
end

function exportXClip(filename)
    print("exportXClip is not yet implemented...")
end

---- usage: ----
-- require "xclipioFusion"
-- ret = composition:AskUser("Select an XClip file (.xcl)", { {"XClip", "FileBrowse"} } )
-- importXClip(ret.XClip)


-- USEFULL SCRIPTS
-- https://www.steakunderwater.com/VFXPedia/96.0.243.189/indexf36d.html?title=Useful_Scripts/Snippets

local function listInputs()
    for _, inp in ipairs(fusion:GetCurrentComp().ActiveTool:GetInputList()) do 
        print(inp:GetAttrs().INPS_ID .. ": " .. inp:GetAttrs().INPS_Name) 
    end
end
local function listOutputs()
    for _, out  in ipairs(fusion:GetCurrentComp().ActiveTool:GetOutputList()) do 
        print(out :GetAttrs().OUTS_ID  .. ": " .. out :GetAttrs().OUTS_Name) 
    end
end
local function listOpIconStrings()
    for i,v in ipairs(fu:GetRegSummary(CT_Tool)) do 
        if v.REGS_OpIconString then 
            print(v.REGS_OpIconString .." = ".. v.REGS_Name) 
        end 
    end
end
local function listKeyframes()
    for key,inp in pairs(activeTool:GetInputList()) do
        if inp:GetConnectedOutput() then
            spline = inp:GetConnectedOutput():GetTool()
            keyframes = spline:GetKeyFrames(true)
            dump(keyframes[0])  -- >0
            dump(keyframes[0].Flags) -- >nil
            dump(keyframes[0].Points) -- >nil
        end
    end
end
local function exampleSpline()
    local splinedata = {}
    splinedata[1] = {
        1,
        LH = { 1, 1 },
        RH = { 1, 1 }
    }
    return splinedata
end
