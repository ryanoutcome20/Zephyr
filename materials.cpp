#include "includes.h"

Materials		g_materials{};;

IMaterial* Materials::Get( std::string material ) {
	hash32_t hash = FNV1a::get( material );
	
	// check if we've already looked for this one.
	if( m_cache[ hash ] )
		return m_cache[ hash ];

	// iterate material handles.
	for ( uint16_t h{ g_csgo.m_material_system->FirstMaterial() }; h != g_csgo.m_material_system->InvalidMaterial(); h = g_csgo.m_material_system->NextMaterial(h) ) {
		// get material from handle.
		IMaterial* target = g_csgo.m_material_system->GetMaterial(h);
		if ( !target )
			continue;

		// compare hashes with name and return if equal.
		if ( FNV1a::get(target->GetName()) == hash ) {
			m_cache[ hash ] = target;
			return target;
		}
	}

	// couldn't find your material, exit.
	return nullptr;
}

std::vector< IMaterial* > Materials::Get( std::vector< std::string > materials ) {
	// I know this isn't the most optimized way of doing this but I prefer it since
	// it decreases code complexity. Should only run once anyway.

	// our completed final vector.
	std::vector< IMaterial* > completed;

	// loop through and add to the final vector.
	for ( auto& material : materials ) {
		IMaterial* final = Get( material );

		if( final )
			completed.push_back( final );
	}

	// return finished product.
	return completed;
}

IMaterial* Materials::Create( const char* name, char* type ) {
	IMaterial* mat = g_csgo.m_material_system->FindMaterial( name, type );
	
	if( !mat )
		return nullptr;

	// increment games index handler.
	mat->IncrementReferenceCount( );

	// return our new material.
	return mat;
}

IMaterial* Materials::New( const char* vmt, const char* shader ) {
	// note that this really should be converted into a dynamic texture
	// and have its variables written dynamically at injection time instead
	// of dumping the texture to disk.

	// create path.
	std::filesystem::path path = m_base / (std::string(vmt) + ".vmt");

	// open our write stream.
	std::ofstream stream{ path };

	// file can't be opened.
	if ( !stream )
		return nullptr; 

	// write shader.
	stream << shader;

	// close stream so the game can read the file.
	stream.close( );

	// stream will automatically deconstruct when it goes out of scope.
	return Create( vmt  );
}

void Materials::Modulate( std::vector< IMaterial* > materials, Color color, bool reset ) {
	for ( const auto& material : materials ) {
		// we are resetting our colors, set back to non modulated.
		if( reset ) { 
			material->ColorModulate(1.f, 1.f, 1.f);
			material->AlphaModulate(1.f);
			continue;
		}

		// we are coloring our material.
		material->ColorModulate(color.r( ) / 255.f, color.g( ) / 255.f, color.b( ) / 255.f);
		material->AlphaModulate(color.a( ) / 255.f);
	}
}