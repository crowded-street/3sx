#include "sf33rd/Source/Game/main.h"

#if defined(TARGET_PS2)
void main() {
    AcrMain();
}
#else
int main() {
    AcrMain();
    return 0;
}
#endif
