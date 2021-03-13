#include "firBranch.h"

std::shared_ptr<Group> FirBranch::build()
{
    Float length = 2.0;
    Float radius = 0.025;
    int segments = 20;
    int perSegment = 24;

    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.reserve(segments + (segments * perSegment));

    std::shared_ptr<Shape> branch = std::make_shared<Cylinder>(0.0, length, true);
    Mat4 scale =
        glm::scale(Mat4(1.0), Vec3(radius, 1.0, radius));

    branch->multiplyTransform(scale);
    branch->calculateInverseTranform();

    std::shared_ptr<Material> branchMaterial = std::make_shared<Material>();
    branchMaterial->colour = Vec3(0.5, 0.35, 0.26);
    branchMaterial->ambient = 0.2;
    branchMaterial->specular = 0.0;
    branchMaterial->diffuse = 0.6;

    branch->setMaterial(branchMaterial);
    shapes.push_back(branch);

    Float segSize = length / (segments - 1);
    Float theta = 2.1 * glm::pi<Float>() / (Float)perSegment;

    Float maxLength = 20.0 * radius;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<Float> dis(0, 1);

    std::shared_ptr<Material> triMaterial = std::make_shared<Material>();
    triMaterial->colour = Vec3(0.26, 0.36, 0.16);
    // triMaterial->ambient = 0.2;
    triMaterial->specular = 0.1;
    // triMaterial->diffuse = 0.6;

    for (int y = 0; y < segments; y++)
    {
        for (int i = 0; i < perSegment; i++)
        {
            Float yBase = segSize * (Float)y + dis(gen) * segSize;
            Float yTip = yBase - dis(gen) * segSize;
            Float yAngle = (Float)i * theta + dis(gen) * theta;

            Float needleLength = maxLength / 2.0 * (1.0 + dis(gen));
            Float ofs = radius / 2.0;

            Vec3 p1(ofs, yBase, ofs);
            Vec3 p2(-ofs, yBase, ofs);
            Vec3 p3(0.0, yTip, needleLength);

            Mat4 rotation = glm::rotate(Mat4(1.0), yAngle,
                                              Vec3(0.0, 1.0, 0.0));

            p1 = Vec3(rotation * Vec4(p1, 1.0));
            p2 = Vec3(rotation * Vec4(p2, 1.0));
            p3 = Vec3(rotation * Vec4(p3, 1.0));

            std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(p1, p2, p3);
            triangle->setMaterial(triMaterial);

            shapes.push_back(triangle);
        }
    }

    std::shared_ptr<Group> root = Model::buildBoundingVolumeHierarchy(shapes);
    std::shared_ptr<Material> rootMaterial = std::make_shared<Material>();
    root->setMaterial(rootMaterial);

    return root;
}
