#include "src/Controller.h"

Controller controller(LED_BUILTIN);

void setup() {
  controller.begin();
}

void loop() {
  controller.update();
}
