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

std::shared_ptr<Shape> ObjectLoader::shapeFromDefinition(ShapeDefinition& shapeDefinition) {
  std::shared_ptr<Shape> ret;
  // Add new shapes here

  if (definitions.find(shapeDefinition.shapeType) != definitions.end()) {
    std::shared_ptr<ShapeDefinition> shapeDef = std::dynamic_pointer_cast<ShapeDefinition>(definitions.at(shapeDefinition.shapeType));
    ret = shapeFromDefinition(*shapeDef);
    // throw std::invalid_argument(shapeDefinition.shapeType+": hahah");
  } else if (shapeDefinition.shapeType == "sphere") {
    ret = std::make_shared<Sphere>();
  } else if (shapeDefinition.shapeType == "plane") {
    ret = std::make_shared<Plane>();
  } else if (shapeDefinition.shapeType == "cube") {
    ret = std::make_shared<Cube>();
  } else if (shapeDefinition.shapeType == "cylinder") {
    if (shapeDefinition.args.empty())
      ret = std::make_shared<Cylinder>();
    else
      ret = std::make_shared<Cylinder>(Cylinder(
          shapeDefinition.args.at(0).scalar, shapeDefinition.args.at(1).scalar,
          std::abs(shapeDefinition.args.at(2).scalar) > std::numeric_limits<double>::epsilon()));
  } else if (shapeDefinition.shapeType == "cone") {
    if (shapeDefinition.args.empty())
      ret = std::make_shared<Cone>();
    else
      ret = std::make_shared<Cone>(Cone(
          shapeDefinition.args.at(0).scalar, shapeDefinition.args.at(1).scalar,
          std::abs(shapeDefinition.args.at(2).scalar) > std::numeric_limits<double>::epsilon()));
  } else if (shapeDefinition.shapeType == "triangle") {
    ret = std::make_shared<Triangle>(
        Triangle(shapeDefinition.args.at(0).vector, shapeDefinition.args.at(1).vector, shapeDefinition.args.at(2).vector));
  } else if (shapeDefinition.shapeType == "group") {
    // TODO
    std::shared_ptr group = std::make_shared<Group>();

    for (auto& child: shapeDefinition.children) {
      std::shared_ptr<Shape> nextChildShape = shapeFromDefinition(child);
      group->addChild(nextChildShape);
    }
    ret = group;
  } else if (shapeDefinition.shapeType == "camera") {
    ret = std::make_shared<Camera>(
        glm::dvec4(shapeDefinition.values["from"].vector, 1.0),
        glm::dvec4(shapeDefinition.values["to"].vector, 1.0),
        glm::dvec4(shapeDefinition.values["up"].vector, 0.0),
        shapeDefinition.values["width"].scalar,
        shapeDefinition.values["height"].scalar,
        shapeDefinition.values["field-of-view"].scalar);
  } else if (shapeDefinition.shapeType == "light") {
    ret = std::make_shared<PointLight>(
        glm::dvec4(shapeDefinition.values["at"].vector, 1.0),
        shapeDefinition.values["intensity"].vector);
  } else {
    throw std::invalid_argument(shapeDefinition.shapeType+": this shape is not implemented");
  }

  if (!shapeDefinition.materialDefinition.empty)
    assignDefinition(ret, shapeDefinition.materialDefinition);
  if (!shapeDefinition.transformDefinition.empty)
    assignDefinition(ret, shapeDefinition.transformDefinition);

  return ret;
}

std::shared_ptr<Shape> ObjectLoader::addShape(const YAML::Node &shapeNode) {
  
  ShapeDefinition shapeDefinition;

  parseShape(shapeNode, shapeDefinition);
    
  return shapeFromDefinition(shapeDefinition);
}

void ObjectLoader::parseShape(const YAML::Node &node, ShapeDefinition &shapeDefinition)
{
  for (YAML::const_iterator it = node.begin(); it != node.end();
       ++it) {
    std::string nextKey = it->first.as<std::string>();

    if (nextKey == "add") {
      std::string nextDef = it->second.as<std::string>();
      if (definitions.find(nextDef) != definitions.end()) {
        std::shared_ptr<Definition> t = definitions.at(nextDef);
        std::shared_ptr<ShapeDefinition> s = std::dynamic_pointer_cast<ShapeDefinition>(t);
        shapeDefinition = *s;
      }
      else {
        shapeDefinition.shapeType = nextDef;
      }
      
      // if (shapeDefinitions[shapeType]) {
      //   shapeDefinition = *shapeDefinitions[shapeType];
      // }
    } else if (nextKey == "children") {
      for (auto &item : it->second) {
        // TODO
        ShapeDefinition childShapeDefinition;
        parseShape(item, childShapeDefinition);
        shapeDefinition.children.push_back(childShapeDefinition);
      }
    } else if (nextKey == "args") {
      parseArgs(it->second, shapeDefinition.args);
    } else if (nextKey == "material") {
      parseMaterial(it->second, shapeDefinition.materialDefinition);
    } else if (nextKey == "transform") {
      parseTransform(it->second, shapeDefinition.transformDefinition);
    } else if (nextKey == "width") {
      shapeDefinition.values["width"] = Value{it->second.as<double>()};
      shapeDefinition.valueOrder.push_back("width");
    } else if (nextKey == "height") {
      shapeDefinition.values["height"] = Value{it->second.as<double>()};
      shapeDefinition.valueOrder.push_back("height");
    } else if (nextKey == "field-of-view") {
      shapeDefinition.values["field-of-view"] =
          Value{it->second.as<double>()};
      shapeDefinition.valueOrder.push_back("field-of-view");
    } else if (nextKey == "from") {
      shapeDefinition.values["from"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                                            it->second[1].as<double>(),
                                            it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("from");
    } else if (nextKey == "to") {
      shapeDefinition.values["to"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                                            it->second[1].as<double>(),
                                            it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("to");
    } else if (nextKey == "up") {
      shapeDefinition.values["up"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                                            it->second[1].as<double>(),
                                            it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("up");
    } else if (nextKey == "at") {
      shapeDefinition.values["at"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                                            it->second[1].as<double>(),
                                            it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("at");
    } else if (nextKey == "intensity") {
      shapeDefinition.values["intensity"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                                            it->second[1].as<double>(),
                                            it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("intensity");
    } else if (nextKey == "shadow") {
      // TODO add functionality for shadowless shape
    } else {
      throw std::invalid_argument("invalid operator in add statement: " +
                                  nextKey);
    }
  }
}

void ObjectLoader::parseArgs(const YAML::Node &node, std::vector<Value> &args) {
  for (auto &item : node) {
    if (item.IsSequence()) {
      args.push_back(Value{0.0,
                           glm::dvec3(item[0].as<double>(),
                                                       item[1].as<double>(),
                                                       item[2].as<double>())});
    } else if (item.IsScalar()) {
      args.push_back(Value{item.as<double>()});
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
      shapePtr->material->colour = definition.values[value].vector;
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
      glm::dmat4 translation =
          glm::translate(glm::dmat4(1.0), definition.values[value].vector);
      shapePtr->multiplyTransform(translation);
    } else if (value == "scale") {
      glm::dmat4 scale =
          glm::scale(glm::dmat4(1.0), definition.values[value].vector);
      shapePtr->multiplyTransform(scale);
    } else if (value == "rotate-x") {
      glm::dmat4 rotation =
          glm::rotate(glm::dmat4(1.0), definition.values[value].scalar,
                      glm::dvec3(1.0, 0.0, 0.0));
      shapePtr->multiplyTransform(rotation);
    } else if (value == "rotate-y") {
      glm::dmat4 rotation =
          glm::rotate(glm::dmat4(1.0), definition.values[value].scalar,
                      glm::dvec3(0.0, 1.0, 0.0));
      shapePtr->multiplyTransform(rotation);
    } else if (value == "rotate-z") {
      glm::dmat4 rotation =
          glm::rotate(glm::dmat4(1.0), definition.values[value].scalar,
                      glm::dvec3(0.0, 0.0, 1.0));
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
  std::shared_ptr<ShapeDefinition> newDefinitionShape = std::make_shared<ShapeDefinition>();
  std::shared_ptr<Definition> newDefinition = newDefinitionShape;
  // std::shared_ptr<ShapeDefinition> newDefinitionShape = std::dynamic_pointer_cast<ShapeDefinition>(newDefinition);

  std::string name;
  std::string inheritFromStr;

  for (YAML::const_iterator it = definitionNode.begin(); it != definitionNode.end(); ++it) {
    std::string nextKey = it->first.as<std::string>();
    if (nextKey == "define") {
      name = it->second.as<std::string>();
    } else if (nextKey == "extend") {
      inheritFromStr = it->second.as<std::string>();
    } else if (nextKey == "value") {
      if (it->second.IsMap()) {
        if (it->second["add"]) {
          // TODO ahh because this one is going out of scope!
          // std::shared_ptr<ShapeDefinition> newDefinition = std::make_shared<ShapeDefinition>();
          parseShape(it->second, *newDefinitionShape);
          
        } else{
          parseMaterial(it->second, *newDefinition);
        }
      } else if (it->second.IsSequence()) {
        parseTransform(it->second, *newDefinition);
      }
      
      else {
        throw std::invalid_argument("value must be map or sequence");
      }
    } else {
      throw std::invalid_argument("invalid operator in define statement");
    }
  }

  if (!inheritFromStr.empty())
    newDefinition->inheritFrom = definitions.at(inheritFromStr);
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
            Value{0.0,
                  glm::dvec3(valueIt->second[0].as<double>(),
                                              valueIt->second[1].as<double>(),
                                              valueIt->second[2].as<double>())};
        definition.valueOrder.push_back("color");
      } else if (valueKey == "diffuse") {
        double diffuse = valueIt->second.as<double>();
        definition.values["diffuse"] = Value{diffuse};
        definition.valueOrder.push_back("diffuse");
      } else if (valueKey == "ambient") {
        double ambient = valueIt->second.as<double>();
        definition.values["ambient"] = Value{ambient};
        definition.valueOrder.push_back("ambient");
      } else if (valueKey == "specular") {
        double specular = valueIt->second.as<double>();
        definition.values["specular"] = Value{specular};
        definition.valueOrder.push_back("specular");
      } else if (valueKey == "shininess") {
        double shininess = valueIt->second.as<double>();
        definition.values["shininess"] = Value{shininess};
        definition.valueOrder.push_back("shininess");
      } else if (valueKey == "reflective") {
        double reflective = valueIt->second.as<double>();
        definition.values["reflective"] = Value{reflective};
        definition.valueOrder.push_back("reflective");
      } else if (valueKey == "transparency") {
        double transparency = valueIt->second.as<double>();
        definition.values["transparency"] = Value{transparency};
        definition.valueOrder.push_back("transparency");
      } else if (valueKey == "refractive-index") {
        double refractiveIndex = valueIt->second.as<double>();
        definition.values["refractive-index"] = Value{refractiveIndex};
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
  double perturbedCoeff;
  bool blendedPattern = false;
  for (YAML::const_iterator valueIt = node.begin(); valueIt != node.end();
       ++valueIt) {
    std::string valueKey = valueIt->first.as<std::string>();

    if (valueKey == "type") {
      type = valueKey;
      if (valueIt->second.as<std::string>() == "stripes") {
        pattern = std::make_unique<StripedPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                   glm::dvec3(0.0, 1.0, 0.0));
      } else if (valueIt->second.as<std::string>() == "gradient") {
        pattern = std::make_unique<GradientPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                    glm::dvec3(0.0, 1.0, 0.0));
      } else if (valueIt->second.as<std::string>() == "rings") {
        pattern = std::make_unique<RingPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                glm::dvec3(0.0, 1.0, 0.0));
      } else if (valueIt->second.as<std::string>() == "checkers") {
        pattern = std::make_unique<CheckedPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                   glm::dvec3(0.0, 1.0, 0.0));
      } else if (valueIt->second.as<std::string>() == "blended") {
        blendedPattern = true;
        break;
      } else {
        throw std::invalid_argument("invalid pattern type");
      }
    } else if (valueKey == "perturbed") {
      perturbedCoeff = valueIt->second.as<double>();
    } else if (valueKey == "colors") {
      if (type == "blended")
        throw std::invalid_argument("invalid arguement for blended pattern");
      if (type == "perturbed")
        throw std::invalid_argument("invalid arguement for perturbed pattern");

      ColourPattern *colourPattern =
          dynamic_cast<ColourPattern *>(pattern.get());

      glm::dvec3 colourA(valueIt->second[0][0].as<double>(),
                        valueIt->second[0][1].as<double>(),
                        valueIt->second[0][2].as<double>());
      glm::dvec3 colourB(valueIt->second[1][0].as<double>(),
                        valueIt->second[1][1].as<double>(),
                        valueIt->second[1][2].as<double>());

      colourPattern->setColour(colourA, 0);
      colourPattern->setColour(colourB, 1);
    } else if (valueKey == "transform") {
      for (auto &transformValue : valueIt->second) {
        if (transformValue[0].as<std::string>() == "translate") {
          glm::dvec3 vector(transformValue[1].as<double>(),
                           transformValue[2].as<double>(),
                           transformValue[3].as<double>());
          glm::dmat4 translation = glm::translate(glm::dmat4(1.0), vector);
          pattern->multiplyTransform(translation);
        } else if (transformValue[0].as<std::string>() == "scale") {
          glm::dvec3 vector(transformValue[1].as<double>(),
                           transformValue[2].as<double>(),
                           transformValue[3].as<double>());
          glm::dmat4 scale = glm::scale(glm::dmat4(1.0), vector);
          pattern->multiplyTransform(scale);
        } else if (transformValue[0].as<std::string>() == "rotate-x") {
          glm::dmat4 rotation =
              glm::rotate(glm::dmat4(1.0), transformValue[1].as<double>(),
                          glm::dvec3(1.0, 0.0, 0.0));
          pattern->multiplyTransform(rotation);
        } else if (transformValue[0].as<std::string>() == "rotate-y") {
          glm::dmat4 rotation =
              glm::rotate(glm::dmat4(1.0), transformValue[1].as<double>(),
                          glm::dvec3(0.0, 1.0, 0.0));
          pattern->multiplyTransform(rotation);
        } else if (transformValue[0].as<std::string>() == "rotate-z") {
          glm::dmat4 rotation =
              glm::rotate(glm::dmat4(1.0), transformValue[1].as<double>(),
                          glm::dvec3(0.0, 0.0, 1.0));
          pattern->multiplyTransform(rotation);
        }
      }
    } else {
      throw std::invalid_argument("invalid key in pattern definition");
    }
  }

  if (blendedPattern) {
    if (node["perturbed"])
      perturbedCoeff = node["perturbed"].as<double>();

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
              Value{item[1].as<double>()};
          definition.valueOrder.push_back(item[0].as<std::string>());
        } else {
          definition.values[item[0].as<std::string>()] =
              Value{0.0,
                    glm::dvec3(item[1].as<double>(),
                                                item[2].as<double>(),
                                                item[3].as<double>())};
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

// std::shared_ptr<Shape> ObjectLoader::loadModel(const std::string &fileName) {
//   return std::make_shared<Cube>();
// }