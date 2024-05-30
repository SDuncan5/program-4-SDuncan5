#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader/tiny_obj_loader.h>

class Shape;
class Program;

class Model {

public:
	Model(std::string path);
	virtual ~Model();
	void draw(const std::shared_ptr<Program> prog) const;
	std::vector<std::shared_ptr<Shape>> getMesh() const { return mesh_shapes; };
private:
	std::vector<std::shared_ptr<Shape>> mesh_shapes;
};

#endif