#include "objectLoader.h"

ObjectLoader::ObjectLoader(/* args */)
{
}

ObjectLoader::~ObjectLoader()
{
}

std::vector<std::unique_ptr<Shape>> ObjectLoader::loadYaml(std::string &fileName)
{
    std::vector<std::unique_ptr<Shape>> ret;

    YAML::Node root = YAML::LoadFile(fileName);
    for (YAML::const_iterator it = root.begin(); it != root.end(); ++it)
    {
        if (it->first.as<std::string>() == "add")
        {
            YAML::Node n = it->second;
            ret.push_back(addShape(n));
        }
        else if (it->first.as<std::string>() == "define")
        {
            addDefinition(*it);
        }
        else
        {
            throw std::invalid_argument("invalid operator in yaml file");
        }
    }

    return ret;
}

std::unique_ptr<Shape> ObjectLoader::addShape(const YAML::Node &shapeNode)
{
    std::string shapeType;
    Definition materialDefinition;
    Definition transformDefinition;
    Definition cameraDefinition;
    Definition lightDefinition;
    std::unique_ptr<Shape> ret;

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
            cameraDefinition.scalarValues["width"] = it->second.as<float>();
        }
        else if (nextKey == "height")
        {
            cameraDefinition.scalarValues["height"] = it->second.as<float>();
        }
        else if (nextKey == "field-of-view")
        {
            cameraDefinition.scalarValues["field-of-view"] = it->second.as<float>();
        }
        else if (nextKey == "from")
        {
            glm::vec3 from(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>());
            cameraDefinition.vectorValues["from"] = from;
        }
        else if (nextKey == "to")
        {
            glm::vec3 to(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>());
            cameraDefinition.vectorValues["to"] = to;
        }
        else if (nextKey == "up")
        {
            glm::vec3 up(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>());
            cameraDefinition.vectorValues["up"] = up;
        }
        else if (nextKey == "at")
        {
            glm::vec3 at(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>());
            lightDefinition.vectorValues["at"] = at;
        }
        else if (nextKey == "intensity")
        {
            glm::vec3 intensity(it->second[0].as<float>(), it->second[1].as<float>(), it->second[2].as<float>());
            lightDefinition.vectorValues["intensity"] = intensity;
        }
        else
        {
            throw std::invalid_argument("invalid operator in add statement");
        }
    }

    if (shapeType == "sphere")
    {
        ret = std::make_unique<Sphere>(1.f, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
    }
    else if (shapeType == "plane")
    {
        ret = std::make_unique<Plane>(1.f, glm::vec4(0.f, 0.f, 0.f, 1.f));
    }
    else if (shapeType == "camera")
    {
        ret = std::make_unique<Camera>(0,
                                       glm::vec4(cameraDefinition.vectorValues["from"], 1.f),
                                       glm::vec4(cameraDefinition.vectorValues["to"], 1.f),
                                       glm::vec4(cameraDefinition.vectorValues["up"], 0.f),
                                       cameraDefinition.scalarValues["height"],
                                       cameraDefinition.scalarValues["width"],
                                       cameraDefinition.scalarValues["field-of-view"]);
    }
    else if (shapeType == "light")
    {
        ret = std::make_unique<PointLight>(0,
                                           glm::vec4(lightDefinition.vectorValues["at"], 1.f),
                                           lightDefinition.vectorValues["intensity"]);
    }
    else
    {
        throw std::invalid_argument("this shape is not implemented");
    }

    if (!materialDefinition.name.empty())
        assignDefinition(ret, materialDefinition);
    if (!transformDefinition.name.empty())
        assignDefinition(ret, transformDefinition);

    return ret;
}

void ObjectLoader::assignDefinition(std::unique_ptr<Shape> &shapePtr, Definition &definition)
{
    if (definition.inheritFrom)
    {
        assignDefinition(shapePtr, *definition.inheritFrom);
    }
    for (auto &scalar : definition.scalarValues)
    {
        if (scalar.first == "diffuse")
        {
            shapePtr->material->diffuse = scalar.second;
        }
        else if (scalar.first == "ambient")
        {
            shapePtr->material->ambient = scalar.second;
        }
        else if (scalar.first == "specular")
        {
            shapePtr->material->specular = scalar.second;
        }
        else if (scalar.first == "shininess")
        {
            shapePtr->material->shininess = scalar.second;
        }
        else if (scalar.first == "reflective")
        {
            shapePtr->material->reflective = scalar.second;
        }
        else if (scalar.first == "transparency")
        {
            shapePtr->material->transparency = scalar.second;
        }
        else if (scalar.first == "refractive-index")
        {
            shapePtr->material->refractiveIndex = scalar.second;
        }
        else if (scalar.first == "rotate-x")
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(), scalar.second, glm::vec3(1.f, 0.f, 0.f));
            shapePtr->transform *= rotation;
        }
        else if (scalar.first == "rotate-y")
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(), scalar.second, glm::vec3(0.f, 1.f, 0.f));
            shapePtr->transform *= rotation;
        }
        else if (scalar.first == "rotate-z")
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(), scalar.second, glm::vec3(0.f, 0.f, 1.f));
            shapePtr->transform *= rotation;
        }
        else
        {
            throw std::invalid_argument("invalid operator in value statement");
        }
    }

    for (auto &vector : definition.vectorValues)
    {
        if (vector.first == "color")
        {
            shapePtr->material->colour = vector.second;
        }
        else if (vector.first == "translate")
        {
            glm::mat4 translation = glm::translate(glm::mat4(), vector.second);
            shapePtr->transform *= translation;
        }
        else if (vector.first == "scale")
        {
            glm::mat4 scale = glm::scale(glm::mat4(), vector.second);
            shapePtr->transform *= scale;
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
    for (YAML::const_iterator valueIt = node.begin(); valueIt != node.end(); ++valueIt)
    {
        if (valueIt->IsScalar())
        {
            definition.inheritFrom = definitions.at(valueIt->as<std::string>());
        }
        else if (valueIt->IsMap())
        {
            std::string valueKey = valueIt->first.as<std::string>();

            if (valueKey == "color")
            {
                glm::vec3 color(valueIt->second[0].as<float>(), valueIt->second[1].as<float>(), valueIt->second[2].as<float>());
                definition.vectorValues["color"] = color;
            }
            else if (valueKey == "diffuse")
            {
                float diffuse = valueIt->second.as<float>();
                definition.scalarValues["diffuse"] = diffuse;
            }
            else if (valueKey == "ambient")
            {
                float ambient = valueIt->second.as<float>();
                definition.scalarValues["ambient"] = ambient;
            }
            else if (valueKey == "specular")
            {
                float specular = valueIt->second.as<float>();
                definition.scalarValues["specular"] = specular;
            }
            else if (valueKey == "shininess")
            {
                float shininess = valueIt->second.as<float>();
                definition.scalarValues["shininess"] = shininess;
            }
            else if (valueKey == "reflective")
            {
                float reflective = valueIt->second.as<float>();
                definition.scalarValues["reflective"] = reflective;
            }
            else if (valueKey == "transparency")
            {
                float transparency = valueIt->second.as<float>();
                definition.scalarValues["transparency"] = transparency;
            }
            else if (valueKey == "refractive-index")
            {
                float refractiveIndex = valueIt->second.as<float>();
                definition.scalarValues["refractiveIndex"] = refractiveIndex;
            }
            else
            {
                throw std::invalid_argument("invalid operator in value statement");
            }
        }
    }
}

void ObjectLoader::parseTransform(const YAML::Node &node, Definition &definition)
{
    for (auto &item : node)
    {
        if (item.IsScalar())
        {
            definition.inheritFrom = definitions.at(item.as<std::string>());
        }
        else if (item.IsSequence())
        {
            if (item[0].as<std::string>().substr(0, 7) == "rotate")
            {
                definition.scalarValues[item[0].as<std::string>()] = item[1].as<float>();
            }
            else
            {
                glm::vec3 vec(item[1].as<float>(), item[2].as<float>(), item[3].as<float>());
                definition.vectorValues[item[0].as<std::string>()] = vec;
            }
        }
        else
        {
            throw std::invalid_argument("value as sequence must contain either extension name or sequence");
        }
    }
}