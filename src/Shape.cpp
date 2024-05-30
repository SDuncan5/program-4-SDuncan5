
#include "Shape.h"
#include <iostream>
#include <cassert>

#include "GLSL.h"
#include "Program.h"

using namespace std;


// copy the data from the shape to this object
void Shape::createShape(tinyobj::shape_t & shape)
{
	posBuf = shape.mesh.positions;
	norBuf = shape.mesh.normals;
	texBuf = shape.mesh.texcoords;
	eleBuf = shape.mesh.indices;
	if (norBuf.empty()) {
		cout << "norBuf empty" << endl;
		calcNormals();
	}
}

vector<float> Shape::crossProduct(vector<float> a, vector<float> b)
{
	vector<float> n(3, 0.0);

	n[0] = a[1] * b[2] - a[2] * b[1];
	n[1] = a[2] * b[0] - a[0] * b[2];
	n[2] = a[0] * b[1] - a[1] * b[0];

	return n;
}

void Shape::calcNormals()
{
	// allocate normal per vertex (fill w/ 0s)
	// (cycle through vertices)
	//std::vector<float> norBuf(posBuf.size(), 0.0);
	cout << "calculating normals" << endl;

	norBuf.assign(posBuf.size(), 0.0);
	//for (int i = 0; i < norBuf.size(); i++) {
	//	cout << norBuf[i] << endl;
	//}

	// calculate normal per face and add to each vertex
	// (cycle through faces/polygons?)
	// guessing elebuf has the triangles
	// guessing posbuf has the data
	// cycle through each elebuf triangle
	// init e1 and e2 placeholders
	std::vector<float> e1(3, 0.0);
	std::vector<float> e2(3, 0.0);
	// for each face
	for (int i = 0; i < eleBuf.size(); i += 3) {

		float v1Ndx = eleBuf[i];
		float v2Ndx = eleBuf[i + 1];
		float v3Ndx = eleBuf[i + 2];

		// Calc e1 (v2 - v1)
		e1[0] = posBuf[3*v2Ndx] - posBuf[3*v1Ndx]; // x
		e1[1] = posBuf[3*v2Ndx + 1] - posBuf[3*v1Ndx + 1]; // y
		e1[2] = posBuf[3*v2Ndx + 2] - posBuf[3*v1Ndx + 2]; // z

		// Calc e2 (v3 - v1)
		e2[0] = posBuf[3 * v3Ndx] - posBuf[3 * v1Ndx]; // x
		e2[1] = posBuf[3 * v3Ndx + 1] - posBuf[3 * v1Ndx + 1]; // y
		e2[2] = posBuf[3 * v3Ndx + 2] - posBuf[3 * v1Ndx + 2]; // z

		// Calc n = e1 x e2
		std::vector<float> n = crossProduct(e1, e2);

		//cout << "crossProduct" << endl;
		//for (int i = 0; i < n.size(); i++) {
		//	cout << norBuf[i] << " ";
		//}
		//cout << endl;

		// Add n to norBuf for each vertex
		//v1
		norBuf[3 * v1Ndx] += n[0]; // x
		norBuf[3 * v1Ndx + 1] += n[1]; // y
		norBuf[3 * v1Ndx + 2] += n[2]; // z
		//v2
		norBuf[3 * v2Ndx] += n[0]; // x
		norBuf[3 * v2Ndx + 1] += n[1]; // y
		norBuf[3 * v2Ndx + 2] += n[2]; // z
		//v3
		norBuf[3 * v3Ndx] += n[0]; // x
		norBuf[3 * v3Ndx + 1] += n[1]; // y
		norBuf[3 * v3Ndx + 2] += n[2]; // z
	}

	//for (int i = 0; i < norBuf.size(); i++) {
	//	cout << norBuf[i] << endl;
	//}


	// normalize normals
	// (cycle through norbuf)
	for (int i = 0; i < norBuf.size(); i += 3) {

		float x = norBuf[i];
		float y = norBuf[i + 1];
		float z = norBuf[i + 2];
		float mag = sqrt(x*x + y*y + z*z);

		norBuf[i] = x / mag;
		norBuf[i + 1] = y / mag;
		norBuf[i + 2] = z / mag;
	}
}

void Shape::measure()
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	minX = minY = minZ = (std::numeric_limits<float>::max)();
	maxX = maxY = maxZ = -(std::numeric_limits<float>::max)();

	//Go through all vertices to determine min and max of each dimension
	for (size_t v = 0; v < posBuf.size() / 3; v++)
	{
		if (posBuf[3*v+0] < minX) minX = posBuf[3 * v + 0];
		if (posBuf[3*v+0] > maxX) maxX = posBuf[3 * v + 0];

		if (posBuf[3*v+1] < minY) minY = posBuf[3 * v + 1];
		if (posBuf[3*v+1] > maxY) maxY = posBuf[3 * v + 1];

		if (posBuf[3*v+2] < minZ) minZ = posBuf[3 * v + 2];
		if (posBuf[3*v+2] > maxZ) maxZ = posBuf[3 * v + 2];
	}

	min.x = minX;
	min.y = minY;
	min.z = minZ;
	max.x = maxX;
	max.y = maxY;
	max.z = maxZ;
}



void Shape::init()
{
	// Initialize the vertex array object
	CHECKED_GL_CALL(glGenVertexArrays(1, &vaoID));
	CHECKED_GL_CALL(glBindVertexArray(vaoID));

	// Send the position array to the GPU
	CHECKED_GL_CALL(glGenBuffers(1, &posBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
	CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW));

	// Send the normal array to the GPU
	if (norBuf.empty())
	{
		norBufID = 0;
		cout << "norBuf is empty!!" << endl;
	}
	else
	{
		CHECKED_GL_CALL(glGenBuffers(1, &norBufID));
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
		CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW));
	}

	// Send the texture array to the GPU
	if (texBuf.empty())
	{
		texBufID = 0;
	}
	else
	{
		CHECKED_GL_CALL(glGenBuffers(1, &texBufID));
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
		CHECKED_GL_CALL(glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW));
	}

	// Send the element array to the GPU
	CHECKED_GL_CALL(glGenBuffers(1, &eleBufID));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));
	CHECKED_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), &eleBuf[0], GL_STATIC_DRAW));

	// Unbind the arrays
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Shape::draw(const shared_ptr<Program> prog) const
{
	int h_pos, h_nor, h_tex;
	h_pos = h_nor = h_tex = -1;

	CHECKED_GL_CALL(glBindVertexArray(vaoID));

	// Bind position buffer
	h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posBufID));
	CHECKED_GL_CALL(glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));

	// Bind normal buffer
	h_nor = prog->getAttribute("vertNor");
	if (h_nor != -1 && norBufID != 0)
	{
		GLSL::enableVertexAttribArray(h_nor);
		CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, norBufID));
		CHECKED_GL_CALL(glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0));
	}

	if (texBufID != 0)
	{
		// Bind texcoords buffer
		h_tex = prog->getAttribute("vertTex");

		if (h_tex != -1 && texBufID != 0)
		{
			GLSL::enableVertexAttribArray(h_tex);
			CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, texBufID));
			CHECKED_GL_CALL(glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0));
		}
	}

	// Bind element buffer
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID));

	// Draw
	CHECKED_GL_CALL(glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0));

	// Disable and unbind
	if (h_tex != -1)
	{
		GLSL::disableVertexAttribArray(h_tex);
	}
	if (h_nor != -1)
	{
		GLSL::disableVertexAttribArray(h_nor);
	}
	GLSL::disableVertexAttribArray(h_pos);
	CHECKED_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	CHECKED_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
