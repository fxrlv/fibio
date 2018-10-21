#ifndef _FIBER_H_
#define _FIBER_H_

struct fiber;

void
fiber_init();

struct fiber *
fiber_spawn(void (*fn)(void *), void *args);

void
fiber_yield();

void
fiber_resume(struct fiber *callee);

void
fiber_cleanup(void);

#endif // _FIBER_H_
