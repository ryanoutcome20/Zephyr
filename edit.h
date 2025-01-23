#pragma once

#define EDIT_X_OFFSET	   20
#define EDIT_BOX_HEIGHT	   20
#define EDIT_ITEM_X_OFFSET 10

class Edit : public Element {
public:
	// ctor.
	__forceinline Edit( ) : m_text{}, m_limit{}, m_typing{ false }, m_old_typing{ false }, m_offset{} {
		m_flags     = ElementFlags::CLICK | ElementFlags::DRAW | ElementFlags::SAVE | ElementFlags::ACTIVE;
		m_type      = ElementTypes::EDIT;
		m_show      = true;
	}

	__forceinline void setup( const std::string &label, const std::string &file_id, size_t limit = 3, bool use_label = true, const std::string& value = "", bool use_chars = false) {
		m_label   = label;
		m_use_label = use_label;
		m_text = value;
		m_file_id = file_id;
		m_limit   = limit;
		m_offset = m_use_label ? 15 : 0;
		m_base_h  = m_h = m_offset + EDIT_BOX_HEIGHT;
		m_use_chars = use_chars;
	}

	__forceinline int get( ) {
		if( m_text.empty( ) )
			return -1;

		return std::stoi( m_text );
	}

	__forceinline std::string gets() {
		return m_text;
	}

	__forceinline void set( int val ) {
		bool changed = val != get( );

		if( val == -1 )
			m_text.clear( );

		else
			m_text = std::to_string( val );

		if( changed && m_callback )
			m_callback( );
	}

	__forceinline void sets( std::string val ) {
		bool changed = val != gets();

		m_text = val;

		if ( changed && m_callback )
			m_callback();
	}

protected:
	std::string m_text;
	size_t		m_limit;
	bool		m_typing;
	bool		m_old_typing;
	bool		m_use_chars;
	int          m_offset;

protected:
	void draw( ) override;
	void think( ) override;
	void click( ) override;
};