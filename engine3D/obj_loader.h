#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <list>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>

typedef unsigned int uint;

static inline glm::dvec4 operator*(const glm::vec3& v, const glm::dmat4& mat)
{
	return glm::transpose(mat) * glm::dvec4(v, 1.0f);
}

static inline double operator*(const glm::dvec4& a, const glm::vec3& b) 
{
	return glm::dot(a, glm::dvec4(b, 1.0f));
}

struct OBJIndex 
{
	uint vertexIndex;
	uint uvIndex;

	inline bool operator<(const OBJIndex& other) const 
	{
		return vertexIndex < other.vertexIndex;
	}

	inline bool operator>(const OBJIndex& other) const 
	{
		return other < *this;
	}

	inline bool operator==(const OBJIndex& other) const 
	{
		return vertexIndex == other.vertexIndex;
	}

	inline bool operator!=(const OBJIndex& other) const 
	{
		return !operator==(other);
	}
};

struct IndexedModel 
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<uint> indices;
};

struct Edge
{
	union
	{
		OBJIndex keep, u;
	};

	union 
	{
		OBJIndex drop, v;
	};

	double error;

	Edge(const OBJIndex& a, const OBJIndex& b, const double& error) : keep((a < b ? a : b)), drop((a < b ? b : a)), error(error) {}
	Edge(const OBJIndex& a, const OBJIndex& b) : Edge(a, b, 0) {}
	Edge(const Edge& other, const double& error) : Edge(other.keep, other.drop, error) {}

	inline bool operator== (const Edge& other) const
	{
		return (u == other.u && v == other.v);
	}

	inline bool operator!= (const Edge& other) const 
	{
		return !operator==(other);
	}

	inline bool lexicographicOrder(const Edge& other) const
	{
		return keep < other.keep || (keep == other.keep && drop < other.drop);
	}

	inline bool operator<(const Edge& other) const 
	{
		return error < other.error || (error == other.error && lexicographicOrder(other));
	}

	inline bool Contains(const OBJIndex& a) const
	{
		return u == a || v == a;
	}

	inline Edge Replace(const OBJIndex& oldIndex, const OBJIndex& newIndex) const 
	{
		const OBJIndex& stay = (oldIndex == u) ? v : u;
		return Edge(newIndex, stay);
	}
};

struct Triangle
{
	OBJIndex a, b, c;
	bool isDeleted;

	Triangle(const OBJIndex& a, const OBJIndex& b, const OBJIndex& c) : a(a), b(b), c(c), isDeleted(false) {}

	//lexicographic order
	bool operator<(const Triangle& other) const
	{ 
		OBJIndex key[] = { a, b, c };
		OBJIndex otherKey[] = { other.a, other.b, other.c };

		std::sort(key, key + 3);
		std::sort(otherKey, otherKey + 3);

		for (int i = 2; i >= 0; --i)
			if (key[i] < otherKey[i])
				return true;
			else if (key[i] > otherKey[i])
				return false;

		return false;
	}

	inline void forEach(const std::function<void(OBJIndex& obj)>& foo) 
	{
		foo(a);
		foo(b);
		foo(c);
	}

	inline void forEach(const std::function<void(const OBJIndex& obj)>& foo) const
	{
		foo(a);
		foo(b);
		foo(c);
	}

	inline void forEach(const std::function<void(Edge&& e)>& foo) 
	{
		foo(Edge(a, b));
		foo(Edge(b, c));
		foo(Edge(c, a));
	}

	inline void forEach(const std::function<void(Edge&& e)>& foo) const
	{
		foo(Edge(a, b));
		foo(Edge(b, c));
		foo(Edge(c, a));
	}

	inline bool Contains(const Edge& edge) const 
	{
		bool ans = false;
		forEach([&](Edge&& e) 
		{
			ans |= (edge == e);
		});
		return ans;
	}

	inline void Delete() 
	{
		isDeleted = true;
	}

	inline Triangle* Replace(const OBJIndex& oldIndex, const OBJIndex& newIndex) 
	{
		forEach([&](OBJIndex& obj)
		{
			if (obj == oldIndex)
				obj = newIndex;
		});
		return this;
	}
};


struct LexicographicComp 
{
	bool operator()(const Edge& lhs, const Edge& rhs) const
	{
		return lhs.lexicographicOrder(rhs);
	}
};

struct Plane;
class OBJModel
{
public:
	OBJModel(const std::string& fileName);
	OBJModel(const std::string& fileName, size_t maxFaces);

	virtual ~OBJModel() {}

	void ToIndexedModel(IndexedModel& indexModel);

private:
	//MARK: fields
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;

	bool hasUVs;
	bool hasNormals;

	// Quadratic
	std::vector<glm::dmat4> qs; 
	std::list<Triangle> triangles;
	std::vector<std::set<Triangle*>> neighbors;
	std::set<Edge, LexicographicComp> edgesLex;
	std::set<Edge> edgesWeights;
	size_t maxFaces;

	//MARK: friends
	friend Plane;

	//MARK: obj_loader_parser.cpp
	glm::vec2 ParseOBJVec2(const std::string& line);
	glm::vec3 ParseOBJVec3(const std::string& line);
	OBJIndex ParseOBJIndex(const std::string& token);
	void CreateOBJFace(const std::string& line, std::set<Triangle>& trianglesSet);

	void InitQ();
	void SimplifyMesh();
	void CalcNormals(const std::set<OBJIndex>& remainingVertices);

	inline glm::vec3 MakeNormal(uint i0, uint i1, uint i2) const
	{
		glm::vec3 v1 = vertices[i1] - vertices[i0];
		glm::vec3 v2 = vertices[i2] - vertices[i0];
		glm::vec3 cross = glm::cross(v2, v1);
		return cross == glm::vec3(0, 0, 0) ? cross : glm::normalize(cross);
	}

	inline glm::vec3 GetMidPosition(const Edge& e) 
	{
		uint u = e.u.vertexIndex;
		uint v = e.v.vertexIndex;
		return (vertices[u] + vertices[v]) * 0.5f;
	}

	inline glm::vec3 GetMinPosition(const Edge& e, uint keep) 
	{
		glm::dmat4 q = qs[keep];
		q[3][0] = q[3][1] = q[3][2] = 0;
		q[3][3] = 1;

		if (std::abs(glm::determinant(q)) > 0.005)
		{
			q = glm::inverse(q);
			return glm::vec3(q[0][3], q[1][3], q[2][3]);
		}
		return GetMidPosition(e);
	}

	inline void UpdateVertexPos(const Edge& e) 
	{
		uint keep = e.keep.vertexIndex;
		vertices[keep] = GetMinPosition(e, keep);
	}

	inline glm::dmat4 GetNewQ(const Edge& e) 
	{
		uint u = e.u.vertexIndex;
		uint v = e.v.vertexIndex;
		return qs[u] + qs[v];
	}

	inline void UpdateQ(const Edge& e) 
	{
		uint keep = e.keep.vertexIndex;
		qs[keep] = GetNewQ(e);
	}

	inline Edge GetWeightedEdge(const Edge& e)
	{
		glm::vec3 vTag = GetMidPosition(e);
		glm::dmat4 qTag = GetNewQ(e);
		double error = vTag * qTag * vTag;
		return Edge(e, error);
	}
};

struct Plane 
{
	float a, b, c, d;
	Plane(const Triangle& t, const OBJModel& model)
	{
		uint i0 = t.a.vertexIndex;
		uint i1 = t.b.vertexIndex;
		uint i2 = t.c.vertexIndex;
		glm::vec3 normal = model.MakeNormal(i0, i1, i2);
		a = normal.x;
		b = normal.y;
		c = normal.z;
		const glm::vec3& p0 = model.vertices[i0];
		d = -glm::dot(normal, p0);
	}
};

static inline glm::dmat4 makeQ(const Plane& p) 
{
	float vec[] = { p.a, p.b, p.c, p.d };
	glm::dmat4 ans;
	for (uint i = 0; i < 4; ++i)
		for (uint j = 0; j < 4; ++j)
			ans[i][j] = vec[i] * vec[j];

	return ans;
}

#endif // OBJ_LOADER_H_INCLUDED
