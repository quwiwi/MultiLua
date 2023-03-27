
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <vram.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <png.h>

void vfpu_sincosf(float x, float *s, float *c)
{
    __asm__ volatile (
        "mtv    %2, s000\n"        // s000 = x
        "vcst.s s001, VFPU_2_PI\n"  // s001 = 2/pi
        "vmul.s s000, s000, s001\n"   // s000 = s000*s001
        "vrot.p c010, s000, [s, c]\n" // s010 = sinf(s000), s011 = cosf(s000)
        "mfv    %0, s010\n"        // *s = s010
        "mfv    %1, S011\n"        // *c = s011
        : "=r"(*s), "=r"(*c) : "r"(x)
    );
}