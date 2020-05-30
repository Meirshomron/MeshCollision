#include "obj_loader.h"

static inline uint FindNextChar(uint start, const char* str, uint length, char token)
{
	uint result = start;

	while(result < length) {
		result++;
		if(str[result] == token)
			break;
	}

	return result;
}

static inline uint ParseOBJIndexValue(const std::string& token, uint start, uint end)
{
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, uint start, uint end)
{
	return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim) 
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	uint strLength = s.length();
	uint start = 0;
	uint end = 0;

	while(end <= strLength) 
	{
		while(end <= strLength) 
		{
			if(cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}

glm::vec2 OBJModel::ParseOBJVec2(const std::string& line) 
{
	uint tokenLength = line.length();
	const char* tokenString = line.c_str();

	uint vertIndexStart = 3;

	while(vertIndexStart < tokenLength) 
	{
		if(tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	uint vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec2(x,y);
}

glm::vec3 OBJModel::ParseOBJVec3(const std::string& line)
{
	uint tokenLength = line.length();
	const char* tokenString = line.c_str();

	uint vertIndexStart = 2;

	while(vertIndexStart < tokenLength)
	{
		if(tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	uint vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x,y,z);
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token)
{
	uint tokenLength = token.length();
	const char* tokenString = token.c_str();

	uint vertIndexStart = 0;
	uint vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	OBJIndex result;
	result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	result.uvIndex = 0;

	if(vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	hasUVs = true;

	if(vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	hasNormals = true;

	return result;
}

void OBJModel::CreateOBJFace(const std::string& line, std::set<Triangle>& trianglesSet) 
{
	std::vector<std::string> tokens = SplitString(line, ' ');

	trianglesSet.emplace(ParseOBJIndex(tokens[1]),
						ParseOBJIndex(tokens[2]),
						ParseOBJIndex(tokens[3]));

	//triangulation
	if(tokens.size() > 4U)
	{ 
		trianglesSet.emplace(ParseOBJIndex(tokens[1]),
							ParseOBJIndex(tokens[3]),
							ParseOBJIndex(tokens[4]));
	}
}