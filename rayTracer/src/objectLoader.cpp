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
    Definition tempDefinition;
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
            parseMaterial(it->second, tempDefinition);
        }
        else if (nextKey == "transform")
        {
            parseTransform(it->second, tempDefinition);
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
    else
    {
        throw std::invalid_argument("this shape is not implemented");
    }

    assignDefinition(ret, tempDefinition);

    return ret;
}

void ObjectLoader::assignDefinition(std::unique_ptr<Shape> &shapePtr, Definition &definition)
{
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
            glm::vec3 vec(item[1].as<float>(), item[2].as<float>(), item[3].as<float>());
            definition.vectorValues[item[0].as<std::string>()] = vec;
        }
        else
        {
            throw std::invalid_argument("value as sequence must contain either extension name or sequence");
        }
    }
}