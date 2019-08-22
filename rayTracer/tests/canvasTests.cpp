#include "../src/canvas.h"
#include "testUtils.h"
#include <iostream>

bool testWritePixel() {
    Canvas c(10, 20);
    glm::vec3 red(1.f,0.f,0.f);

    c.writePixel(2, 3, red);

    return c.getPixel(2, 3) == red;
}

int main(int argc, char const *argv[])
{
    ASSERT_THROW(testWritePixel());
    return 0;
}