#include "objectLoader.h"

ObjectLoader::ObjectLoader(/* args */)
{
}

ObjectLoader::~ObjectLoader()
{
}

std::pair<std::shared_ptr<Camera>, std::vector<std::shared_ptr<Shape>>> ObjectLoader::loadYaml(const std::string &fileName)
{
    std::vector<std::shared_ptr<Shape>> ret;
    std::shared_ptr<Camera> camera;

    YAML::Node root = YAML::LoadFile(fileName);
    for (YAML::const_iterator it = root.begin(); it != root.end(); ++it)
    {
        assert((*it).Type() == YAML::NodeType::Map);
        if (it->begin()->first.as<std::string>() == "add")
        {
            if (it->begin()->second.as<std::string>() == "camera")
            {
                camera = std::dynamic_pointer_cast<Camera>(addShape(*it));
            }
            else
            {
                ret.push_back(addShape(*it));
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

    return std::pair<std::shared_ptr<Camera>, std::vector<std::shared_ptr<Shape>>>(camera, ret);
}

std::shared_ptr<Shape> ObjectLoader::addShape(const YAML::Node &shapeNode)
{
    std::string shapeType;
    Definition materialDefinition;
    Definition transformDefinition;
    Definition cameraDefinition;
    Definition lightDefinition;
    std::shared_ptr<Shape> ret;

    for (YAML::const_iterator it = shapeNode.begin(); it != shapeNode.end(); ++it)
    {
        std::string nextKey = it->first.as<std::string>();

        if (nextKey == "add")
        {
            shapeType = it->second.as<std::string>();
        }
        else if (nextKey == "material")
        {
            parseMaterial(it->second, materialDefinition);
        }
        else if (nextKey == "transform")
        {
            parseTransform(it->second, transformDefinition);
        }
        else if (nextKey == "width")
        {
            cameraDefinition.values["width"] = Value{true, it->second.as<float>()};
        }
        else if (nextKey == "height")
        {
            cameraDefinition.values["height"] = Value{true, it->second.as<float>()};
        }
        else if (nextKey == "field-of-view")
        {
            cameraDefinition.values["field-of-view"] = Value{true, it->second.as<float>()};
        }
        else if (nextKey == "from")
        {
            cameraDefinition.values["from"] = Value{false, 0.f, std::make_unique<glm::vec3>(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>())};
        }
        else if (nextKey == "to")
        {
            cameraDefinition.values["to"] = Value{false, 0.f, std::make_unique<glm::vec3>(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>())};
        }
        else if (nextKey == "up")
        {
            cameraDefinition.values["up"] = Value{false, 0.f, std::make_unique<glm::vec3>(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>())};
        }
        else if (nextKey == "at")
        {
            lightDefinition.values["at"] = Value{false, 0.f, std::make_unique<glm::vec3>(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>())};
        }
        else if (nextKey == "intensity")
        {
            lightDefinition.values["intensity"] = Value{false, 0.f, std::make_unique<glm::vec3>(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>())};
        }
        else
        {
            throw std::invalid_argument("invalid operator in add statement");
        }
    }

    if (shapeType == "sphere")
    {
        ret = std::make_shared<Sphere>(1.f, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    }
    else if (shapeType == "plane")
    {
        ret = std::make_shared<Plane>(1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
    }
    else if (shapeType == "cube")
    {
        ret = std::make_shared<Cube>(1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
    }
    else if (shapeType == "camera")
    {
        ret = std::make_shared<Camera>(0,
                                       glm::vec4(*cameraDefinition.values["from"].vector, 1.f),
                                       glm::vec4(*cameraDefinition.values["to"].vector, 1.f),
                                       glm::vec4(*cameraDefinition.values["up"].vector, 0.f),
                                       cameraDefinition.values["width"].scalar,
                                       cameraDefinition.values["height"].scalar,
                                       cameraDefinition.values["field-of-view"].scalar);
    }
    else if (shapeType == "light")
    {
        ret = std::make_shared<PointLight>(0,
                                           glm::vec4(*lightDefinition.values["at"].vector, 1.f),
                                           *lightDefinition.values["intensity"].vector);
    }
    else
    {
        throw std::invalid_argument("this shape is not implemented");
    }

    if (!materialDefinition.empty)
        assignDefinition(ret, materialDefinition);
    if (!transformDefinition.empty)
        assignDefinition(ret, transformDefinition);

    return ret;
}

void ObjectLoader::assignDefinition(std::shared_ptr<Shape> &shapePtr, Definition &definition)
{
    if (!shapePtr->material)
        shapePtr->material = std::make_shared<Material>();

    if (definition.inheritFrom)
    {
        assignDefinition(shapePtr, *definition.inheritFrom);
    }
    for (auto &value : definition.values)
    {
        if (value.first == "color")
        {
            shapePtr->material->colour = *value.second.vector;
        }
        else if (value.first == "translate")
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.f), *value.second.vector);
            shapePtr->transform *= translation;
        }
        else if (value.first == "scale")
        {
            glm::mat4 scale = glm::scale(glm::mat4(1.f), *value.second.vector);
            shapePtr->transform *= scale;
        }
        else if (value.first == "diffuse")
        {
            shapePtr->material->diffuse = value.second.scalar;
        }
        else if (value.first == "ambient")
        {
            shapePtr->material->ambient = value.second.scalar;
        }
        else if (value.first == "specular")
        {
            shapePtr->material->specular = value.second.scalar;
        }
        else if (value.first == "shininess")
        {
            shapePtr->material->shininess = value.second.scalar;
        }
        else if (value.first == "reflective")
        {
            shapePtr->material->reflective = value.second.scalar;
        }
        else if (value.first == "transparency")
        {
            shapePtr->material->transparency = value.second.scalar;
        }
        else if (value.first == "refractive-index")
        {
            shapePtr->material->refractiveIndex = value.second.scalar;
        }
        else if (value.first == "rotate-x")
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.f), value.second.scalar, glm::vec3(1.f, 0.f, 0.f));
            shapePtr->transform *= rotation;
        }
        else if (value.first == "rotate-y")
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.f), value.second.scalar, glm::vec3(0.f, 1.f, 0.f));
            shapePtr->transform *= rotation;
        }
        else if (value.first == "rotate-z")
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.f), value.second.scalar, glm::vec3(0.f, 0.f, 1.f));
            shapePtr->transform *= rotation;
        }
        else
        {
            throw std::invalid_argument("invalid operator in value statement");
        }
    }
}

// TODO: add patterns
void ObjectLoader::addDefinition(const YAML::Node &definitionNode)
{
    std::shared_ptr<Definition> newDefinition = std::make_shared<Definition>();
    std::string name;
    for (YAML::const_iterator it = definitionNode.begin(); it != definitionNode.end(); ++it)
    {
        std::string nextKey = it->first.as<std::string>();
        if (nextKey == "define")
        {
            name = it->second.as<std::string>();
        }
        else if (nextKey == "extend")
        {
            newDefinition->inheritFrom = definitions.at(it->second.as<std::string>());
        }
        else if (nextKey == "value")
        {
            if (it->second.IsMap())
            {
                parseMaterial(it->second, *newDefinition);
            }
            else if (it->second.IsSequence())
            {
                parseTransform(it->second, *newDefinition);
            }
            else
            {
                throw std::invalid_argument("value must be sequence or map");
            }
        }
        else
        {
            throw std::invalid_argument("invalid operator in define statement");
        }
    }

    definitions[name] = newDefinition;
}

void ObjectLoader::parseMaterial(const YAML::Node &node, Definition &definition)
{
    if (node.IsScalar())
    {
        definition.inheritFrom = definitions.at(node.as<std::string>());
    }
    else
    {
        for (YAML::const_iterator valueIt = node.begin(); valueIt != node.end(); ++valueIt)
        {
            std::string valueKey = valueIt->first.as<std::string>();

            if (valueKey == "color")
            {

                definition.values["color"] = Value{false, 0.f, std::make_unique<glm::vec3>(valueIt->second[0].as<float>(), valueIt->second[1].as<float>(), valueIt->second[2].as<float>())};
            }
            else if (valueKey == "diffuse")
            {
                float diffuse = valueIt->second.as<float>();
                definition.values["diffuse"] = Value{true, diffuse};
            }
            else if (valueKey == "ambient")
            {
                float ambient = valueIt->second.as<float>();
                definition.values["ambient"] = Value{true, ambient};
            }
            else if (valueKey == "specular")
            {
                float specular = valueIt->second.as<float>();
                definition.values["specular"] = Value{true, specular};
            }
            else if (valueKey == "shininess")
            {
                float shininess = valueIt->second.as<float>();
                definition.values["shininess"] = Value{true, shininess};
            }
            else if (valueKey == "reflective")
            {
                float reflective = valueIt->second.as<float>();
                definition.values["reflective"] = Value{true, reflective};
            }
            else if (valueKey == "transparency")
            {
                float transparency = valueIt->second.as<float>();
                definition.values["transparency"] = Value{true, transparency};
            }
            else if (valueKey == "refractive-index")
            {
                float refractiveIndex = valueIt->second.as<float>();
                definition.values["refractiveIndex"] = Value{true, refractiveIndex};
            }
            else
            {
                throw std::invalid_argument("invalid operator in value statement");
            }
        }
    }
    definition.empty = false;
}

void ObjectLoader::parseTransform(const YAML::Node &node, Definition &definition)
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
                if (item[0].as<std::string>().substr(0, 6) == "rotate")
                {
                    definition.values[item[0].as<std::string>()] = Value{true, item[1].as<float>()};
                }
                else
                {
                    definition.values[item[0].as<std::string>()] = Value{false, 0.f, std::make_unique<glm::vec3>(item[1].as<float>(), item[2].as<float>(), item[3].as<float>())};
                }
            }
        }
    }
    else
    {
        throw std::invalid_argument("value as sequence must contain either extension name or sequence");
    }
    definition.empty = false;
}