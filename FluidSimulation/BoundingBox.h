#pragma once

#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <vector>

#include "Vector3d.h"

using namespace std;

class BoundingBox {

	vector<Vector3d> m_vertices;

public:

	BoundingBox(Vector3d v1, Vector3d v2, Vector3d v3, Vector3d v4,
		Vector3d v5, Vector3d v6, Vector3d v7, Vector3d v8) {
		m_vertices.resize(8);
		m_vertices.push_back(v1); m_vertices.push_back(v2);
		m_vertices.push_back(v3); m_vertices.push_back(v4);
		m_vertices.push_back(v5); m_vertices.push_back(v6);
		m_vertices.push_back(v7); m_vertices.push_back(v8);
	}

	virtual ~ BoundingBox() {}

	inline Vector3d get_vertex(const int i) const { return m_vertices[i]; }
	inline vector<Vector3d> get_vertices() const { return m_vertices; }

};



#endif