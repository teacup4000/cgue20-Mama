#define STARTPOSITION_X 9
#define STARTPOSITION_Y 10
#define STARTPOSITION_Z -11

#define NUM_BONES_PER_VERTEX 4

#define GLCALL(call) call; _GLGetError(__FILE__, __LINE__, #call)

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define GRAVITY 0.05f