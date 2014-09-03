#include "ModelData.hpp"

#if defined(max)
#undef max
#endif /* defined(max) */

#include <algorithm>
#include <cassert>

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
