#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "yaml-cpp/yaml.h"
#include "shape.h"
#include "sphere.h"
#include "plane.h"
#include "patterns.h"
#include "material.h"

class ObjectLoader
{
private:
    struct definition
    {
        std::shared_ptr<definition> inheritFrom;
        std::unordered_map<std::string, float> scalarValues;
        std::unordered_map<std::string, glm::vec4> vectorValues;
    };

    std::unique_ptr<Shape> add(YAML::Node &shapeNode);
    /* data */
public:
    ObjectLoader(/* args */);
    ~ObjectLoader();
    std::vector<std::unique_ptr<Shape>> loadYaml(std::string &fileName);
    std::unordered_map<std::string, definition> definitions;
};
