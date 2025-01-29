#pragma once

class Materials {
private:
	// cache map with all of our cached materials in here.
	std::unordered_map< hash32_t, IMaterial* > m_cache;

	// base path to apply to all custom materials.
	const std::filesystem::path m_base = "csgo\\materials";
public:
	// caching functions.
	IMaterial* Get( std::string material );
	std::vector< IMaterial* > Get( std::vector< std::string > materials );

	// creation functions.
	IMaterial* Create( const char* name, char* type = "Model textures" );
	IMaterial* New( const char* vmt, const char* shader );

	// modulation functions.
	void Modulate( std::vector< IMaterial* > materials, Color color, bool reset );

	void Modulate(std::vector< IMaterial* > materials) {
		return Modulate( materials, colors::white, true );
	}
};

extern Materials g_materials;