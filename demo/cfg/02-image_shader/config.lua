-- The default game configuration file.

window = Vec2Di.new (800, 600)

local assetPath = "../demo/assets/02-image_shader/"
path = {
	textureBg = assetPath .. "crate1.png",
	texture1 = assetPath .. "crate2.png",
	texture2 = assetPath .. "crate3.png",
	texture3 = assetPath .. "crowbar.png",
	hexShaderFrag = assetPath .. "hex.frag"
}

hex1_outerRadius = 100.0;
sprite3_rect = Rect2D.new (window.x, window.y, -256.0, -256.0)
