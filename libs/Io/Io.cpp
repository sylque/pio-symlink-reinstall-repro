#include "Io.h"

#include <Button2.h>

static Button2 s_button;

void ioSetup() { s_button.begin(0); }

void ioLoop() { s_button.loop(); }
