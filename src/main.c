#include "fiber.h"

#include <stdio.h>

void
coro(void *args)
{
    (void) args;
    printf("coro\n");
}

void
bar(void *args)
{
    (void) args;
    printf("bar 1\n");

    fiber_yield();
    printf("bar 2\n");

    fiber_spawn(coro, NULL);
    printf("bar 3\n");
}

void
foo(void *args)
{
    (void) args;
    printf("foo 1\n");

    fiber_yield();
    printf("foo 2\n");

    struct fiber *f;
    f = fiber_spawn(bar, NULL);
    printf("foo 2\n");

    fiber_yield();
    printf("foo 3\n");

    fiber_resume(f);
    printf("foo 4\n");
}

int
main(void)
{
    fiber_init();

    struct fiber *f;
    f = fiber_spawn(foo, NULL);
    printf("main 1\n");

    fiber_resume(f);
    printf("main 2\n");

    fiber_resume(f);
    printf("main 3\n");

    fiber_cleanup();
    return 0;
}
