--[[
/*===========================================================================*
 * Arch Engine - "Testers/guiControls.lua"                                   *
 *                                                                           *
 * This is a tester script. It's purpose is to test some functionalities,    *
 * thus should be removed from releases.                                     *
 * Expect some hard coded, bizarre stuff. You've been warned ;)              *
 *                                                                           *
 * Marcelo de Matos Menezes - marcelodmmenezes@gmail.com                     *
 * Created: 24/06/2018                                                       *
 * Last Modified: 24/06/2018                                                 *
 *===========================================================================*/
]]


renderableComponent1 = {
	id = "sidebar",
	shader_id = 7,
	limits = { 0.0, 0.0, 205.0, 600.0 },
	texture = "",
	color = { 0.22, 0.22, 0.24, 1.0 },
	hover_color = { 0.5, 0.8, 0.5, 1.0 },
	border_color = { 0.3, 0.6, 0.3, 1.0 },
	border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

writableComponent1 = {
	id = "fpscounter",
	position = { 50.0, 50.0 },
	font_id = 1,
	font_scale = 1.0,
	text_spacing = 15,
	color = { 1.0, 1.0, 0.0 },
	hover_color = { 1.0, 1.0, 1.0 },
	maximum_size = { 165.0, 32.0 },
	track_mouse = true,
}

pushButton1 = {
	id = "xablau",
	button_id = 0,
	shader_id = 8,
	limits = { 20.0, 500.0, 184.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU1",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.5, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

pushButton2 = {
	id = "xablau2",
	button_id = 0,
	shader_id = 8,
	limits = { 20.0, 459.0, 184.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU2",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.5, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

pushButton3 = {
	id = "xablau3",
	button_id = 0,
	shader_id = 8,
	limits = { 20.0, 418.0, 184.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU3",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.5, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

pushButton4 = {
	id = "xablau4",
	button_id = 0,
	shader_id = 8,
	limits = { 20.0, 377.0, 184.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU4",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.5, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

pushButton5 = {
	id = "xablau5",
	button_id = 0,
	shader_id = 8,
	limits = { 20.0, 338.0, 184.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU5",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.5, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}