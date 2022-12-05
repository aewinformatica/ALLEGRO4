-- The script must have 4 special functions:
---- Update()
-- This function update the logic of the game.

---- Draw()
-- The function Draw() just draw things in the buffer.
-- The script don't have direct acess to (BITMAP* screen).

---- Construct()
-- Is some thing like a startup. It initialize variables.
-- Some variables, like the font8x16, buffer and sprite are initialized just 1 time.
-- Others may initialize more times. eg x, y, tx, ty


---- Destruct()
-- destroy the resources.



------------------------------
-- must have in all scripts --
------------------------------
function globalize(lib)
	if type(_G[lib])=="table" then
		for key,value in pairs(_G[lib]) do
			_G[key]=value
		end
	end
end

-- mainly this 2 lines above
require("script_interface")
globalize("script_interface")

require("allegro")
globalize("allegro")

require("math")

-------------------------------------------------




sprite = load_bitmap("media/sprite.bmp")
buffer = GetActivePage();
font8x16 = load_font("media/8x16.pcx")


-- Update the logic of the game. Speed is controled in main.cpp.
function Update()
--     LeTecla <--> ReadKey
	if LeTecla(84) then
		y = y - 5
	end

	if LeTecla(85) then
		y = y + 5
	end

	if LeTecla(82) then
		x = x - 5
	end

	if LeTecla(83) then
		x = x + 5
	end

	tx = tx + 1
	ty = math.sin(tx/20)*190+240
	if tx == 690 then tx = -50 end
end


function Draw()
	Escreva(50, 100, "Primeira linha", "Segunda linha")

	local WHITE = makecol(192,192,192)
	textout_centre(buffer, font8x16, "Relogio", 320, 0, WHITE, 0)
	textout(buffer, font8x16, "AEWINFORMATICA:\\>_", 0, 0, WHITE, 0)

	circlefill(buffer, tx, ty, 50, makecol(0, 0, 255))
	circlefill(buffer, tx, ty, 30, makecol(255, 255, 255))
	circle(buffer, tx, ty, 30, 0)
	circlefill(buffer, tx, ty, 10, makecol(255, 0, 0))
	circle(buffer, tx, ty, 10, 0)

	draw_sprite(buffer, sprite, x, y)
	circle(buffer, x, y, 50, makecol(255, 255, 255))
end

function Construct()
	tx = 0
	ty = 0
	x = 0
	y = 0
end

function Destruct()
	collectgarbage("collect") -- I don't know if this is necessary.
end
