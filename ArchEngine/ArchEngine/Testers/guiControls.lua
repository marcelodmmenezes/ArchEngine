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


component1 = {
	tp = "renderableComponent",
	id = "sidebar",
	shader_id = 8,
	limits = { 0.0, 0.0, 200.0, 540.0 },
	texture = "",
	color = { 0.22, 0.22, 0.24, 1.0 },
	hover_color = { 0.3, 0.6, 0.3, 1.0 },
	border_color = { 0.3, 0.6, 0.3, 1.0 },
	border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	border_constraint = { 1, 0, 1, 1 },
	track_mouse = true,
}

component2 = {
	tp = "renderableComponent",
	id = "topbar",
	shader_id = 8,
	limits = { 0.0, 540.0, 800, 60.0 },
	texture = "",
	color = { 0.22, 0.22, 0.24, 1.0 },
	--hover_color = { 0.5, 0.8, 0.5, 1.0 },
	--border_color = { 0.3, 0.6, 0.3, 1.0 },
	--border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	border_constraint = { 1, 1, 0, 0 },
	track_mouse = true,
}

component3 = {
	tp = "writableComponent",
	id = "logo",
	text = "Arch Editor",
	position = { 25.0, 600.0 },
	font_id = 0,
	font_scale = 0.5,
	text_spacing = 15,
	color = { 1.0, 1.0, 1.0 },
	--hover_color = { 1.0, 1.0, 1.0 },
	maximum_size = { 600.0, 50.0 },
	track_mouse = true,
}

component4 = {
	tp = "writableComponent",
	id = "fpscounter",
	position = { 30.0, 70.0 },
	font_id = 1,
	font_scale = 1.2,
	text_spacing = 20,
	color = { 1.0, 1.0, 1.0 },
	hover_color = { 1.0, 1.0, 1.0 },
	maximum_size = { 185.0, 100.0 },
	anchored = true,
	track_mouse = true,
}

component5 = {
	tp = "pushButton",
	id = "xablau",
	button_id = 0,
	shader_id = 9,
	limits = { 5.0, 500.0, 190.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "Debug Draw",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.8, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

component6 = {
	tp = "pushButton",
	button_id = 1,
	shader_id = 9,
	limits = { 5.0, 459.0, 190.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU2",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.8, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

component7 = {
	tp = "pushButton",
	button_id = 2,
	shader_id = 9,
	limits = { 5.0, 418.0, 190.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU3",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.8, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

component8 = {
	tp = "pushButton",
	button_id = 3,
	shader_id = 9,
	limits = { 5.0, 377.0, 190.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU4",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.8, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}

component9 = {
	tp = "pushButton",
	button_id = 4,
	shader_id = 9,
	limits = { 5.0, 338.0, 190.0, 40.0 },
	font_id = 1,
	font_scale = 1.0,
	text_color = { 1.0, 1.0, 1.0 },
	texture = "",
	text = "XABLAU5",
	render_color = { 0.15, 0.15, 0.15, 1.0 },
	render_hover_color = { 0.8, 0.8, 0.5, 1.0 },
	render_border_color = { 0.3, 0.6, 0.3, 1.0 },
	render_border_width = 1,
	projection = { 0.0, 800.0, 0.0, 600.0 },
	track_mouse = true,
}