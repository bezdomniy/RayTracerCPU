#pragma once

#include "camera.h"
#include "cube.h"
#include "cylinder.h"
#include "material.h"
#include "pattern.h"
#include "plane.h"
#include "pointLight.h"
#include "shape.h"
#include "sphere.h"
#include "cone.h"
#include "group.h"
#include "triangle.h"
#include "firBranch.h"
#include "model.h"
#include "world.h"

#include "yaml-cpp/yaml.h"
#include "fstream"
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <unordered_map>
#include <vector>

#ifdef __EMSCRIPTEN__
// #include <emscripten.h>
#include <emscripten/fetch.h>
#endif

class ObjectLoader
{
private:
  struct Value
  {
    double scalar;
    glm::dvec3 vector;
  };

  // TODO why do I even use a definition? Why not just make the shapes and add copies to the world?
  struct Definition
  {
    virtual ~Definition() = default;
    bool empty = true;
    std::shared_ptr<Definition> inheritFrom;
    std::unordered_map<std::string, Value> values;
    std::vector<std::string> valueOrder;
    std::shared_ptr<Pattern> pattern;

    std::unordered_map<std::string, int> transformCounts{{"rotate-x", 0}, {"rotate-y", 0}, {"rotate-z", 0}, {"translate", 0}, {"scale", 0}};
  };

  struct ShapeDefinition : Definition
  {
    std::string shapeType;
    Definition materialDefinition;
    Definition transformDefinition;
    std::vector<Value> args;
    std::vector<ShapeDefinition> children;
    std::string filePath;
  };

  std::unordered_map<std::string, std::shared_ptr<Definition>> definitions;
  // std::unordered_map<std::string, std::shared_ptr<ShapeDefinition>> shapeDefinitions;

  std::shared_ptr<Shape> addShape(const YAML::Node &shapeNode);
  std::shared_ptr<Shape> shapeFromDefinition(ShapeDefinition &shapeDefinition);

  void addDefinition(const YAML::Node &definitionNode);
  void assignDefinition(std::shared_ptr<Shape> &shapePtr,
                        Definition &definition);

  void parseMaterial(const YAML::Node &node, Definition &definition);
  void parseTransform(const YAML::Node &node, Definition &definition);
  void parsePattern(const YAML::Node &node, Definition &definition);
  void parseArgs(const YAML::Node &node, std::vector<Value> &args);
  void parseShape(const YAML::Node &node, ShapeDefinition &shapeDefinition);
  void parseUV(const YAML::Node &node, std::shared_ptr<UVTexture> &uvTexture, int face = 0);

#ifdef __EMSCRIPTEN__
  static void downloadAsset(const std::string &assetPath, const std::string &targetPath);
  static void downloadSucceeded(emscripten_fetch_t *fetch);
  static void downloadFailed(emscripten_fetch_t *fetch);
#endif

public:
  ObjectLoader();
  ~ObjectLoader();
  std::pair<std::shared_ptr<Camera>, std::unique_ptr<World>> loadYaml(const std::string &fileName);
  std::shared_ptr<Shape> loadModel(const std::string &fileName);
  std::pair<int, int> getCanvasSize(const std::string &fileName);
};
