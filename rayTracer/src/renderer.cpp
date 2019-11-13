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
   
    float canvasDistance = 20.f;
    glm::vec4 cameraPosition(-10.f,0.f,0.f,1.f);
    glm::vec3 cameraForward(1.f,0.f,0.f);
    // glm::vec3 cameraUp(0.f,1.f,0.f);
    // glm::vec3 cameraLeft = glm::cross(cameraForward, cameraUp);

    // glm::vec3 cShape(1.f, 0.f, 0.f);
	glm::vec3 cBackground(0.f, 0.f, 0.f);

    int halfHeight = this->canvas->height / 2;
    int halfWidth = this->canvas->width / 2;

    glm::vec4 pixelPosition = cameraPosition + glm::vec4(cameraForward * canvasDistance, 0.f);
    glm::vec4 rayDirection = glm::normalize(pixelPosition - cameraPosition);
    // std::cout << pixelPosition.x << pixelPosition.y << pixelPosition.z <<std::endl;
    // std::cout << cameraPosition.x << cameraPosition.y << cameraPosition.z <<std::endl;
    // std::cout << rayDirection.x << rayDirection.y << rayDirection.z <<std::endl;
    Ray cast(cameraPosition, rayDirection);

    // float horizonalAngle;
    // float verticalAngle;
    // glm::quat quatRotation;
    // glm::quat quatRotation2;
    // glm::mat4 rotation;

    float wallX = 80.f;
    float worldZ;
    float worldY;
    glm::vec4 position;

    
    // for (int i = this->canvas->height - 1; i >= 0; i--) {
    for (int i = 0; i < this->canvas->height; i++) {
        // verticalAngle = glm::atan((halfHeight - i)/canvasDistance);
        // quatRotation = glm::angleAxis(verticalAngle, cameraLeft);
        worldY = halfHeight - i;
        

        for (int j = 0; j < this->canvas->width; j++) {
            // horizonalAngle = glm::atan((-halfWidth + j)/canvasDistance);
            // // std::cout << verticalAngle << " " << horizonalAngle << std::endl;
            // quatRotation2 = glm::rotate(quatRotation,horizonalAngle,cameraUp);
	        // rotation = glm::mat4_cast(quatRotation2);
            // cast.direction = glm::normalize(rotation * rayDirection);
            

            worldZ = -halfWidth + j;
            position = glm::vec4(wallX,worldY,worldZ,1.f);
            cast.direction = glm::normalize(position - cast.origin);

            std::vector<Geometry::Intersection> intersections = Geometry::intersectRaySphere(cast, *sphere);
            Geometry::Intersection* hit;
            if ((hit = Geometry::hit(intersections))) {
                glm::vec4 point = cast.origin + cast.direction * hit->t;
                // glm::vec4 point = position;
                glm::vec4 normal = sphere->normalAt(point);
                glm::vec4 eye = -cast.direction;

                glm::vec3 cShape = lighting(sphere->material, lights.at(0), point, eye, normal);

                // std::cout << cast << std::endl;
                this->canvas->writePixel(i,j,cShape);
                // this->canvas->writePixel(cast.direction.y + 50,cast.direction.z+50,cShape);
            }
        }
    }
}

void Renderer::addLight(std::shared_ptr<PointLight>& light) {
    this->lights.push_back(light);
}

glm::vec3 Renderer::lighting(std::shared_ptr<Material> material, std::shared_ptr<PointLight> light, glm::vec4 point, glm::vec4 eyev, glm::vec4 normalv) {
    glm::vec3 diffuse;
    glm::vec3 specular;

    //combine the surface color with the light's color/intensity​
    glm::vec3 effectiveColour = material->colour * light->intensity;
    glm::vec4 lightv = glm::normalize(light->position - point);

    //compute the ambient contribution​
    glm::vec3 ambient = effectiveColour * material->ambient;

    //light_dot_normal represents the cosine of the angle between the​
    //light vector and the normal vector. A negative number means the​
    //light is on the other side of the surface.​

    float lightDotNormal = glm::dot(lightv, normalv);
    if (lightDotNormal < 0) {
        diffuse = glm::vec3(0.f,0.f,0.f);
        specular = glm::vec3(0.f,0.f,0.f);
    }
    else { 
        //compute the diffuse contribution​
        diffuse = effectiveColour * material->diffuse * lightDotNormal;

        //reflect_dot_eye represents the cosine of the angle between the
        //reflection vector and the eye vector. A negative number means the
        //light reflects away from the eye.​
        glm::vec4 reflectv = glm::reflect(-lightv, normalv);
        float reflectDotEye = glm::dot(reflectv, eyev);

        if (reflectDotEye <= 0) {
            specular = glm::vec3(0.f,0.f,0.f);
        }
        else {
            //compute the specular contribution​
            float factor = std::pow(reflectDotEye, material->shininess);
            specular = light->intensity * material->specular * factor;
        }
    }

    return (ambient + diffuse + specular);
}
