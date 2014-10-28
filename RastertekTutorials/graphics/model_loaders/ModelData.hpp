#ifndef _GRAPHICS_MODELLOADERS_MODELDATA_HPP_
#define _GRAPHICS_MODELLOADERS_MODELDATA_HPP_

#include <vector>

#include <boost/cstdint.hpp>
#include <boost/filesystem/path.hpp>

#include <D3DX10math.h>

namespace tutorials {
namespace graphics {
namespace model_loaders {

class ModelData {
public:

	enum Topology {
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
	};

	struct Vertex {
		
		// TODO: change D3DX Vectors to a portable type
		D3DXVECTOR3 position;

		D3DXVECTOR2 textureCoordinate;

		D3DXVECTOR3 normal;

	};

	typedef boost::uint32_t Index;

	struct Group {

		Topology topology;

		size_t firstIndexIndex;

		Group(Topology topology) :
			topology(topology),
			firstIndexIndex(0)
		{
		}

	};

	typedef std::vector<Group> Groups;

	typedef std::vector<Vertex> Vertices;

	typedef std::vector<Index> Indices;

	ModelData() {
		reset();
	}

	void reset();

	void addGroup(Topology topology);

	void addVertex(const Vertex& vertex);

	void addIndex(Index index);

	void setBaseTexture(const boost::filesystem::path& baseTexture) {
		baseTexture_ = baseTexture;
	}

	void setDetailTexture(const boost::filesystem::path& detailTexture) {
		detailTexture_ = detailTexture;
	}

	void generateNormals();

	const Groups& groups() const {
		return groups_;
	}

	const Vertices& vertices() const {
		return vertices_;
	}

	const Indices& indices() const {
		return indices_;
	}

	const boost::filesystem::path& baseTexture() const {
		return baseTexture_;
	}

	const boost::filesystem::path& detailTexture() const {
		return detailTexture_;
	}

private:

	Groups groups_;

	Vertices vertices_;

	Indices indices_;

	size_t maxIndex_;

	boost::filesystem::path baseTexture_;

	boost::filesystem::path detailTexture_;

	void generateNormal(size_t indexIndex);

};

} // namespace model_loaders
} // namespace graphics
} // namespace tutorials

#endif /* _GRAPHICS_MODELLOADERS_MODELDATA_HPP_ */
