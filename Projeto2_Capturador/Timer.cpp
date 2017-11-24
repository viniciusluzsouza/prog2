/* 
 * File:   Timer.cpp
 * Author: msobral
 * 
 * Created on 21 de Maio de 2016, 11:16
 */

#include <cstdlib>
#include "Timer.h"

typedef void (*Sighandler)(int, siginfo_t *, void *);

Timer::Timer() : restart(false), timeout(0), handler(NULL), timer(0), timer_id(0), owner(NULL) {
}

Timer::Timer(const Timer& orig) {
    timer = orig.timer;
    *this = orig;
}

Timer::Timer(long ms, TimerHandler aHandler, int timerId, void * obj) : timeout(ms), handler(aHandler), 
        timer(0), timer_id(timerId), owner(obj), restart(false) {
    init_timer();
}

Timer::~Timer() {
    if (timer) timer_delete(timer);
}

Timer& Timer::operator =(const Timer& outro) {
    if (timer) timer_delete(timer);
    timeout = outro.timeout;
    handler = outro.handler;
    timer_id = outro.timer_id;
    owner = outro.owner;
    restart = outro.restart;
    init_timer();
}

void Timer::init_timer() {
    if (timer) timer_delete(timer);
    if (timeout > 0) {        
        sigevent_t ev;

        ev.sigev_signo = SIGUSR2;
        ev.sigev_notify  = SIGEV_SIGNAL;
        ev.sigev_value.sival_ptr = (void*)this;

        struct sigaction int_handler;
        int_handler.sa_sigaction=(Sighandler)Timer::timer_handler;
        int_handler.sa_flags = SA_SIGINFO;
        
        sigaction(SIGUSR2,&int_handler,0);
        
        if (timer_create(CLOCK_REALTIME, &ev, &timer) < 0) throw -1;
    }
}

void Timer::set_restartable(bool restart) {
    this->restart = restart;
}

void Timer::start() {
    if (timer <= 0) throw -1;
    if (timeout <= 0) throw -1;
    
    struct itimerspec ts = {{0, 0}, {timeout/1000,(timeout%1000)*1000000}};

    if (timer_settime(timer, 0, &ts, NULL) < 0) throw -1;
}

void Timer::stop() {
    if (timer <= 0 or timeout <= 0) return;
    
    struct itimerspec ts = {{0, 0}, {0, 0}};
    if (timer_settime(timer, 0, &ts, NULL) < 0) throw -1;    
}

void Timer::timer_handler(int signo, siginfo_t * info, void * ctx) {
    Timer * obj = (Timer*)info->si_value.sival_ptr;
    
    if (obj->restart) obj->start();
    obj->handler(obj->timer_id, obj->owner);
}