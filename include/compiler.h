#ifndef COMPILER_H
#define COMPILER_H

// Declare the external assemblies templates
extern const char *g_HellHallAsm;
extern const char *g_StubTemplate;

// Function prototype to build the artifact
int build_artifact(void);

#endif // COMPILER_H
