#include "SimpleModelLoader.hpp"

#include <fstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/bind.hpp>

#include "ModelData.hpp"

using namespace tutorials;
using namespace tutorials::graphics;
using namespace tutorials::graphics::model_loaders;

namespace /* anonymous */ {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace ascii = spirit::ascii;
namespace phoenix = boost::phoenix;


class SimpleModelParser : public qi::grammar<spirit::istream_iterator, void(), ascii::blank_type> {
public:

	SimpleModelParser() :
		SimpleModelParser::base_type(startRule_),
		data_(0)
	{
		blankRule_ = qi::eol;
		endRule_ = (qi::eol | qi::eoi) >> *blankRule_;
		vertexRule_ = qi::repeat(8)[qi::float_][boost::bind(&SimpleModelParser::makeVertex, this, _1)] >> endRule_;
		startRule_ = *blankRule_ >> *vertexRule_;
	}

	void parse(std::istream* is, ModelData* data) {
		data_ = data;
		data_->reset();

		is->unsetf(std::istream::skipws);
		spirit::istream_iterator it(*is), end;

		bool result = qi::phrase_parse(it, end, *this, ascii::blank);

		if (!result || it != end) {
			std::runtime_error("Failed to parse model file");
		}

		if (!data_->groups().empty()) {
			for (size_t i = 0; i < data_->vertices().size(); ++i) {
				data_->addIndex(i);
			}
		}

		data_ = 0;
	}

private:

	typedef qi::rule<spirit::istream_iterator, void(), ascii::blank_type> Rule;

	Rule blankRule_;

	Rule endRule_;

	Rule vertexRule_;

	Rule startRule_;

	ModelData* data_;

	void makeVertex(const std::vector<float>& rawData) {
		if (rawData.size() != 8) {
			throw std::logic_error("raw data size is not 8 - this shouldn't be possible");
		}

		if (data_->groups().empty()) {
			data_->addGroup(ModelData::TRIANGLE_LIST);
		}

		ModelData::Vertex vertex;
		vertex.position = D3DXVECTOR3(rawData[0], rawData[1], rawData[2]);
		vertex.textureCoordinate = D3DXVECTOR2(rawData[3], rawData[4]);
		vertex.normal = D3DXVECTOR3(rawData[5], rawData[6], rawData[7]);

		data_->addVertex(vertex);
	}

};

} // anonymous namespace

void SimpleModelLoader::load(ModelData* modelData, const boost::filesystem::path& sourcePath) {
	std::ifstream ifs(sourcePath.string().c_str());

	SimpleModelParser parser;
	parser.parse(&ifs, modelData);

	ifs.close();
}
