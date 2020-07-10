#pragma once

#include "group.h"
#include <glm/gtc/matrix_transform.hpp>
#include <random>

#include <vector>
// #include <iostream>

class FirBranch : public Group
{
private:
public:
    FirBranch(/* args */);
    ~FirBranch();

    void build();

    virtual std::string type() override;
};
