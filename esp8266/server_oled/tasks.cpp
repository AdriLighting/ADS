/*
 * tasks.cpp
 *
 *  Created on: 27 janv. 2018
 *      Author: Nri
 *
 *
 *
 *  2018 02 06 added statistics.
 *
 *      Mean and max time time are now provided for the periodicity  and the load for each task
 *      Because this consumes 2 longs and 2 floats, there is a conditional compile switch for
 *      saving RAM when SW has no longer any need to be debugged
 *
 */


	#include "tasks.h"
	#include "tools.h"

// #define STATISTICS
boolean task_trace = false;

			event::event() 			{ evt=false;							}
	boolean	event::is_set() 		{ return evt;							}
	void	event::set()			{ evt=true;								}
	void	event::clear()			{ evt=false;							}
	boolean	event::was_set()		{ boolean b=evt; evt=false; return b;	}


    timer::timer(unsigned long v){
        period = v;
        debug = false;
        stopped=false;
        last = micros();
    }
    void timer::set_period(unsigned long v) {
        period = v;
    }
    unsigned long timer::get_period() {
    	return period;
    }
    void timer::start_debug() {
        debug=true;
    }
    void timer::stop_debug() {
        debug=false;
    }

    void timer::start() {
        stopped=false;
    }

    void timer::stop() {
        stopped=true;
    }

    boolean timer::elapsed () {
        if (stopped) return false;
        long now=micros();
        unsigned long delta = (now>=last) ? now-last : (0xffffffff - last) + now ;
        boolean done = (delta>=period);
        if (debug) {
        	String s = (done) ? " DONE" : "";
            fsprintf("Timer abs time:%8d period:%4d elapsed:%4d",now/1000,period,delta); Serial.println(s);
        }
        if (done) {
            last = now;
            return true;
        }
        return false;
    }

// TASK

	int number_of_tasks=0;
	task * task_pool[MAX_NUMBER_OF_TASKS];




		task::task( char * id, void (*setup_v)(), void (*loop_v)(), void (*end_v)(), unsigned long period_v, boolean run) {
			if (number_of_tasks < MAX_NUMBER_OF_TASKS) {
				int i=0; while ( (i<8)  ) { ident[i] = id[i]; i++; }
				running = run;
				#ifdef STATISTICS
					statistics_reset();
				#endif
				setup	= setup_v;
				loop	= loop_v;
				end		= end_v;
				chrono = new timer(period_v);
				//if (task_trace) {fsprintf("Task %-8s started\n", ident);}
				setup();
				task_pool[number_of_tasks++]=this;
				if (task_trace) {
					if (running) 	{fsprintf("Task %-8s created, T=%8d, running\n", ident, period_v);}
					else 			{fsprintf("Task %-8s created, T=%8d, stopped\n", ident, period_v);}
				}
			}
			else
			{fsprintln("Fatal error: tried to open too many tasks");}
		}

		void task::schedule() {
			if (running) {
				if (chrono->elapsed()) {
					if (task_trace) {fsprintf("Task '%s' runs ", ident);}
					#ifdef STATISTICS
						/*
						 * if statistics are enabled, the real periodicity and the task cpu load are checked
						 * when a task overloads the processor, an alarm flag is raised
						 */
						unsigned long last_elapsed;
						unsigned long now=micros();
						if (last_exe_time!=0) { // no calculation 1st time, because time between init and 1st run may be very long
							last_elapsed = (now>=last_exe_time) ? now-last_exe_time : (0xffffffff - last_exe_time ) + now ;
							mean_lapse=((mean_lapse*iteration)+last_elapsed) / (iteration+1);
							max_lapse=max(last_elapsed,max_lapse);
						}
						last_exe_time=now;
						now=micros();
						loop();
						if (task_trace) {fsprintln("and exits");}
						last_load=micros()-now;
						mean_load=((mean_load*iteration)+last_load) / (iteration+1);
						max_load = max(max_load, last_load);
						alarm = (last_load>get_period());
						iteration++;
					#else
						loop();
						if (task_trace) {fsprintln("and exits");}
					#endif
				}
			}
		}

		String task::statistics() {
			#ifdef STATISTICS
				char buf[100];
				sprintf(buf,"%-10s\tite:%10d\t period [def:%5d mean:%5d max:%5d]\tload [mean:%5d last:%5d max:%5d]\n",ident,iteration, int(get_period()/1000), int(mean_lapse/1000), max_lapse/1000, int(mean_load/1000), int(last_load/1000), max_load/1000);
				return String(buf);
			#else
				return "";
			#endif
		}

		void task::statistics_reset() {
			#ifdef STATISTICS
				iteration		= 0;
				last_exe_time	= 0;
				mean_lapse		= 0;
				max_lapse		= 0;
				mean_load		= 0;
				max_load		= 0;
			#endif
		}

		void task::start() {
			running=true;
			if (task_trace) {fsprintf("Task %-8s started\n", ident );}
		}

		void task::stop() {
			running=false;
			if (task_trace) {fsprintf("Task %-8s stopped\n", ident);}
		}

		void task::toggle() {
			running = !running;
			if (task_trace) {fsprintf("Task %-8s toggle\n", ident);}
		}

		void task::set_period(unsigned long lapse) {
			chrono->set_period(lapse);
			if (task_trace) {fsprintf("Task %-8s set period to %d\n", ident, lapse);}
		}

		unsigned long task::get_period() {
			return chrono->get_period();
		}

		String task::dump() {
			String s="";
			s=(running)?"r ":"  ";
			char buf[100];
			#ifdef STATISTICS
				sprintf(buf, "%-8s (%8d mS) %8d iterations\n",ident, chrono->period/1000, iteration); s+=String(buf);
			#else
				sprintf(buf, "%-8s (%8d mS)\n",ident, chrono->period/1000); s+=String(buf);
			#endif
			return s;
		}

		String task::status() {
			if (running) return String(ident) + " started"; else return String(ident) + " stopped";
		}


		void task_all_stats_display() {
			#ifdef STATISTICS
				//char b[8];
				fsprintln("Task statistics");
				for (int i=0; i<number_of_tasks; i++) {
					if (task_pool[i]->iteration!=0) Serial.print(task_pool[i]->statistics());
				}
				Serial.println();
			#endif
		}

		void task_stop_all()		{
			for (int i=number_of_tasks-1; i>=0; i--) {
				if (String(task_pool[i]->ident).indexOf("kbd")==-1) { // we never kill the keyboard
					task_pool[i]->end();
					task_pool[i]->stop();
					if (task_trace) fsprintf("task %-8s terminated\n",task_pool[i]->ident );
					yield();
				}
			}
		}

		void task_start_all()		{
			for (int i=0; i<number_of_tasks; i++) {
				task_pool[i]->setup();
				if (task_trace) fsprintf("task %-8s setup\n",task_pool[i]->ident );
				yield();
			}
		}

		void task_schedule_all()	{
			for (int i=0; i<number_of_tasks; i++) {
				task_pool[i]->schedule();
				yield();
			}
		}

		void task_all_stats_reset() {
			#ifdef STATISTICS
				char b[8];
				fsprintln("resetting all tasks statistics");
				for (int i=0; i<number_of_tasks; i++) task_pool[i]->statistics_reset();
				task_all_stats_display();
			#endif
		}


		void schedule_all_tasks() {
			for (int i=0; i<number_of_tasks; i++) {
				task_pool[i]->schedule();
				#ifdef STATISTICS
					//if ((task_pool[i]->iteration!=0) && (task_pool[i]->alarm)) Serial.print(task_pool[i]->statistics());
				#endif
				//fsprintf("scheduling task %s \n",task_pool[i]->ident);
			}
			yield();
			//Serial.println();
		}

		String task_status() {
			String s="TASKS\n";
			char b[8];
			for (int i=0; i<number_of_tasks; i++) {
				sprintf(b,"%2d ",i+1);
				s+=String(b);
				s+=task_pool[i]->dump();
			}
			return s;
		}

