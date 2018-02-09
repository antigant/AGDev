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
moveForward  = "Q"
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
stage1_start = {-224.277328,0.000000,-12.802730}
