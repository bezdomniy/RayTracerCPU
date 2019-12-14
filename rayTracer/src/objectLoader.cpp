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
            ret.push_back(add(n));
        }
        else if (it->first.as<std::string>() == "define")
        {
        }
        else
        {
            throw std::invalid_argument("invalid operator in yaml file");
        }
    }

    return ret;
}

std::unique_ptr<Shape> ObjectLoader::add(YAML::Node &shapeNode)
{
    return std::make_unique<Sphere>(1.f, glm::vec4(0.f, 0.f, 0.f, 1.f), 1.f);
}