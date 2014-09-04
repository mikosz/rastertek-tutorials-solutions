#include "ModelData.hpp"

#if defined(max)
#undef max
#endif /* defined(max) */

#include <algorithm>
#include <cassert>

#include <boost/bind.hpp>

#include <D3DX10math.h>

using namespace tutorials;
using namespace tutorials::graphics;
using namespace tutorials::graphics::model_loaders;

void ModelData::reset() {
	groups_.clear();
	vertices_.clear();
	indices_.clear();
	maxIndex_ = 0;
}

void ModelData::addGroup(Topology topology) {
	groups_.push_back(Group(topology));
	groups_.back().firstIndexIndex = indices_.size();
}

void ModelData::addVertex(const Vertex& vertex) {
	vertices_.push_back(vertex);
}

void ModelData::addIndex(Index index) {
	indices_.push_back(index);
	maxIndex_ = std::max(index, maxIndex_);
}

void ModelData::generateNormals() {
	assert(!groups_.empty());

	const D3DXVECTOR3 NORMAL_NEEDS_GENERATION(0.0f, 0.0f, 0.0f);

	for (size_t indexIndex = groups_.back().firstIndexIndex; indexIndex < indices_.size(); ++indexIndex) {
		size_t index = indices_[indexIndex];

		if (vertices_[index].normal == NORMAL_NEEDS_GENERATION) {
			assert(groups_.back().topology == TRIANGLE_LIST); // TODO: TRIANGLE_STRIP
			generateNormal(indexIndex);
		}
	}
}

void ModelData::generateNormal(size_t indexIndex) {
	std::vector<D3DXVECTOR3> normals;

	const size_t vertexIndex = indices_[indexIndex];

	for (size_t i = indexIndex; i < indices_.size(); ++i) {
		if (indices_[i] == vertexIndex) {
			const size_t firstIndexInTriangle = i - (i % 3);

			D3DXVECTOR3 vector1(vertices_[indices_[firstIndexInTriangle + 1]].position - vertices_[indices_[firstIndexInTriangle]].position);
			D3DXVECTOR3 vector2(vertices_[indices_[firstIndexInTriangle + 2]].position - vertices_[indices_[firstIndexInTriangle]].position);

			D3DXVECTOR3 normal;
			D3DXVec3Cross(&normal, &vector1, &vector2);
			D3DXVec3Normalize(&normal, &normal);

			normals.push_back(normal);
		}
	}

	D3DXVECTOR3 sum(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < normals.size(); ++i) {
		sum = sum + normals[i];
	}

	D3DXVec3Normalize(&vertices_[indices_[indexIndex]].normal, &sum);
}
