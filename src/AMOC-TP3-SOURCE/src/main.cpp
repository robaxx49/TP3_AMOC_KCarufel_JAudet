#include <Arduino.h>

#include "Program.h"

Program *program;

void setup()
{
  // put your setup code here, to run once:
  program = new Program();
}

void loop()
{
  // put your main code here, to run repeatedly:
  program->loop();
}
