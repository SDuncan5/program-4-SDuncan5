#include "Model.h"
#include "Shape.h"

using namespace std;

Model::Model(std::string path)
{
	vector<tinyobj::shape_t> TOshapes;
	vector<tinyobj::material_t> objMaterials;
	string errStr;
	//load in the mesh and make the shape(s)
	bool rc = tinyobj::LoadObj(TOshapes, objMaterials, errStr, (path).c_str());

	if (!rc) {
		cerr << errStr << endl;
	}
	else {
		//for now all our shapes will not have textures - change in later labs

		for (int i = 0; i < TOshapes.size(); i++) {

			// push back into the meshes_shape vector
			shared_ptr<Shape> shape = make_shared<Shape>();
			mesh_shapes.push_back(shape);

			// need to get the shape out of mesh_shapes
			mesh_shapes[i]->createShape(TOshapes[i]);
			mesh_shapes[i]->measure();
			mesh_shapes[i]->init();
		}
	}
}

Model::~Model()
{
}


// Draws the model
void Model::draw(const shared_ptr<Program> prog) const {
	for (int i = 0; i < mesh_shapes.size(); i++) {
		mesh_shapes[i]->draw(prog);
	};
}