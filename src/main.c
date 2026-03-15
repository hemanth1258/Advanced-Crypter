#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

int main() {
  printf("\n"
         "   _____ _______  _______ _______ _______  ______\n"
         "  / ____|  ____\\ \\/ /  __ \\__   __|  ____|  __ \\\n"
         " | |    | |__   \\  /| |__) | | |  | |__  | |__) |\n"
         " | |    |  __|   \\/  |  ___/  | |  |  __| |  _  /\n"
         " | |____|  |     | | | |      | |  | |____| | \\ \\\n"
         "  \\_____|_|     |_| |_|      |_|  |______|_|  \\_\\\n"
         "        Artifact Builder & Obfuscator        \n"
         "\n");

  return build_artifact();
}
