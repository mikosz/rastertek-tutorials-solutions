#include "ObjModelLoader.hpp"

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

class ObjModelParser : public qi::grammar<spirit::istream_iterator, void(), ascii::blank_type> {
public:

	struct Vertex {
		size_t positionIndex;
		size_t textureCoordinateIndex;
		size_t normalIndex;
	};

	struct Face {
		Vertex vertices[3];
	};

	struct Object {
		std::vector<Face> faces;
	};

	typedef std::vector<Object> Objects;

	typedef std::vector<D3DXVECTOR3> Positions;

	typedef std::vector<D3DXVECTOR2> TextureCoordinates;

	typedef std::vector<D3DXVECTOR3> Normals;
	
	ObjModelParser() :
		ObjModelParser::base_type(startRule_),
		data_(0)
	{
		blankRule_ = -(qi::char_('#') >> *(qi::char_ - qi::eol)) >> qi::eol;
		endRule_ = (qi::eol | qi::eoi) >> *blankRule_;
		smoothingGroupRule_ = 's' >> (qi::lit("off") | qi::int_) >> endRule_;
		materialRule_ = qi::lit("usemtl") >> (*(qi::char_ - ascii::space)) >> endRule_;
		vertexRule_ = (qi::uint_ % '/')[qi::_val = phoenix::bind(&ObjModelParser::makeVertex, this, qi::_1)];
		faceRule_ = 'f' >> qi::repeat(3)[vertexRule_][boost::bind(&ObjModelParser::addFace, this, _1)] >> endRule_;
		positionRule_ = 'v' >> qi::repeat(3)[qi::double_][boost::bind(&ObjModelParser::addPosition, this, _1)] >> endRule_;
		textureCoordinateRule_ = qi::lit("vt") >> qi::repeat(2)[qi::double_][boost::bind(&ObjModelParser::addTextureCoordinate, this, _1)] >> endRule_;
		normalRule_ = qi::lit("vn") >> qi::repeat(3)[qi::double_][boost::bind(&ObjModelParser::addNormal, this, _1)] >> endRule_;
		objectNameRule_ = (qi::char_('o') | 'g') >> (*(qi::char_ - ascii::space))[boost::bind(&ObjModelParser::newObject, this, _1)] >> endRule_;
		materialLibRule_ = qi::lit("mtllib") >> (*(qi::char_ - ascii::space)) >> endRule_;

		startRule_ = *blankRule_
			>> *(
					smoothingGroupRule_ |
					materialRule_ |
					materialLibRule_ |
					faceRule_ |
					positionRule_ |
					textureCoordinateRule_ |
					normalRule_ |
					objectNameRule_
				);
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

	const Objects& objects() const {
		return objects_;
	}

	const Positions& positions() const {
		return positions_;
	}
	const TextureCoordinates& textureCoordinates() const {
		return textureCoordinates_;
	}
	const Normals& normals() const {
		return normals_;
	}

private:

	typedef qi::rule<spirit::istream_iterator, void(), ascii::blank_type> Rule;

	typedef qi::rule<spirit::istream_iterator, Vertex(), ascii::blank_type> VertexRule;

	Rule blankRule_;

	Rule endRule_;

	Rule smoothingGroupRule_;

	Rule materialRule_;

	VertexRule vertexRule_;

	Rule faceRule_;

	Rule positionRule_;

	Rule textureCoordinateRule_;

	Rule normalRule_;

	Rule objectNameRule_;

	Rule materialLibRule_;

	Rule startRule_;

	ModelData* data_;

	Objects objects_;

	Positions positions_;

	TextureCoordinates textureCoordinates_;

	Normals normals_;

	Vertex makeVertex(const std::vector<size_t>& vertexData) {
		if (vertexData.size() != 3) {
			throw std::runtime_error("Vertex data has size different than 3");
		}

		Vertex vertex;
		vertex.positionIndex = vertexData[0] - 1;
		vertex.textureCoordinateIndex = vertexData[1] - 1;
		vertex.normalIndex = vertexData[2] - 1;

		return vertex;
	}

	void addFace(const std::vector<Vertex>& face) {
		if (face.size() != 3) {
			throw std::runtime_error("Currently supporting only faces with 3 vertices");
		}
		if (objects_.empty()) {
			throw std::runtime_error("Attempted to add a face with no object specified");
		}

		Face storedFace;
		storedFace.vertices[0] = face[0];
		storedFace.vertices[1] = face[1];
		storedFace.vertices[2] = face[2];
		objects_.back().faces.push_back(storedFace);
	}

	void addPosition(const std::vector<double>& vector) {
		assert(vector.size() == 3);
		positions_.push_back(
			D3DXVECTOR3(static_cast<float>(vector[0]), static_cast<float>(vector[1]), static_cast<float>(vector[2])));
	}

	void addTextureCoordinate(const std::vector<double>& vector) {
		assert(vector.size() == 2);
		textureCoordinates_.push_back(D3DXVECTOR2(static_cast<float>(vector[0]), static_cast<float>(vector[1])));
	}

	void addNormal(const std::vector<double>& vector) {
		assert(vector.size() == 3);
		normals_.push_back(
			D3DXVECTOR3(static_cast<float>(vector[0]), static_cast<float>(vector[1]), static_cast<float>(vector[2])));
	}

	void newObject(const std::vector<char>& chars) {
		objects_.push_back(Object());
	}

};

} // anonymous namespace

void ObjModelLoader::load(ModelData* modelData, const boost::filesystem::path& sourcePath) {
	std::ifstream ifs(sourcePath.string().c_str());

	ObjModelParser parser;
	parser.parse(&ifs, modelData);

	ifs.close();
}
