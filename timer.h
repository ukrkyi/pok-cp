/* (c) 2018 ukrkyi */
#ifndef TIMER_H
#define TIMER_H

extern void timer_init();

extern void (*falling_it)(void);
extern void rising_it(void);

#endif // TIMER_H
