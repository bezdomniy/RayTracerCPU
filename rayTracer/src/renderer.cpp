#include "renderer.h"

Renderer::Renderer()
{
}

Renderer::Renderer(std::shared_ptr<Canvas> c)
{
    this->canvas = c;
}

Renderer::~Renderer()
{
}

void Renderer::render(Shape& shape) {
    Sphere* sphere;
    try {
        sphere = dynamic_cast<Sphere*>(&shape);
    }
    catch (std::bad_cast& bc) {
        std::cerr << "bad_cast caught: " << bc.what() << '\n';
        return;
    }
   
    float canvasDistance = 150.f;
    glm::vec4 cameraPosition(-5.f,0.f,0.f,1.f);
    glm::vec3 cameraForward(1.f,0.f,0.f);
    glm::vec3 cameraUp(0.f,1.f,0.f);
    glm::vec3 perpCameraForward = glm::cross(cameraForward, cameraUp);

    glm::vec3 cShape(1.f, 0.f, 0.f);
	glm::vec3 cBackground(0.f, 0.f, 0.f);

    int halfHeight = this->canvas->height / 2;
    int halfWidth = this->canvas->width / 2;

    glm::vec4 pixelPosition = cameraPosition + glm::vec4(cameraForward * canvasDistance, 0.f);
    glm::vec4 rayDirection = pixelPosition - cameraPosition;
    Ray cast(cameraPosition, rayDirection);

    float horizonalAngle;
    float verticalAngle;
    glm::quat quatRotation;
    glm::quat quatRotation2;
    glm::mat4 rotation;

    
    

    for (int i = 0; i < this->canvas->height; i++) {
        verticalAngle = glm::atan((-halfHeight + i)/canvasDistance);
        quatRotation = glm::angleAxis(verticalAngle, perpCameraForward);

        

        for (int j = 0; j < this->canvas->width; j++) {
            horizonalAngle = glm::atan((-halfWidth + j)/canvasDistance);
            quatRotation2 = glm::rotate(quatRotation,horizonalAngle,cameraUp);
            // quatRotation = glm::angleAxis(horizonalAngle, cameraUp);
	        rotation = glm::mat4_cast(quatRotation2);
            cast.direction = rotation * rayDirection;

            // std::cout << horizonalAngle << " " << verticalAngle << std::endl;

            // std::cout << cast << std::endl;

            std::optional<std::vector<Geometry::Intersection>> intersections = Geometry::intersectRaySphere(cast, *sphere);
            if (intersections.has_value()) {
                this->canvas->writePixel(i,j,cShape);
            }
        }
    }
}