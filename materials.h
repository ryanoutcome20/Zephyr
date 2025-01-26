#pragma once

class Materials {
private:
	// cache map with all of our cached materials in here.
	std::unordered_map< hash32_t, IMaterial* > cache;
public:
	// caching functions.
	IMaterial* Get( std::string material );
	std::vector< IMaterial* > Get( std::vector< std::string > materials );
	std::vector< IMaterial* > GetGroup( std::string group );

	// modulation functions.
	void Modulate( std::vector< IMaterial* > materials, Color color, bool reset );

	void Modulate(std::vector< IMaterial* > materials) {
		return Modulate( materials, colors::white, true );
	}
};

extern Materials g_materials;