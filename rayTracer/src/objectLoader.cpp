#include "objectLoader.h"

ObjectLoader::ObjectLoader(/* args */) {}

ObjectLoader::~ObjectLoader() {}

std::pair<std::shared_ptr<Camera>, std::vector<std::shared_ptr<Shape>>>
ObjectLoader::loadYaml(const std::string &fileName) {
  std::vector<std::shared_ptr<Shape>> ret;
  std::shared_ptr<Camera> camera;

  YAML::Node root;

  // if (std::filesystem::exists(fileName))

  std::ifstream checkFile(fileName, std::ifstream::in);
  if (checkFile) {
    checkFile.close();
    root = YAML::LoadFile(fileName);
  } else
    root = YAML::Load(fileName);

  for (YAML::const_iterator it = root.begin(); it != root.end(); ++it) {
    assert((*it).Type() == YAML::NodeType::Map);
    if (it->begin()->first.as<std::string>() == "add") {
      if (it->begin()->second.as<std::string>() == "camera") {
        camera = std::dynamic_pointer_cast<Camera>(addShape(*it));
      } else {
        ret.push_back(addShape(*it));
      }
    } else if (it->begin()->first.as<std::string>() == "define") {
      addDefinition(*it);
    } else {
      throw std::invalid_argument("invalid operator in yaml file");
    }
  }

  return std::pair<std::shared_ptr<Camera>,
                   std::vector<std::shared_ptr<Shape>>>(camera, ret);
}

std::shared_ptr<Shape> ObjectLoader::addShape(const YAML::Node &shapeNode) {
  std::string shapeType;
  Definition materialDefinition;
  Definition transformDefinition;
  Definition cameraDefinition;
  Definition lightDefinition;
  std::shared_ptr<Shape> ret;
  std::vector<Value> args;

  for (YAML::const_iterator it = shapeNode.begin(); it != shapeNode.end();
       ++it) {
    std::string nextKey = it->first.as<std::string>();

    if (nextKey == "add") {
      shapeType = it->second.as<std::string>();
    } else if (nextKey == "args") {
      parseArgs(it->second, args);
    } else if (nextKey == "material") {
      parseMaterial(it->second, materialDefinition);
    } else if (nextKey == "transform") {
      parseTransform(it->second, transformDefinition);
    } else if (nextKey == "width") {
      cameraDefinition.values["width"] = Value{true, it->second.as<float>()};
      cameraDefinition.valueOrder.push_back("width");
    } else if (nextKey == "height") {
      cameraDefinition.values["height"] = Value{true, it->second.as<float>()};
      cameraDefinition.valueOrder.push_back("height");
    } else if (nextKey == "field-of-view") {
      cameraDefinition.values["field-of-view"] =
          Value{true, it->second.as<float>()};
      cameraDefinition.valueOrder.push_back("field-of-view");
    } else if (nextKey == "from") {
      cameraDefinition.values["from"] =
          Value{false, 0.f,
                std::make_unique<glm::vec3>(it->second[0].as<float>(),
                                            it->second[1].as<float>(),
                                            it->second[2].as<float>())};
      cameraDefinition.valueOrder.push_back("from");
    } else if (nextKey == "to") {
      cameraDefinition.values["to"] =
          Value{false, 0.f,
                std::make_unique<glm::vec3>(it->second[0].as<float>(),
                                            it->second[1].as<float>(),
                                            it->second[2].as<float>())};
      cameraDefinition.valueOrder.push_back("to");
    } else if (nextKey == "up") {
      cameraDefinition.values["up"] =
          Value{false, 0.f,
                std::make_unique<glm::vec3>(it->second[0].as<float>(),
                                            it->second[1].as<float>(),
                                            it->second[2].as<float>())};
      cameraDefinition.valueOrder.push_back("up");
    } else if (nextKey == "at") {
      lightDefinition.values["at"] =
          Value{false, 0.f,
                std::make_unique<glm::vec3>(it->second[0].as<float>(),
                                            it->second[1].as<float>(),
                                            it->second[2].as<float>())};
      lightDefinition.valueOrder.push_back("at");
    } else if (nextKey == "intensity") {
      lightDefinition.values["intensity"] =
          Value{false, 0.f,
                std::make_unique<glm::vec3>(it->second[0].as<float>(),
                                            it->second[1].as<float>(),
                                            it->second[2].as<float>())};
      lightDefinition.valueOrder.push_back("intensity");
    } else {
      throw std::invalid_argument("invalid operator in add statement");
    }
  }

  if (shapeType == "sphere") {
    ret = std::make_shared<Sphere>();
  } else if (shapeType == "plane") {
    ret = std::make_shared<Plane>();
  } else if (shapeType == "cube") {
    ret = std::make_shared<Cube>();
  } else if (shapeType == "cylinder") {
    if (args.empty())
      ret = std::make_shared<Cylinder>();
    else
      ret = std::make_shared<Cylinder>(Cylinder(
          args.at(0).scalar, args.at(1).scalar,
          std::abs(args.at(2).scalar) > std::numeric_limits<float>::epsilon()));
  } else if (shapeType == "triangle") {
    ret = std::make_shared<Triangle>(
        Triangle(*args.at(0).vector, *args.at(1).vector, *args.at(2).vector));
  } else if (shapeType == "camera") {
    ret = std::make_shared<Camera>(
        glm::vec4(*cameraDefinition.values["from"].vector, 1.f),
        glm::vec4(*cameraDefinition.values["to"].vector, 1.f),
        glm::vec4(*cameraDefinition.values["up"].vector, 0.f),
        cameraDefinition.values["width"].scalar,
        cameraDefinition.values["height"].scalar,
        cameraDefinition.values["field-of-view"].scalar);
  } else if (shapeType == "light") {
    ret = std::make_shared<PointLight>(
        glm::vec4(*lightDefinition.values["at"].vector, 1.f),
        *lightDefinition.values["intensity"].vector);
  } else {
    throw std::invalid_argument("this shape is not implemented");
  }

  if (!materialDefinition.empty)
    assignDefinition(ret, materialDefinition);
  if (!transformDefinition.empty)
    assignDefinition(ret, transformDefinition);

  return ret;
}

void ObjectLoader::parseArgs(const YAML::Node &node, std::vector<Value> &args) {
  for (auto &item : node) {
    if (item.IsSequence()) {
      args.push_back(Value{false, 0.f,
                           std::make_unique<glm::vec3>(item[0].as<float>(),
                                                       item[1].as<float>(),
                                                       item[2].as<float>())});
    } else if (item.IsScalar()) {
      args.push_back(Value{true, item.as<float>()});
    } else {
      throw std::invalid_argument("invalid arguement in shape definition");
    }
  }
}

void ObjectLoader::assignDefinition(std::shared_ptr<Shape> &shapePtr,
                                    Definition &definition) {
  if (!shapePtr->material)
    shapePtr->material = std::make_shared<Material>();

  if (definition.inheritFrom) {
    assignDefinition(shapePtr, *definition.inheritFrom);
  }
  for (auto &value : definition.valueOrder) {
    if (value == "color") {
      shapePtr->material->colour = *definition.values[value].vector;
    } else if (value == "diffuse") {
      shapePtr->material->diffuse = definition.values[value].scalar;
    } else if (value == "ambient") {
      shapePtr->material->ambient = definition.values[value].scalar;
    } else if (value == "specular") {
      shapePtr->material->specular = definition.values[value].scalar;
    } else if (value == "shininess") {
      shapePtr->material->shininess = definition.values[value].scalar;
    } else if (value == "reflective") {
      shapePtr->material->reflective = definition.values[value].scalar;
    } else if (value == "transparency") {
      shapePtr->material->transparency = definition.values[value].scalar;
    } else if (value == "refractive-index") {
      shapePtr->material->refractiveIndex = definition.values[value].scalar;
    } else if (value == "translate") {
      glm::mat4 translation =
          glm::translate(glm::mat4(1.f), *definition.values[value].vector);
      shapePtr->multiplyTransform(translation);
    } else if (value == "scale") {
      glm::mat4 scale =
          glm::scale(glm::mat4(1.f), *definition.values[value].vector);
      shapePtr->multiplyTransform(scale);
    } else if (value == "rotate-x") {
      glm::mat4 rotation =
          glm::rotate(glm::mat4(1.f), definition.values[value].scalar,
                      glm::vec3(1.f, 0.f, 0.f));
      shapePtr->multiplyTransform(rotation);
    } else if (value == "rotate-y") {
      glm::mat4 rotation =
          glm::rotate(glm::mat4(1.f), definition.values[value].scalar,
                      glm::vec3(0.f, 1.f, 0.f));
      shapePtr->multiplyTransform(rotation);
    } else if (value == "rotate-z") {
      glm::mat4 rotation =
          glm::rotate(glm::mat4(1.f), definition.values[value].scalar,
                      glm::vec3(0.f, 0.f, 1.f));
      shapePtr->multiplyTransform(rotation);
    } else {
      throw std::invalid_argument("invalid operator in value statement");
    }
  }
  if (definition.pattern) {
    shapePtr->material->setPattern(definition.pattern);
    shapePtr->material->pattern->calculateInverseTranform();
  }

  shapePtr->calculateInverseTranform();
}

// TODO: add patterns
void ObjectLoader::addDefinition(const YAML::Node &definitionNode) {
  std::shared_ptr<Definition> newDefinition = std::make_shared<Definition>();
  std::string name;
  for (YAML::const_iterator it = definitionNode.begin();
       it != definitionNode.end(); ++it) {
    std::string nextKey = it->first.as<std::string>();
    if (nextKey == "define") {
      name = it->second.as<std::string>();
    } else if (nextKey == "extend") {
      newDefinition->inheritFrom = definitions.at(it->second.as<std::string>());
    } else if (nextKey == "value") {
      if (it->second.IsMap()) {
        parseMaterial(it->second, *newDefinition);
      } else if (it->second.IsSequence()) {
        parseTransform(it->second, *newDefinition);
      } else {
        throw std::invalid_argument("value must be sequence or map");
      }
    } else {
      throw std::invalid_argument("invalid operator in define statement");
    }
  }

  definitions[name] = newDefinition;
}

void ObjectLoader::parseMaterial(const YAML::Node &node,
                                 Definition &definition) {
  if (node.IsScalar()) {
    definition.inheritFrom = definitions.at(node.as<std::string>());
  } else {
    for (YAML::const_iterator valueIt = node.begin(); valueIt != node.end();
         ++valueIt) {
      std::string valueKey = valueIt->first.as<std::string>();

      if (valueKey == "color") {
        definition.values["color"] =
            Value{false, 0.f,
                  std::make_unique<glm::vec3>(valueIt->second[0].as<float>(),
                                              valueIt->second[1].as<float>(),
                                              valueIt->second[2].as<float>())};
        definition.valueOrder.push_back("color");
      } else if (valueKey == "diffuse") {
        float diffuse = valueIt->second.as<float>();
        definition.values["diffuse"] = Value{true, diffuse};
        definition.valueOrder.push_back("diffuse");
      } else if (valueKey == "ambient") {
        float ambient = valueIt->second.as<float>();
        definition.values["ambient"] = Value{true, ambient};
        definition.valueOrder.push_back("ambient");
      } else if (valueKey == "specular") {
        float specular = valueIt->second.as<float>();
        definition.values["specular"] = Value{true, specular};
        definition.valueOrder.push_back("specular");
      } else if (valueKey == "shininess") {
        float shininess = valueIt->second.as<float>();
        definition.values["shininess"] = Value{true, shininess};
        definition.valueOrder.push_back("shininess");
      } else if (valueKey == "reflective") {
        float reflective = valueIt->second.as<float>();
        definition.values["reflective"] = Value{true, reflective};
        definition.valueOrder.push_back("reflective");
      } else if (valueKey == "transparency") {
        float transparency = valueIt->second.as<float>();
        definition.values["transparency"] = Value{true, transparency};
        definition.valueOrder.push_back("transparency");
      } else if (valueKey == "refractive-index") {
        float refractiveIndex = valueIt->second.as<float>();
        definition.values["refractive-index"] = Value{true, refractiveIndex};
        definition.valueOrder.push_back("refractive-index");
      } else if (valueKey == "pattern") {
        parsePattern(valueIt->second, definition);
      } else {
        throw std::invalid_argument("invalid operator in value statement");
      }
    }
  }
  definition.empty = false;
}

void ObjectLoader::parsePattern(const YAML::Node &node,
                                Definition &definition) {
  std::string type;
  std::shared_ptr<Pattern> pattern;
  float perturbedCoeff;
  bool blendedPattern = false;
  for (YAML::const_iterator valueIt = node.begin(); valueIt != node.end();
       ++valueIt) {
    std::string valueKey = valueIt->first.as<std::string>();

    if (valueKey == "type") {
      type = valueKey;
      if (valueIt->second.as<std::string>() == "stripes") {
        pattern = std::make_unique<StripedPattern>(glm::vec3(1.f, 0.f, 0.f),
                                                   glm::vec3(0.f, 1.f, 0.f));
      } else if (valueIt->second.as<std::string>() == "gradient") {
        pattern = std::make_unique<GradientPattern>(glm::vec3(1.f, 0.f, 0.f),
                                                    glm::vec3(0.f, 1.f, 0.f));
      } else if (valueIt->second.as<std::string>() == "rings") {
        pattern = std::make_unique<RingPattern>(glm::vec3(1.f, 0.f, 0.f),
                                                glm::vec3(0.f, 1.f, 0.f));
      } else if (valueIt->second.as<std::string>() == "checkers") {
        pattern = std::make_unique<CheckedPattern>(glm::vec3(1.f, 0.f, 0.f),
                                                   glm::vec3(0.f, 1.f, 0.f));
      } else if (valueIt->second.as<std::string>() == "blended") {
        blendedPattern = true;
        break;
      } else {
        throw std::invalid_argument("invalid pattern type");
      }
    } else if (valueKey == "perturbed") {
      perturbedCoeff = valueIt->second.as<float>();
    } else if (valueKey == "colors") {
      if (type == "blended")
        throw std::invalid_argument("invalid arguement for blended pattern");
      if (type == "perturbed")
        throw std::invalid_argument("invalid arguement for perturbed pattern");

      ColourPattern *colourPattern =
          dynamic_cast<ColourPattern *>(pattern.get());

      glm::vec3 colourA(valueIt->second[0][0].as<float>(),
                        valueIt->second[0][1].as<float>(),
                        valueIt->second[0][2].as<float>());
      glm::vec3 colourB(valueIt->second[1][0].as<float>(),
                        valueIt->second[1][1].as<float>(),
                        valueIt->second[1][2].as<float>());

      colourPattern->setColour(colourA, 0);
      colourPattern->setColour(colourB, 1);
    } else if (valueKey == "transform") {
      for (auto &transformValue : valueIt->second) {
        if (transformValue[0].as<std::string>() == "translate") {
          glm::vec3 vector(transformValue[1].as<float>(),
                           transformValue[2].as<float>(),
                           transformValue[3].as<float>());
          glm::mat4 translation = glm::translate(glm::mat4(1.f), vector);
          pattern->multiplyTransform(translation);
        } else if (transformValue[0].as<std::string>() == "scale") {
          glm::vec3 vector(transformValue[1].as<float>(),
                           transformValue[2].as<float>(),
                           transformValue[3].as<float>());
          glm::mat4 scale = glm::scale(glm::mat4(1.f), vector);
          pattern->multiplyTransform(scale);
        } else if (transformValue[0].as<std::string>() == "rotate-x") {
          glm::mat4 rotation =
              glm::rotate(glm::mat4(1.f), transformValue[1].as<float>(),
                          glm::vec3(1.f, 0.f, 0.f));
          pattern->multiplyTransform(rotation);
        } else if (transformValue[0].as<std::string>() == "rotate-y") {
          glm::mat4 rotation =
              glm::rotate(glm::mat4(1.f), transformValue[1].as<float>(),
                          glm::vec3(0.f, 1.f, 0.f));
          pattern->multiplyTransform(rotation);
        } else if (transformValue[0].as<std::string>() == "rotate-z") {
          glm::mat4 rotation =
              glm::rotate(glm::mat4(1.f), transformValue[1].as<float>(),
                          glm::vec3(0.f, 0.f, 1.f));
          pattern->multiplyTransform(rotation);
        }
      }
    } else {
      throw std::invalid_argument("invalid key in pattern definition");
    }
  }

  if (blendedPattern) {
    if (node["perturbed"])
      perturbedCoeff = node["perturbed"].as<float>();

    Definition patternADefinition;
    Definition patternBDefinition;

    parsePattern(node["patterns"][0], patternADefinition);
    parsePattern(node["patterns"][1], patternBDefinition);

    pattern = std::make_shared<BlendedPattern>(patternADefinition.pattern,
                                               patternBDefinition.pattern);
  }

  if (perturbedCoeff) {
    pattern = std::make_shared<PerturbedPattern>(pattern, perturbedCoeff);
  }
  definition.pattern = std::move(pattern);
}

void ObjectLoader::parseTransform(const YAML::Node &node,
                                  Definition &definition) {
  if (node.IsScalar()) {
    definition.inheritFrom = definitions.at(node.as<std::string>());
  } else if (node.IsSequence()) {
    for (auto &item : node) {
      if (item.IsScalar()) {
        definition.inheritFrom = definitions.at(item.as<std::string>());
      } else {
        if (item[0].as<std::string>().substr(0, 6) == "rotate") {
          definition.values[item[0].as<std::string>()] =
              Value{true, item[1].as<float>()};
          definition.valueOrder.push_back(item[0].as<std::string>());
        } else {
          definition.values[item[0].as<std::string>()] =
              Value{false, 0.f,
                    std::make_unique<glm::vec3>(item[1].as<float>(),
                                                item[2].as<float>(),
                                                item[3].as<float>())};
          definition.valueOrder.push_back(item[0].as<std::string>());
        }
      }
    }
  } else {
    throw std::invalid_argument(
        "value as sequence must contain either extension name or sequence");
  }
  definition.empty = false;
}