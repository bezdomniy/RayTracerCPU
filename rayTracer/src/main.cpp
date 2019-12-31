#include "camera.h"
#include "canvas.h"
#include "geometry.h"
#include "pattern.h"
#include "patterns.h"
#include "plane.h"
#include "ray.h"
#include "renderer.h"
#include "sphere.h"
#include "world.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/bind.h>
#endif

void run(const std::string &sceneDesc)
{
  World world;
  std::shared_ptr<Camera> camera = world.loadFromFile(sceneDesc);

  Renderer renderer(camera);
  renderer.render(world);
  renderer.canvas.writeToPPM("test.ppm", false);

#ifdef __EMSCRIPTEN__
  emscripten::val::global("window").call<void>(
      "offerFileAsDownload", std::string("test.ppm"), std::string("mime/type"));
#endif
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Module)
{
  emscripten::function("runRayTracer", &run);
}

EM_JS(void, offerFileAsDownload, (std::string filename, std::string mime), {
  mime = mime || "application/octet-stream";

  let content = Module.FS.readFile(filename);
  // console.log(`Offering download of "${filename}", with ${content.length} bytes...`);

  var a = document.createElement('a');
  a.download = filename;
  a.href = URL.createObjectURL(new Blob([content], { type: mime }));
  a.style.display = 'none';

  document.body.appendChild(a);
  a.click();
  // setTimeout(() = > {
  //   document.body.removeChild(a);
  //   URL.revokeObjectURL(a.href);
  // },
  //            2000);
});
#endif

int main(int argc, char const *argv[])
{
#ifdef __EMSCRIPTEN__
  EM_ASM({
    var textarea = document.createElement("TEXTAREA");
    var defaultText = document.createTextNode("Enter scene definition here.");
    textarea.appendChild(defaultText);
    document.body.appendChild(textarea);

    // 1. Create the button
    var button = document.createElement("button");
    button.innerHTML = "go!";
    document.body.appendChild(button);

    // 3. Add event handler
    button.addEventListener("click", function() {
      console.log(textarea.value);
      Module.runRayTracer(textarea.value);
    });
  });
#else
  run("./scene2.yaml");
#endif

  return 0;
}
