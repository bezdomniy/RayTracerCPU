#pragma once

#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "yaml-cpp/yaml.h"
#include "shape.h"
#include "sphere.h"
#include "plane.h"
#include "cube.h"
#include "patterns.h"
#include "material.h"
#include "camera.h"
#include "pointLight.h"

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
        std::unique_ptr<Pattern> pattern;
    };

    std::unordered_map<std::string, std::shared_ptr<Definition>> definitions;

    std::shared_ptr<Shape> addShape(const YAML::Node &shapeNode);
    void addDefinition(const YAML::Node &definitionNode);
    void assignDefinition(std::shared_ptr<Shape> &shapePtr, Definition &definition);
    void parseMaterial(const YAML::Node &node, Definition &definition);
    void parseTransform(const YAML::Node &node, Definition &definition);
    void parsePattern(const YAML::Node &node, Definition &definition);

public:
    ObjectLoader();
    ~ObjectLoader();
    std::pair<std::shared_ptr<Camera>, std::vector<std::shared_ptr<Shape>>> loadYaml(const std::string &fileName);
};
