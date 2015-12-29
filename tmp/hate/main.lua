function hate.load()
	-- print(hate.filesystem.exists("conf.lua"))
	hate.filesystem.getDirectoryItems("/", function(f)
		print(f)
	end)
	print(hate.filesystem.read("main.lua"))
	print(hate.filesystem.getSize("main.lua"))
	print(hate.filesystem.getSaveDirectory())
	print(hate.filesystem.getWorkingDirectory())
	hate.filesystem.write("test", "cocks\n")
	hate.graphics.setBackgroundColor(255, 255, 0, 255)
end

function hate.keypressed(key, isrepeat)
	print(key)
	if key == "escape" then
		hate.event.quit()
	end
end

function hate.keyreleased(key)
	print(key)
end

function hate.update(dt)
	-- do some shit
end

function hate.draw()
	-- do some shit
end
