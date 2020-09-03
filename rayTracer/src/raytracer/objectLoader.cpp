#include "objectLoader.h"

ObjectLoader::ObjectLoader(/* args */) {}

ObjectLoader::~ObjectLoader() {}

std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>>
ObjectLoader::loadYaml(const std::string &fileName)
{
  // std::vector<std::shared_ptr<Shape>> ret;
  std::shared_ptr<Camera> camera;
  std::shared_ptr<World> world = std::make_shared<World>();

  // world->shapes.clear();
  // world->lights.clear();

  YAML::Node root;

  if (std::__fs::filesystem::exists(fileName))
  {
    std::cout << "Found found: " << fileName << std::endl;
    root = YAML::LoadFile(fileName);
  }
  else
  {
    std::cout << "Found not found: " << fileName << std::endl;
    root = YAML::Load(fileName);
  }

  for (YAML::const_iterator it = root.begin(); it != root.end(); ++it)
  {
    // std::cout << "Node: " << it->begin()->first.as<std::string>() << std::endl;
    assert((*it).Type() == YAML::NodeType::Map);
    if (it->begin()->first.as<std::string>() == "add")
    {
      if (it->begin()->second.as<std::string>() == "camera")
      {
        camera = std::dynamic_pointer_cast<Camera>(addShape(*it));
      }
      else if (it->begin()->second.as<std::string>() == "light")
      {
        std::shared_ptr<PointLight> light = std::dynamic_pointer_cast<PointLight>(addShape(*it));
        world->addLight(light);
      }
      else
      {
        std::shared_ptr<Shape> shape = addShape(*it);
        world->addShape(shape);
      }
    }
    else if (it->begin()->first.as<std::string>() == "define")
    {
      addDefinition(*it);
    }
    else
    {
      throw std::invalid_argument("invalid operator in yaml file");
    }
  }

  return std::pair<std::shared_ptr<Camera>,
                   std::shared_ptr<World>>(camera, world);
}

std::shared_ptr<Shape> ObjectLoader::addShape(const YAML::Node &shapeNode)
{
  ShapeDefinition shapeDefinition;

  parseShape(shapeNode, shapeDefinition);

  return shapeFromDefinition(shapeDefinition);
}

std::shared_ptr<Shape> ObjectLoader::shapeFromDefinition(ShapeDefinition &shapeDefinition)
{
  std::shared_ptr<Shape> ret;
  // Add new shapes here

  if (definitions.find(shapeDefinition.shapeType) != definitions.end())
  {
    std::shared_ptr<ShapeDefinition> shapeDef = std::dynamic_pointer_cast<ShapeDefinition>(definitions.at(shapeDefinition.shapeType));
    ret = shapeFromDefinition(*shapeDef);
    // throw std::invalid_argument(shapeDefinition.shapeType+": hahah");
  }
  else if (shapeDefinition.shapeType == "sphere")
  {
    ret = std::make_shared<Sphere>();
  }
  else if (shapeDefinition.shapeType == "plane")
  {
    ret = std::make_shared<Plane>();
  }
  else if (shapeDefinition.shapeType == "cube")
  {
    ret = std::make_shared<Cube>();
  }
  else if (shapeDefinition.shapeType == "cylinder")
  {
    if (shapeDefinition.args.empty())
      ret = std::make_shared<Cylinder>();
    else
      ret = std::make_shared<Cylinder>(Cylinder(
          shapeDefinition.args.at(0).scalar, shapeDefinition.args.at(1).scalar,
          std::abs(shapeDefinition.args.at(2).scalar) > std::numeric_limits<double>::epsilon()));
  }
  else if (shapeDefinition.shapeType == "cone")
  {
    if (shapeDefinition.args.empty())
      ret = std::make_shared<Cone>();
    else
      ret = std::make_shared<Cone>(Cone(
          shapeDefinition.args.at(0).scalar, shapeDefinition.args.at(1).scalar,
          std::abs(shapeDefinition.args.at(2).scalar) > std::numeric_limits<double>::epsilon()));
  }
  else if (shapeDefinition.shapeType == "triangle")
  {
    ret = std::make_shared<Triangle>(
        Triangle(shapeDefinition.args.at(0).vector, shapeDefinition.args.at(1).vector, shapeDefinition.args.at(2).vector));
  }
  else if (shapeDefinition.shapeType == "group")
  {
    std::shared_ptr<Group> group = std::make_shared<Group>();

    for (auto &child : shapeDefinition.children)
    {
      std::shared_ptr<Shape> nextChildShape = shapeFromDefinition(child);
      group->addChild(nextChildShape);
    }
    ret = group;
  }
  else if (shapeDefinition.shapeType == "obj")
  {
    Model model;
    model.build(shapeDefinition.filePath, true);

    ret = model.mesh;
  }
  else if (shapeDefinition.shapeType == "camera")
  {
    ret = std::make_shared<Camera>(
        glm::dvec4(shapeDefinition.values["from"].vector, 1.0),
        glm::dvec4(shapeDefinition.values["to"].vector, 1.0),
        glm::dvec4(shapeDefinition.values["up"].vector, 0.0),
        shapeDefinition.values["width"].scalar,
        shapeDefinition.values["height"].scalar,
        shapeDefinition.values["field-of-view"].scalar);
  }
  else if (shapeDefinition.shapeType == "light")
  {
    ret = std::make_shared<PointLight>(
        glm::dvec4(shapeDefinition.values["at"].vector, 1.0),
        shapeDefinition.values["intensity"].vector);
  }
  else if (shapeDefinition.shapeType == "fir_branch")
  {
    std::shared_ptr<Group> temp = FirBranch::build();
    ret = std::dynamic_pointer_cast<Shape>(temp);
  }

  else
  {
    throw std::invalid_argument(shapeDefinition.shapeType + ": this shape is not implemented");
  }

  if (!shapeDefinition.materialDefinition.empty)
    assignDefinition(ret, shapeDefinition.materialDefinition);
  if (!shapeDefinition.transformDefinition.empty)
    assignDefinition(ret, shapeDefinition.transformDefinition);

  if (shapeDefinition.values.count("shadow") && !shapeDefinition.values["shadow"].scalar)
    ret->material->shadow = false;
  return ret;
}

void ObjectLoader::parseShape(const YAML::Node &node, ShapeDefinition &shapeDefinition)
{
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
  {
    std::string nextKey = it->first.as<std::string>();

    if (nextKey == "add")
    {
      std::string nextDef = it->second.as<std::string>();
      if (definitions.find(nextDef) != definitions.end())
      {
        std::shared_ptr<Definition> t = definitions.at(nextDef);
        std::shared_ptr<ShapeDefinition> s = std::dynamic_pointer_cast<ShapeDefinition>(t);
        shapeDefinition = *s;
      }
      else
      {
        shapeDefinition.shapeType = nextDef;
      }

      // if (shapeDefinitions[shapeType]) {
      //   shapeDefinition = *shapeDefinitions[shapeType];
      // }
    }
    else if (nextKey == "children")
    {
      for (auto &item : it->second)
      {
        // TODO
        ShapeDefinition childShapeDefinition;
        parseShape(item, childShapeDefinition);
        shapeDefinition.children.push_back(childShapeDefinition);
      }
    }
    else if (nextKey == "args")
    {
      parseArgs(it->second, shapeDefinition.args);
    }
    else if (nextKey == "file")
    {
      shapeDefinition.filePath = it->second.as<std::string>();
    }
    else if (nextKey == "material")
    {
      parseMaterial(it->second, shapeDefinition.materialDefinition);
    }
    else if (nextKey == "transform")
    {
      parseTransform(it->second, shapeDefinition.transformDefinition);
    }
    else if (nextKey == "width")
    {
      shapeDefinition.values["width"] = Value{it->second.as<double>()};
      shapeDefinition.valueOrder.push_back("width");
    }
    else if (nextKey == "height")
    {
      shapeDefinition.values["height"] = Value{it->second.as<double>()};
      shapeDefinition.valueOrder.push_back("height");
    }
    else if (nextKey == "field-of-view")
    {
      shapeDefinition.values["field-of-view"] =
          Value{it->second.as<double>()};
      shapeDefinition.valueOrder.push_back("field-of-view");
    }
    else if (nextKey == "from")
    {
      shapeDefinition.values["from"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                           it->second[1].as<double>(),
                           it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("from");
    }
    else if (nextKey == "to")
    {
      shapeDefinition.values["to"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                           it->second[1].as<double>(),
                           it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("to");
    }
    else if (nextKey == "up")
    {
      shapeDefinition.values["up"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                           it->second[1].as<double>(),
                           it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("up");
    }
    else if (nextKey == "at")
    {
      shapeDefinition.values["at"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                           it->second[1].as<double>(),
                           it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("at");
    }
    else if (nextKey == "intensity")
    {
      shapeDefinition.values["intensity"] =
          Value{0.0,
                glm::dvec3(it->second[0].as<double>(),
                           it->second[1].as<double>(),
                           it->second[2].as<double>())};
      shapeDefinition.valueOrder.push_back("intensity");
    }
    else if (nextKey == "shadow")
    {
      shapeDefinition.values["shadow"] = it->second.as<bool>() ? Value{1.0} : Value{};
      shapeDefinition.valueOrder.push_back("shadow");
    }
    else
    {
      throw std::invalid_argument("invalid operator in add statement: " +
                                  nextKey);
    }
  }
}

void ObjectLoader::parseArgs(const YAML::Node &node, std::vector<Value> &args)
{
  for (auto &item : node)
  {
    if (item.IsSequence())
    {
      args.push_back(Value{0.0,
                           glm::dvec3(item[0].as<double>(),
                                      item[1].as<double>(),
                                      item[2].as<double>())});
    }
    else if (item.IsScalar())
    {
      args.push_back(Value{item.as<double>()});
    }
    else
    {
      throw std::invalid_argument("invalid arguement in shape definition");
    }
  }
}

void ObjectLoader::assignDefinition(std::shared_ptr<Shape> &shapePtr,
                                    Definition &definition)
{
  bool newPattern = false;
  if (definition.inheritFrom)
  {
    assignDefinition(shapePtr, *definition.inheritFrom);
  }

  std::shared_ptr<Material> newMaterial;

  if (!shapePtr->material)
    newMaterial = std::make_shared<Material>();
  else
    newMaterial = shapePtr->material;

  for (auto &value : definition.valueOrder)
  {
    if (value == "color")
    {
      newMaterial->colour = definition.values[value].vector;
      newPattern = true;
    }
    else if (value == "diffuse")
    {
      newMaterial->diffuse = definition.values[value].scalar;
      newPattern = true;
    }
    else if (value == "ambient")
    {
      newMaterial->ambient = definition.values[value].scalar;
      newPattern = true;
    }
    else if (value == "specular")
    {
      newMaterial->specular = definition.values[value].scalar;
      newPattern = true;
    }
    else if (value == "shininess")
    {
      newMaterial->shininess = definition.values[value].scalar;
      newPattern = true;
    }
    else if (value == "reflective")
    {
      newMaterial->reflective = definition.values[value].scalar;
      newPattern = true;
    }
    else if (value == "transparency")
    {
      newMaterial->transparency = definition.values[value].scalar;
      newPattern = true;
    }
    else if (value == "refractive-index")
    {
      newMaterial->refractiveIndex = definition.values[value].scalar;
      newPattern = true;
    }
    else if (value.substr(0, 9) == "translate")
    {
      glm::dmat4 translation =
          glm::translate(glm::dmat4(1.0), definition.values[value].vector);
      shapePtr->multiplyTransform(translation);
    }
    else if (value.substr(0, 5) == "scale")
    {
      glm::dmat4 scale =
          glm::scale(glm::dmat4(1.0), definition.values[value].vector);
      shapePtr->multiplyTransform(scale);
    }
    else if (value.substr(0, 8) == "rotate-x")
    {
      glm::dmat4 rotation =
          glm::rotate(glm::dmat4(1.0), definition.values[value].scalar,
                      glm::dvec3(1.0, 0.0, 0.0));
      shapePtr->multiplyTransform(rotation);
    }
    else if (value.substr(0, 8) == "rotate-y")
    {
      glm::dmat4 rotation =
          glm::rotate(glm::dmat4(1.0), definition.values[value].scalar,
                      glm::dvec3(0.0, 1.0, 0.0));
      shapePtr->multiplyTransform(rotation);
    }
    else if (value.substr(0, 8) == "rotate-z")
    {
      glm::dmat4 rotation =
          glm::rotate(glm::dmat4(1.0), definition.values[value].scalar,
                      glm::dvec3(0.0, 0.0, 1.0));
      shapePtr->multiplyTransform(rotation);
    }
    else
    {
      throw std::invalid_argument("invalid operator in value statement");
    }
  }
  if (definition.pattern)
  {
    newMaterial->setPattern(definition.pattern);
    newMaterial->pattern->calculateInverseTranform();
  }

  if (!shapePtr->material)
  {
    if (newPattern)
      shapePtr->setMaterial(newMaterial);
    else
      shapePtr->material = newMaterial;
  }

  shapePtr->calculateInverseTranform();
}

// TODO: add patterns
void ObjectLoader::addDefinition(const YAML::Node &definitionNode)
{
  // TODO this is bad, make properly polymorphic.
  std::shared_ptr<ShapeDefinition> newDefinitionShape = std::make_shared<ShapeDefinition>();
  std::shared_ptr<Definition> newDefinition = newDefinitionShape;

  std::string name;
  std::string inheritFromStr;

  for (YAML::const_iterator it = definitionNode.begin(); it != definitionNode.end(); ++it)
  {
    std::string nextKey = it->first.as<std::string>();
    if (nextKey == "define")
    {
      name = it->second.as<std::string>();
    }
    else if (nextKey == "extend")
    {
      inheritFromStr = it->second.as<std::string>();
    }
    else if (nextKey == "value")
    {
      if (it->second.IsMap())
      {
        if (it->second["add"])
        {
          parseShape(it->second, *newDefinitionShape);
        }
        else
        {
          parseMaterial(it->second, *newDefinition);
        }
      }
      else if (it->second.IsSequence())
      {
        parseTransform(it->second, *newDefinition);
      }

      else
      {
        throw std::invalid_argument("value must be map or sequence");
      }
    }
    else
    {
      throw std::invalid_argument("invalid operator in define statement");
    }
  }

  if (!inheritFromStr.empty())
    newDefinition->inheritFrom = definitions.at(inheritFromStr);
  definitions[name] = newDefinition;
}

void ObjectLoader::parseMaterial(const YAML::Node &node,
                                 Definition &definition)
{
  if (node.IsScalar())
  {
    definition.inheritFrom = definitions.at(node.as<std::string>());
  }
  else
  {
    for (YAML::const_iterator valueIt = node.begin(); valueIt != node.end();
         ++valueIt)
    {
      std::string valueKey = valueIt->first.as<std::string>();

      if (valueKey == "color")
      {
        definition.values["color"] =
            Value{0.0,
                  glm::dvec3(valueIt->second[0].as<double>(),
                             valueIt->second[1].as<double>(),
                             valueIt->second[2].as<double>())};
        definition.valueOrder.push_back("color");
      }
      else if (valueKey == "diffuse")
      {
        double diffuse = valueIt->second.as<double>();
        definition.values["diffuse"] = Value{diffuse};
        definition.valueOrder.push_back("diffuse");
      }
      else if (valueKey == "ambient")
      {
        double ambient = valueIt->second.as<double>();
        definition.values["ambient"] = Value{ambient};
        definition.valueOrder.push_back("ambient");
      }
      else if (valueKey == "specular")
      {
        double specular = valueIt->second.as<double>();
        definition.values["specular"] = Value{specular};
        definition.valueOrder.push_back("specular");
      }
      else if (valueKey == "shininess")
      {
        double shininess = valueIt->second.as<double>();
        definition.values["shininess"] = Value{shininess};
        definition.valueOrder.push_back("shininess");
      }
      else if (valueKey == "reflective")
      {
        double reflective = valueIt->second.as<double>();
        definition.values["reflective"] = Value{reflective};
        definition.valueOrder.push_back("reflective");
      }
      else if (valueKey == "transparency")
      {
        double transparency = valueIt->second.as<double>();
        definition.values["transparency"] = Value{transparency};
        definition.valueOrder.push_back("transparency");
      }
      else if (valueKey == "refractive-index")
      {
        double refractiveIndex = valueIt->second.as<double>();
        definition.values["refractive-index"] = Value{refractiveIndex};
        definition.valueOrder.push_back("refractive-index");
      }
      else if (valueKey == "pattern")
      {
        parsePattern(valueIt->second, definition);
      }
      else
      {
        throw std::invalid_argument("invalid operator in value statement");
      }
    }
  }
  definition.empty = false;
}

void ObjectLoader::parsePattern(const YAML::Node &node,
                                Definition &definition)
{
  std::string type;
  std::shared_ptr<Pattern> pattern;
  double perturbedCoeff = 0;

  std::shared_ptr<UVTexture> uvTexture;
  std::shared_ptr<TextureMap> textureMap;

  bool cubeMapping = false;

  bool blendedPattern = false;
  bool mappedPattern = false;
  for (YAML::const_iterator valueIt = node.begin(); valueIt != node.end();
       ++valueIt)
  {
    std::string valueKey = valueIt->first.as<std::string>();

    if (valueKey == "type")
    {
      type = valueKey;
      if (valueIt->second.as<std::string>() == "stripes")
      {
        pattern = std::make_unique<StripedPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                   glm::dvec3(0.0, 1.0, 0.0));
      }
      else if (valueIt->second.as<std::string>() == "gradient")
      {
        pattern = std::make_unique<GradientPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                    glm::dvec3(0.0, 1.0, 0.0));
      }
      else if (valueIt->second.as<std::string>() == "rings")
      {
        pattern = std::make_unique<RingPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                glm::dvec3(0.0, 1.0, 0.0));
      }
      else if (valueIt->second.as<std::string>() == "checkers")
      {
        pattern = std::make_unique<CheckedPattern>(glm::dvec3(1.0, 0.0, 0.0),
                                                   glm::dvec3(0.0, 1.0, 0.0));
      }
      else if (valueIt->second.as<std::string>() == "map")
      {
        mappedPattern = true;
      }
      else if (valueIt->second.as<std::string>() == "blended")
      {
        blendedPattern = true;
        break;
      }
      else
      {
        throw std::invalid_argument("invalid pattern type");
      }
    }
    else if (valueKey == "mapping")
    {
      std::string mappingType = valueIt->second.as<std::string>();

      if (mappingType == "spherical")
      {
        textureMap = std::make_shared<SphericalMap>();
      }
      else if (mappingType == "planar")
      {
        textureMap = std::make_shared<PlanarMap>();
      }
      else if (mappingType == "cylindrical")
      {
        textureMap = std::make_shared<CylinderMap>();
      }
      else if (mappingType == "cube")
      {
        textureMap = std::make_shared<CubeMap>();
        uvTexture = std::make_shared<ImageTexture>();
        cubeMapping = true;
      }
    }
    else if (valueKey == "uv_pattern")
    {
      if (cubeMapping)
      {
      }

      parseUV(valueIt->second, uvTexture);
    }
    else if (valueKey == "right")
    {
      parseUV(valueIt->second, uvTexture, 1);
    }
    else if (valueKey == "left")
    {
      parseUV(valueIt->second, uvTexture, 2);
    }
    else if (valueKey == "up")
    {
      parseUV(valueIt->second, uvTexture, 3);
    }
    else if (valueKey == "down")
    {
      parseUV(valueIt->second, uvTexture, 4);
    }
    else if (valueKey == "front")
    {
      parseUV(valueIt->second, uvTexture, 5);
    }
    else if (valueKey == "back")
    {
      parseUV(valueIt->second, uvTexture, 6);
    }

    else if (valueKey == "perturbed")
    {
      perturbedCoeff = valueIt->second.as<double>();
    }
    else if (valueKey == "colors")
    {
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
    }
    else if (valueKey == "transform")
    {
      for (auto &transformValue : valueIt->second)
      {
        if (transformValue[0].as<std::string>() == "translate")
        {
          glm::dvec3 vector(transformValue[1].as<double>(),
                            transformValue[2].as<double>(),
                            transformValue[3].as<double>());
          glm::dmat4 translation = glm::translate(glm::dmat4(1.0), vector);
          pattern->multiplyTransform(translation);
        }
        else if (transformValue[0].as<std::string>() == "scale")
        {
          glm::dvec3 vector(transformValue[1].as<double>(),
                            transformValue[2].as<double>(),
                            transformValue[3].as<double>());
          glm::dmat4 scale = glm::scale(glm::dmat4(1.0), vector);
          pattern->multiplyTransform(scale);
        }
        else if (transformValue[0].as<std::string>() == "rotate-x")
        {
          glm::dmat4 rotation =
              glm::rotate(glm::dmat4(1.0), transformValue[1].as<double>(),
                          glm::dvec3(1.0, 0.0, 0.0));
          pattern->multiplyTransform(rotation);
        }
        else if (transformValue[0].as<std::string>() == "rotate-y")
        {
          glm::dmat4 rotation =
              glm::rotate(glm::dmat4(1.0), transformValue[1].as<double>(),
                          glm::dvec3(0.0, 1.0, 0.0));
          pattern->multiplyTransform(rotation);
        }
        else if (transformValue[0].as<std::string>() == "rotate-z")
        {
          glm::dmat4 rotation =
              glm::rotate(glm::dmat4(1.0), transformValue[1].as<double>(),
                          glm::dvec3(0.0, 0.0, 1.0));
          pattern->multiplyTransform(rotation);
        }
      }
    }
    else
    {
      throw std::invalid_argument("invalid key in pattern definition");
    }
  }

  if (blendedPattern)
  {
    if (node["perturbed"])
      perturbedCoeff = node["perturbed"].as<double>();

    Definition patternADefinition;
    Definition patternBDefinition;

    parsePattern(node["patterns"][0], patternADefinition);
    parsePattern(node["patterns"][1], patternBDefinition);

    pattern = std::make_shared<BlendedPattern>(patternADefinition.pattern,
                                               patternBDefinition.pattern);
  }
  else if (mappedPattern)
  {
    pattern = std::make_unique<MappedPattern>(uvTexture, textureMap);
  }

  if (perturbedCoeff)
  {
    pattern = std::make_shared<PerturbedPattern>(pattern, perturbedCoeff);
  }
  definition.pattern = std::move(pattern);
}

void ObjectLoader::parseTransform(const YAML::Node &node,
                                  Definition &definition)
{
  if (node.IsScalar())
  {
    definition.inheritFrom = definitions.at(node.as<std::string>());
  }
  else if (node.IsSequence())
  {
    for (auto &item : node)
    {
      if (item.IsScalar())
      {
        definition.inheritFrom = definitions.at(item.as<std::string>());
      }
      else
      {
        std::string newValueName = item[0].as<std::string>() + std::to_string(definition.transformCounts.at(item[0].as<std::string>()));
        if (item[0].as<std::string>().substr(0, 6) == "rotate")
        {
          definition.values[newValueName] = Value{item[1].as<double>()};
          definition.valueOrder.push_back(newValueName);
        }
        else
        {
          definition.values[newValueName] =
              Value{0.0,
                    glm::dvec3(item[1].as<double>(),
                               item[2].as<double>(),
                               item[3].as<double>())};
          definition.valueOrder.push_back(newValueName);

          // definition.transformCounts.at(item[0].as<std::string>())
        }
        definition.transformCounts.at(item[0].as<std::string>())++;
      }
    }
  }
  else
  {
    throw std::invalid_argument(
        "value as sequence must contain either extension name or sequence");
  }
  definition.empty = false;
}

void ObjectLoader::parseUV(const YAML::Node &node, std::shared_ptr<UVTexture> &uvTexture, int face)
{
  std::string uvPatternType;
  std::unordered_map<std::string, Value> values;
  std::string imageFileName;

  for (YAML::const_iterator uvIt = node.begin(); uvIt != node.end();
       ++uvIt)
  {
    std::string uvKey = uvIt->first.as<std::string>();

    if (uvKey == "type")
    {
      uvPatternType = uvIt->second.as<std::string>();
    }
    else if (uvKey == "file")
    {
      imageFileName = uvIt->second.as<std::string>();
    }
    else if (uvKey == "width")
    {
      values["width"].scalar = uvIt->second.as<int>();
    }
    else if (uvKey == "height")
    {
      values["height"].scalar = uvIt->second.as<int>();
    }
    else if (uvKey == "colors")
    {
      values["colourA"].vector = glm::dvec3(uvIt->second[0][0].as<double>(),
                                            uvIt->second[0][1].as<double>(),
                                            uvIt->second[0][2].as<double>());
      values["colourB"].vector = glm::dvec3(uvIt->second[1][0].as<double>(),
                                            uvIt->second[1][1].as<double>(),
                                            uvIt->second[1][2].as<double>());
    }
  }

  if (uvPatternType == "checkers")
  {
    uvTexture = std::make_shared<CheckeredTexture>(values["colourA"].vector, values["colourB"].vector, values["width"].scalar, values["height"].scalar);
  }
  else if (uvPatternType == "image")
  {
    if (!face)
      uvTexture = std::make_shared<ImageTexture>(imageFileName);
    else
    {
      if (face == 1)
        uvTexture->loadRight(imageFileName);
      else if (face == 2)
        uvTexture->loadLeft(imageFileName);
      else if (face == 3)
        uvTexture->loadUp(imageFileName);
      else if (face == 4)
        uvTexture->loadDown(imageFileName);
      else if (face == 5)
        uvTexture->loadFront(imageFileName);
      else if (face == 6)
        uvTexture->loadBack(imageFileName);
    }
  }
}