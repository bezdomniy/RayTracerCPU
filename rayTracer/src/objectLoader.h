#pragma once

#include "camera.h"
#include "cube.h"
#include "cylinder.h"
#include "fstream"
#include "material.h"
#include "pattern.h"
#include "plane.h"
#include "pointLight.h"
#include "shape.h"
#include "sphere.h"
#include "triangle.h"
#include "yaml-cpp/yaml.h"
// #include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class ObjectLoader
{
private:
  struct Value
  {
    bool isScalar;
    float scalar;
    std::unique_ptr<glm::vec3> vector;
  };

  struct Definition
  {
    bool empty = true;
    std::shared_ptr<Definition> inheritFrom;
    std::unordered_map<std::string, Value> values;
    std::vector<std::string> valueOrder;
    std::shared_ptr<Pattern> pattern;
  };

  std::unordered_map<std::string, std::shared_ptr<Definition>> definitions;

  std::shared_ptr<Shape> addShape(const YAML::Node &shapeNode);
  void addDefinition(const YAML::Node &definitionNode);
  void assignDefinition(std::shared_ptr<Shape> &shapePtr,
                        Definition &definition);
  void parseMaterial(const YAML::Node &node, Definition &definition);
  void parseTransform(const YAML::Node &node, Definition &definition);
  void parsePattern(const YAML::Node &node, Definition &definition);
  void parseArgs(const YAML::Node &node, std::vector<Value> &args);

public:
  ObjectLoader();
  ~ObjectLoader();
  std::pair<std::shared_ptr<Camera>, std::vector<std::shared_ptr<Shape>>>
  loadYaml(const std::string &fileName);
  std::pair<int, int> getCanvasSize(const std::string &fileName);
};
