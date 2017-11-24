/* 
 * File:   Timer.h
 * Author: msobral
 *
 * Created on 21 de Maio de 2016, 11:16
 */

#ifndef TIMER_H
#define	TIMER_H

#include <signal.h>
#include <time.h>

// argumentos: timer_id e objeto que recebe o disparo do timer
typedef void (*TimerHandler)(int, void*);

class Timer {
public:
    Timer();
    Timer(long ms, TimerHandler aHandler, int timerId, void * obj);
    Timer(const Timer& orig);
    virtual ~Timer();
    void set_restartable(bool restart);
    void start();
    void stop();
    Timer & operator=(const Timer & outro);
private:
    TimerHandler handler;
    long timeout;
    timer_t timer;
    int timer_id;
    void * owner; // objeto que recebe o disparo do timer
    bool restart;
    
    void init_timer();
    static void timer_handler(int signo, siginfo_t * info, void * ctx);
};

#endif	/* TIMER_H */

