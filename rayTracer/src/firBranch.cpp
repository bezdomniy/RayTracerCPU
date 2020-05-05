#include "firBranch.h"

FirBranch::FirBranch(/* args */) : Group()
{
    double length = 2.0;
    double radius = 0.025;
    int segments = 20;
    int perSegment = 24;

    std::shared_ptr<Shape> branch = std::make_shared<Cylinder>(0.0, length, true);
    glm::dmat4 scale =
          glm::scale(glm::dmat4(1.0), glm::dvec3(radius, 1.0, radius));

    branch->multiplyTransform(scale);
    branch->calculateInverseTranform();

    std::shared_ptr<Material> branchMaterial = std::make_shared<Material>();
    branchMaterial->colour = glm::dvec3(0.5,0.35,0.26);
    branchMaterial->ambient = 0.2;
    branchMaterial->specular = 0.0;
    branchMaterial->diffuse = 0.6;

    branch->setMaterial(branchMaterial);
    this->addChild(branch);

    double segSize = length / (segments - 1);
    double theta = 2.1 * glm::pi<double>() / (double)perSegment;

    double maxLength = 20.0 * radius;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0, 1);

    for (int y = 0; y < segments; y++) {
        std::shared_ptr<Group> subgroup = std::make_shared<Group>();

        for (int i = 0; i < perSegment; i++) {
            double yBase = segSize * (double)y + dis(gen) * segSize;
            double yTip = yBase - dis(gen) * segSize;
            double yAngle = (double)i * theta + dis(gen) * theta;

            double needleLength = maxLength / 2.0 * (1.0 + dis(gen));
            double ofs = radius / 2.0;

            glm::dvec3 p1(ofs, yBase, ofs);
            glm::dvec3 p2(-ofs, yBase, ofs);
            glm::dvec3 p3(0.0, yTip, needleLength);

            std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(p1,p2,p3);

            glm::dmat4 rotation = glm::rotate(glm::dmat4(1.0), yAngle,
                          glm::dvec3(0.0, 1.0, 0.0));

            triangle->multiplyTransform(rotation);
            triangle->calculateInverseTranform();

            std::shared_ptr<Material> triMaterial = std::make_shared<Material>();
            triMaterial->colour = glm::dvec3(0.26,0.36,0.16);
            // triMaterial->ambient = 0.2;
            triMaterial->specular = 0.1;
            // triMaterial->diffuse = 0.6;
            triangle->setMaterial(triMaterial);

            subgroup->addChild(triangle);

        }
        std::shared_ptr<Shape> subgroupShape = std::dynamic_pointer_cast<Shape>(subgroup);
        this->addChild(subgroupShape);
    }

}


FirBranch::~FirBranch()
{
}




std::string FirBranch::type() { return "FirBranch"; }