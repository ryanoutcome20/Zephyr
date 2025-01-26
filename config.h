#pragma once

class Config {
public:
	void init( );
	void load( const Form* form, const std::string& name );
	void save( const Form* form, const std::string& name );
	void remove( const Form* form, const std::string& name);
	void update( const Form* form );

private:
	bool m_init;
	std::string m_path;
};

extern Config g_config;