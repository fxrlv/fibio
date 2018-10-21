#include "fiber.h"

#include "coro/coro.h"

#include <stdlib.h>
#include <sys/queue.h>

struct fiber
{
    struct coro_context ctx;
    struct coro_stack stack;

    struct fiber *caller;

    void (*fn)(void *);
    void *args;

    // clang-format off
    TAILQ_ENTRY(fiber) link;
    // clang-format on
};

struct fiber *fiber;
TAILQ_HEAD(fiber_list, fiber);
struct fiber_list parked;

static void
fiber_loop(void *);

void
fiber_init()
{
    TAILQ_INIT(&parked);

    fiber = malloc(sizeof(struct fiber));
    coro_create(&fiber->ctx, NULL, NULL, NULL, 0);
}

struct fiber *
fiber_spawn(void (*fn)(void *), void *args)
{
    struct fiber *new = NULL;

    if (!TAILQ_EMPTY(&parked))
    {
        new = TAILQ_FIRST(&parked);
        TAILQ_REMOVE(&parked, new, link);
    }
    else
    {
        new = malloc(sizeof(struct fiber));
        if (new == NULL)
            goto error;

        if (!coro_stack_alloc(&new->stack, 8192))
            goto error;

        coro_create(&new->ctx, fiber_loop, NULL, new->stack.sptr, new->stack.ssze);
    }

    new->fn = fn;
    new->args = args;
    fiber_resume(new);
    return new;

error:
    if (new != NULL)
    {
        coro_stack_free(&new->stack);
        free(new);
    }

    return NULL;
}

void
fiber_yield()
{
    struct fiber *callee = fiber;
    fiber = callee->caller;
    coro_transfer(&callee->ctx, &fiber->ctx);
}

void
fiber_resume(struct fiber *callee)
{
    struct fiber *caller = fiber;
    callee->caller = caller;
    fiber = callee;
    coro_transfer(&caller->ctx, &callee->ctx);
}

void
fiber_cleanup(void)
{
    struct fiber *f;
    while (!TAILQ_EMPTY(&parked))
    {
        f = TAILQ_FIRST(&parked);
        TAILQ_REMOVE(&parked, f, link);
        coro_stack_free(&f->stack);
        free(f);
    }
}

static void
fiber_loop(void *args)
{
    (void) args;

    while (42)
    {
        fiber->fn(fiber->args);
        TAILQ_INSERT_HEAD(&parked, fiber, link);
        fiber_yield();
    }
}
