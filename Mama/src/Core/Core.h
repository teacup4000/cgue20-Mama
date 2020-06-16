#define STARTPOSITION_X -9.26916f
#define STARTPOSITION_Y 12.4
#define STARTPOSITION_Z -15.9479f

#define NUM_BONES_PER_VERTEX 4

#define GLCALL(call) call; _GLGetError(__FILE__, __LINE__, #call)

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define GRAVITY 0.05f
