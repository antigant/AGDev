-- Functions
function SavePlayerPos(outputString, overwrite)
   print("SavePlayerPos...")
   local f;						-- The file
   if overwrite == 1 then		-- Wipe the contents with new data
      f = assert(io.open("Lua/Player.lua", "w"))
   elseif overwrite == 0 then	-- Append with new data
      f = assert(io.open("Lua/Player.lua", "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

-- Player input
moveForward  = "W"
moveBackward = "S"
moveLeft     = "A"
moveRight    = "D"
reload       = "R"
reset        = "I"
quit         = "0x1B"
jump         = "0x20"
look_left    = "0x25"
look_up      = "0x26"
look_right   = "0x27"
look_down    = "0x28"

-- Player's position
stage2_start = {0,0,20}
stage1_start = {-11.000000,0.000000,68.000000}
stage1_start = {-16.154190,0.000000,-3.427338}
stage1_start = {99.186211,63.836884,-121.183517}
stage1_start = {117.228546,0.000000,-146.202484}
stage1_start = {224.466675,0.000000,-52.997742}
