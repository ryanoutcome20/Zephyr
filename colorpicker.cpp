#include "includes.h"

// static variables.
int Colorpicker::texture = 0;;
int Colorpicker::saturation = 100;;
std::unique_ptr< Color[ ] > Colorpicker::gradient = nullptr;;

void Colorpicker::init() {
	const int w{ COLORPICKER_PICKER_SIZE };
	const int h{ COLORPICKER_PICKER_SIZE };

	// should never happen.
	if ( gradient )
		return;

	// allocate.
	gradient = std::make_unique< Color[ ] >(w * h);

	// init.
	float hue{}, lum{ 1.f };
	float sat = saturation / 100.f;

	// iterate width.
	for ( int i{}; i < w; ++i ) {

		// iterate height.
		for ( int j{}; j < h; ++j ) {

			// write back to array.
			*(Color*)(gradient.get() + i + j * w) = Color::hsl_to_rgb(hue, sat, lum);

			hue += (1.f / w);
		}

		lum -= (1.f / h);
		hue = 0.f;
	}

	// allocate new texture in engine.
	texture = g_csgo.m_surface->CreateNewTextureID(true);

	// assign allocated memory containing the picker to said texture.
	g_csgo.m_surface->DrawSetTextureRGBA(texture, gradient.get(), w, h);
}

void Colorpicker::draw( ) {
	Rect  area{ m_parent->GetElementsRect() };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	// fix drawing under other elements.
	if (m_open)
		m_parent->m_active_element = this;

	// label.
	render::menu_shade.string( p.x + LABEL_OFFSET, p.y - 2, { 205, 205, 205, m_parent->m_alpha }, m_label );

	// outline.
	render::rect( p.x + m_w - COLORPICKER_WIDTH, p.y, COLORPICKER_WIDTH, COlORPICKER_HEIGHT, { 0, 0, 0, m_parent->m_alpha } );

	// make a copy of the color.
	Color preview = m_color;

	// modify the alpha channel of this color.
	preview.a( ) = m_parent->m_alpha;

	// draw preview window.
	render::rect_filled( p.x + m_w - COLORPICKER_WIDTH + 1, p.y + 1, COLORPICKER_WIDTH - 2, COlORPICKER_HEIGHT - 2, preview );

	// fade / shine.
	render::rect_filled_fade( p.x + m_w - COLORPICKER_WIDTH + 1, p.y + 1, COLORPICKER_WIDTH - 2, COlORPICKER_HEIGHT - 2, { 50, 50, 35, m_parent->m_alpha }, 0, 150 );

	// draw picker
	if (m_open) {
		// make a copy of white.
		Color color = colors::white;

		// modify the alpha channel to make it fade with the menu.
		color.a( ) = m_parent->m_alpha;

		// set panel color.
		g_csgo.m_surface->DrawSetColor( color );

		// set texture.
		g_csgo.m_surface->DrawSetTexture( texture );

		// draw picker.
		g_csgo.m_surface->DrawTexturedRect( p.x + m_w - COLORPICKER_WIDTH,
			p.y + COlORPICKER_HEIGHT + 2,
			p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE,
			p.y + COlORPICKER_HEIGHT + 2 + COLORPICKER_PICKER_SIZE );

		// picker outline.
		render::rect( p.x + m_w - COLORPICKER_WIDTH,
			p.y + COlORPICKER_HEIGHT + 2,
			COLORPICKER_PICKER_SIZE + COLORPICKER_SATURATION_SIZE,
			COLORPICKER_PICKER_SIZE,
			{ 0, 0, 0, m_parent->m_alpha } );

		// saturation background.
		render::rect_filled(p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE,
			p.y + COlORPICKER_HEIGHT + 2,
			COLORPICKER_SATURATION_SIZE,
			COLORPICKER_PICKER_SIZE,
			{ 17, 17, 17, m_parent->m_alpha });

		// saturation bar.
		render::rect_filled_fade( p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE,
			p.y + COlORPICKER_HEIGHT + 2,
			COLORPICKER_SATURATION_SIZE,
			COLORPICKER_PICKER_SIZE,
			color, 255, 0 );
	}
}

void Colorpicker::think( ) {
	Rect  area{ m_parent->GetElementsRect( ) };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	if (m_open) {
		m_parent->m_active_element = this;

		// get our color from the actual gradient.
		Rect picker{ p.x + m_w - COLORPICKER_WIDTH, p.y + COlORPICKER_HEIGHT + 2, COLORPICKER_PICKER_SIZE, COLORPICKER_PICKER_SIZE };
		bool inpicker = g_input.IsCursorInRect(picker);

		if ( inpicker && g_input.GetKeyState(VK_LBUTTON) ) {
			m_color = Colorpicker::ColorFromPos(g_input.m_mouse.x - (p.x + m_w - COLORPICKER_WIDTH), g_input.m_mouse.y - (p.y + COlORPICKER_HEIGHT + 2));
		}

		// adjust for saturation.
		Rect saturation_picker{ p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE, p.y + COlORPICKER_HEIGHT + 2, COLORPICKER_SATURATION_SIZE, COLORPICKER_PICKER_SIZE };
		bool insat = g_input.IsCursorInRect(saturation_picker);

		if ( insat && g_input.GetKeyState(VK_LBUTTON) ) {
			saturation = 100 - ((g_input.m_mouse.y - (saturation_picker.y - 1)) * (100.f / 250.f));
			
			// reset our gradient.
			gradient.reset();
			gradient = nullptr;
			
			// call the init function again to redo the texture.
			Colorpicker::init();
		}

		// handle input.
		// this is dirty. the color picker button will instantly close because your on the button not the picker, write a library for rect
		// transformations please even if it would be less optimized.
		Rect input{ p.x + m_w - COLORPICKER_WIDTH - COLORPICKER_PICKER_GRACE, p.y + COlORPICKER_HEIGHT - COLORPICKER_PICKER_GRACE, COLORPICKER_PICKER_SIZE + COLORPICKER_SATURATION_SIZE + COLORPICKER_PICKER_GRACE + COLORPICKER_PICKER_GRACE_EXTENDED, COLORPICKER_PICKER_SIZE + COLORPICKER_PICKER_GRACE + COLORPICKER_PICKER_GRACE_EXTENDED };

		if ( g_input.GetKeyState(VK_LBUTTON) && !g_input.IsCursorInRect(input) ) {
			m_open = false;

			if ( m_callback )
				m_callback();
		}
	}

	if (m_ptr)
		*m_ptr = m_color;

	m_parent->m_priority = m_open ? this : nullptr;
}

void Colorpicker::click( ) {
	Rect  area{ m_parent->GetElementsRect( ) };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	Rect preview{ p.x + m_w - COLORPICKER_WIDTH, p.y, COLORPICKER_WIDTH, COlORPICKER_HEIGHT };

	if (g_input.IsCursorInRect( preview )) {
		saturation = 100;
		m_open = true;
	}
}