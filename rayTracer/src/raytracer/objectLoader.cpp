#include "objectLoader.h"

ObjectLoader::ObjectLoader(/* args */) {}

ObjectLoader::~ObjectLoader() {}

std::pair<std::shared_ptr<Camera>, std::shared_ptr<World>>
ObjectLoader::loadYaml(const std::string &fileName)
{
  // std::vector<std::shared_ptr<Shape>> shapes;
  std::shared_ptr<Camera> camera;
  std::shared_ptr<World> world = std::make_shared<World>();

  // std::vector<std::shared_ptr<Shape>> shapes;

  // world->shapes.clear();
  // world->lights.clear();

  YAML::Node root;

#ifdef __EMSCRIPTEN__
  if (std::__fs::filesystem::exists(fileName))
#else
  if (std::filesystem::exists(fileName))
#endif
  {
    // std::cout << "Found found: " << fileName << std::endl;
    root = YAML::LoadFile(fileName);
  }
  else
  {
    // std::cout << "File not found: " << fileName << std::endl;
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
        // shapes.push_back(shape);
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

  // std::shared_ptr<Shape> bvh = Model::buildBoundingVolumeHierarchy(shapes);
  // std::shared_ptr<Material> bvhMaterial = std::make_shared<Material>();
  // bvhMaterial->shadow = false;
  // bvh->setMaterial(bvhMaterial);
  // world->addShape(bvh);

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
          std::abs(shapeDefinition.args.at(2).scalar) > std::numeric_limits<Float>::epsilon()));
  }
  else if (shapeDefinition.shapeType == "cone")
  {
    if (shapeDefinition.args.empty())
      ret = std::make_shared<Cone>();
    else
      ret = std::make_shared<Cone>(Cone(
          shapeDefinition.args.at(0).scalar, shapeDefinition.args.at(1).scalar,
          std::abs(shapeDefinition.args.at(2).scalar) > std::numeric_limits<Float>::epsilon()));
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
#ifdef __EMSCRIPTEN__
    if (!std::__fs::filesystem::exists(shapeDefinition.filePath))
      downloadAsset(shapeDefinition.filePath, shapeDefinition.filePath);
#endif
    std::shared_ptr<Model> model = std::make_shared<Model>();
    model->build(shapeDefinition.filePath);
    ret = model;
  }
  else if (shapeDefinition.shapeType == "camera")
  {
    ret = std::make_shared<Camera>(
        Vec4(shapeDefinition.values["from"].vector, 1.0),
        Vec4(shapeDefinition.values["to"].vector, 1.0),
        Vec4(shapeDefinition.values["up"].vector, 0.0),
        shapeDefinition.values["width"].scalar,
        shapeDefinition.values["height"].scalar,
        shapeDefinition.values["field-of-view"].scalar);
  }
  else if (shapeDefinition.shapeType == "light")
  {
    ret = std::make_shared<PointLight>(
        Vec4(shapeDefinition.values["at"].vector, 1.0),
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
      shapeDefinition.values["width"] = Value{it->second.as<Float>()};
      shapeDefinition.valueOrder.push_back("width");
    }
    else if (nextKey == "height")
    {
      shapeDefinition.values["height"] = Value{it->second.as<Float>()};
      shapeDefinition.valueOrder.push_back("height");
    }
    else if (nextKey == "field-of-view")
    {
      shapeDefinition.values["field-of-view"] =
          Value{it->second.as<Float>()};
      shapeDefinition.valueOrder.push_back("field-of-view");
    }
    else if (nextKey == "from")
    {
      shapeDefinition.values["from"] =
          Value{0.0,
                Vec3(it->second[0].as<Float>(),
                     it->second[1].as<Float>(),
                     it->second[2].as<Float>())};
      shapeDefinition.valueOrder.push_back("from");
    }
    else if (nextKey == "to")
    {
      shapeDefinition.values["to"] =
          Value{0.0,
                Vec3(it->second[0].as<Float>(),
                     it->second[1].as<Float>(),
                     it->second[2].as<Float>())};
      shapeDefinition.valueOrder.push_back("to");
    }
    else if (nextKey == "up")
    {
      shapeDefinition.values["up"] =
          Value{0.0,
                Vec3(it->second[0].as<Float>(),
                     it->second[1].as<Float>(),
                     it->second[2].as<Float>())};
      shapeDefinition.valueOrder.push_back("up");
    }
    else if (nextKey == "at")
    {
      shapeDefinition.values["at"] =
          Value{0.0,
                Vec3(it->second[0].as<Float>(),
                     it->second[1].as<Float>(),
                     it->second[2].as<Float>())};
      shapeDefinition.valueOrder.push_back("at");
    }
    else if (nextKey == "intensity")
    {
      shapeDefinition.values["intensity"] =
          Value{0.0,
                Vec3(it->second[0].as<Float>(),
                     it->second[1].as<Float>(),
                     it->second[2].as<Float>())};
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
                           Vec3(item[0].as<Float>(),
                                item[1].as<Float>(),
                                item[2].as<Float>())});
    }
    else if (item.IsScalar())
    {
      args.push_back(Value{item.as<Float>()});
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

  std::vector<Mat4> transforms;

  for (auto &value : definition.valueOrder)
  {
    if (value == "color")
    {
      newMaterial->colour = definition.values[value].vector;
      newPattern = true;
    }
    else if (value == "emissiveness")
    {
      newMaterial->emissiveness = definition.values[value].vector;
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
      Mat4 translation =
          glm::translate(Mat4(1.0), definition.values[value].vector);
      transforms.push_back(translation);
      // shapePtr->multiplyTransform(translation);
    }
    else if (value.substr(0, 5) == "scale")
    {
      Mat4 scale =
          glm::scale(Mat4(1.0), definition.values[value].vector);
      transforms.push_back(scale);
      // shapePtr->multiplyTransform(scale);
    }
    else if (value.substr(0, 8) == "rotate-x")
    {
      Mat4 rotation =
          glm::rotate(Mat4(1.0), definition.values[value].scalar,
                      Vec3(1.0, 0.0, 0.0));
      transforms.push_back(rotation);
      // shapePtr->multiplyTransform(rotation);
    }
    else if (value.substr(0, 8) == "rotate-y")
    {
      Mat4 rotation =
          glm::rotate(Mat4(1.0), definition.values[value].scalar,
                      Vec3(0.0, 1.0, 0.0));
      transforms.push_back(rotation);
      // shapePtr->multiplyTransform(rotation);
    }
    else if (value.substr(0, 8) == "rotate-z")
    {
      Mat4 rotation =
          glm::rotate(Mat4(1.0), definition.values[value].scalar,
                      Vec3(0.0, 0.0, 1.0));
      transforms.push_back(rotation);
      // shapePtr->multiplyTransform(rotation);
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
    // else
    //   shapePtr->material = newMaterial;
  }

  shapePtr->calculateInverseTranform(transforms);
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
                  Vec3(valueIt->second[0].as<Float>(),
                       valueIt->second[1].as<Float>(),
                       valueIt->second[2].as<Float>())};
        definition.valueOrder.push_back("color");
      }
      else if (valueKey == "emissiveness")
      {
        definition.values["emissiveness"] =
            Value{0.0,
                  Vec3(valueIt->second[0].as<Float>(),
                       valueIt->second[1].as<Float>(),
                       valueIt->second[2].as<Float>())};
        definition.valueOrder.push_back("emissiveness");
      }
      else if (valueKey == "diffuse")
      {
        Float diffuse = valueIt->second.as<Float>();
        definition.values["diffuse"] = Value{diffuse};
        definition.valueOrder.push_back("diffuse");
      }
      else if (valueKey == "ambient")
      {
        Float ambient = valueIt->second.as<Float>();
        definition.values["ambient"] = Value{ambient};
        definition.valueOrder.push_back("ambient");
      }
      else if (valueKey == "specular")
      {
        Float specular = valueIt->second.as<Float>();
        definition.values["specular"] = Value{specular};
        definition.valueOrder.push_back("specular");
      }
      else if (valueKey == "shininess")
      {
        Float shininess = valueIt->second.as<Float>();
        definition.values["shininess"] = Value{shininess};
        definition.valueOrder.push_back("shininess");
      }
      else if (valueKey == "reflective")
      {
        Float reflective = valueIt->second.as<Float>();
        definition.values["reflective"] = Value{reflective};
        definition.valueOrder.push_back("reflective");
      }
      else if (valueKey == "transparency")
      {
        Float transparency = valueIt->second.as<Float>();
        definition.values["transparency"] = Value{transparency};
        definition.valueOrder.push_back("transparency");
      }
      else if (valueKey == "refractive-index")
      {
        Float refractiveIndex = valueIt->second.as<Float>();
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
  Float perturbedCoeff = 0;

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
        pattern = std::make_unique<StripedPattern>(Vec3(1.0, 0.0, 0.0),
                                                   Vec3(0.0, 1.0, 0.0));
      }
      else if (valueIt->second.as<std::string>() == "gradient")
      {
        pattern = std::make_unique<GradientPattern>(Vec3(1.0, 0.0, 0.0),
                                                    Vec3(0.0, 1.0, 0.0));
      }
      else if (valueIt->second.as<std::string>() == "rings")
      {
        pattern = std::make_unique<RingPattern>(Vec3(1.0, 0.0, 0.0),
                                                Vec3(0.0, 1.0, 0.0));
      }
      else if (valueIt->second.as<std::string>() == "checkers")
      {
        pattern = std::make_unique<CheckedPattern>(Vec3(1.0, 0.0, 0.0),
                                                   Vec3(0.0, 1.0, 0.0));
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
      perturbedCoeff = valueIt->second.as<Float>();
    }
    else if (valueKey == "colors")
    {
      if (type == "blended")
        throw std::invalid_argument("invalid arguement for blended pattern");
      if (type == "perturbed")
        throw std::invalid_argument("invalid arguement for perturbed pattern");

      ColourPattern *colourPattern =
          dynamic_cast<ColourPattern *>(pattern.get());

      Vec3 colourA(valueIt->second[0][0].as<Float>(),
                   valueIt->second[0][1].as<Float>(),
                   valueIt->second[0][2].as<Float>());
      Vec3 colourB(valueIt->second[1][0].as<Float>(),
                   valueIt->second[1][1].as<Float>(),
                   valueIt->second[1][2].as<Float>());

      colourPattern->setColour(colourA, 0);
      colourPattern->setColour(colourB, 1);
    }
    else if (valueKey == "transform")
    {
      for (auto &transformValue : valueIt->second)
      {
        if (transformValue[0].as<std::string>() == "translate")
        {
          Vec3 vector(transformValue[1].as<Float>(),
                      transformValue[2].as<Float>(),
                      transformValue[3].as<Float>());
          Mat4 translation = glm::translate(Mat4(1.0), vector);
          pattern->multiplyTransform(translation);
        }
        else if (transformValue[0].as<std::string>() == "scale")
        {
          Vec3 vector(transformValue[1].as<Float>(),
                      transformValue[2].as<Float>(),
                      transformValue[3].as<Float>());
          Mat4 scale = glm::scale(Mat4(1.0), vector);
          pattern->multiplyTransform(scale);
        }
        else if (transformValue[0].as<std::string>() == "rotate-x")
        {
          Mat4 rotation =
              glm::rotate(Mat4(1.0), transformValue[1].as<Float>(),
                          Vec3(1.0, 0.0, 0.0));
          pattern->multiplyTransform(rotation);
        }
        else if (transformValue[0].as<std::string>() == "rotate-y")
        {
          Mat4 rotation =
              glm::rotate(Mat4(1.0), transformValue[1].as<Float>(),
                          Vec3(0.0, 1.0, 0.0));
          pattern->multiplyTransform(rotation);
        }
        else if (transformValue[0].as<std::string>() == "rotate-z")
        {
          Mat4 rotation =
              glm::rotate(Mat4(1.0), transformValue[1].as<Float>(),
                          Vec3(0.0, 0.0, 1.0));
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
      perturbedCoeff = node["perturbed"].as<Float>();

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
          definition.values[newValueName] = Value{item[1].as<Float>()};
          definition.valueOrder.push_back(newValueName);
        }
        else
        {
          definition.values[newValueName] =
              Value{0.0,
                    Vec3(item[1].as<Float>(),
                         item[2].as<Float>(),
                         item[3].as<Float>())};
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
      values["colourA"].vector = Vec3(uvIt->second[0][0].as<Float>(),
                                      uvIt->second[0][1].as<Float>(),
                                      uvIt->second[0][2].as<Float>());
      values["colourB"].vector = Vec3(uvIt->second[1][0].as<Float>(),
                                      uvIt->second[1][1].as<Float>(),
                                      uvIt->second[1][2].as<Float>());
    }
  }

  if (uvPatternType == "checkers")
  {
    uvTexture = std::make_shared<CheckeredTexture>(values["colourA"].vector, values["colourB"].vector, values["width"].scalar, values["height"].scalar);
  }
  else if (uvPatternType == "image")
  {
#ifdef __EMSCRIPTEN__
    if (!std::__fs::filesystem::exists(imageFileName))
      downloadAsset(imageFileName, imageFileName);
#endif

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

#ifdef __EMSCRIPTEN__
void ObjectLoader::downloadSucceeded(emscripten_fetch_t *fetch)
{
  printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);

  std::__fs::filesystem::path p(fetch->url);
  std::__fs::filesystem::path dir = p.parent_path();

  if (!std::__fs::filesystem::exists(dir))
    std::__fs::filesystem::create_directories(dir);

  std::ofstream outfile(fetch->url, std::ios::out);
  outfile.write(fetch->data, fetch->numBytes);
  // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
  emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void ObjectLoader::downloadFailed(emscripten_fetch_t *fetch)
{
  printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
  emscripten_fetch_close(fetch); // Also free data on failure.
}

void ObjectLoader::downloadAsset(const std::string &assetPath, const std::string &targetPath)
{
  std::cout << "download asset called.\n";
  emscripten_fetch_attr_t attr;
  emscripten_fetch_attr_init(&attr);
  strcpy(attr.requestMethod, "GET");
  attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_SYNCHRONOUS | EMSCRIPTEN_FETCH_REPLACE;
  attr.onsuccess = downloadSucceeded;
  attr.destinationPath = targetPath.c_str();
  attr.onerror = downloadFailed;
  emscripten_fetch(&attr, assetPath.c_str());
  std::cout << "download asset completed.\n";
}
#endif