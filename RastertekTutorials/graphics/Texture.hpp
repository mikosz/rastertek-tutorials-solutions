#ifndef _GRAPHICS_TEXTURE_HPP_
#define _GRAPHICS_TEXTURE_HPP_

#include <boost/filesystem/path.hpp>

#include <d3d11.h>

#include "utils/COMWrapper.hpp"

namespace tutorials {
namespace graphics {

class Texture {
public:

	Texture() {
	}

	void initialise(ID3D11Device* device, const boost::filesystem::path& textureFile);

	void initialise(utils::COMWrapper<ID3D11ShaderResourceView> shaderResourceView);

	void reset();

	ID3D11ShaderResourceView* resource() {
		return texture_;
	}

private:

	utils::COMWrapper<ID3D11ShaderResourceView> texture_;

};

} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_TEXTURE_HPP_ */
