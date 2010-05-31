require "png"

image = PNG.Open("test/smile.png")

print("width = " .. image.width)
print("height = " .. image.height)
if image.format == PNG.RGB then
	print("format = RGB")
else
	print("format = RGBA")
end
