#include "../src/tuple.h"
#include "testUtils.h"
#include <iostream>
#include <string>

bool testVec() {
    glm::vec3 testVec(3.f,4.f,5.f);

    std::cout << "Tuple x: " << testVec.x << std::endl;

    return true;
}

int main(int argc, char const *argv[])
{
    ASSERT_THROW(testVec());
    return 0;
}