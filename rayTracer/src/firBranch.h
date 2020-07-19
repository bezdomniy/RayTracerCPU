#pragma once

#include "group.h"
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include <vector>
// #include <iostream>

class FirBranch : public Group
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::base_class<Group>(this));
    }

public:
    FirBranch(/* args */);
    ~FirBranch();

    void build();

    virtual std::string type() override;
};

CEREAL_REGISTER_TYPE(FirBranch);