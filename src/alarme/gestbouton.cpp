#include "gestbouton.h"

void GestionBoutons::setup()
{
  pinMode(BTN_CONF_PIN, INPUT_PULLUP);
  pinMode(BTN_PLUS_PIN, INPUT_PULLUP);
  pinMode(BTN_MOINS_PIN, INPUT_PULLUP);
  pinMode(BTN_OK_PIN, INPUT_PULLUP);
}

void GestionBoutons::Update()
{
  btnpressed = 0;
  if (digitalRead(BTN_CONF_PIN) == LOW)
  {
    btnpressed |= BTN_CONF;
  }
  if (digitalRead(BTN_PLUS_PIN) == LOW)
  {
    btnpressed |= BTN_PLUS;
  }
  if (digitalRead(BTN_MOINS_PIN) == LOW)
  {
    btnpressed |= BTN_MOINS;
  }
  if (digitalRead(BTN_OK_PIN) == LOW)
  {
    btnpressed |= BTN_OK;
  }
  btnchanged = btnpressed ^ btnlastpressed;
  btnlastpressed = btnpressed;
}

bool GestionBoutons::justPressed(uint8_t button)
{
  return ((btnpressed & btnchanged) & button) != 0;
}

bool GestionBoutons::stillPressed(uint8_t button)
{
  return ((btnpressed & ~btnchanged) & button) != 0;
}

bool GestionBoutons::isPressed(uint8_t button)
{
  return (btnpressed & button) != 0;
}

