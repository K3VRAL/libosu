#include <stdio.h>
#include <setjmp.h>

#define TRY \
    do { \
        switch (setjmp(jump_buffer)) { \
            case 0:

#define CATCH(x) \
                break; \
            case x:

#define ETRY \
        } \
    } while (0)

#define THROW longjmp(jump_buffer, 1)

static jmp_buf jump_buffer;