/*
 * tasks.h
 *
 *  Created on: 27 janv. 2018
 *      Author: Nri
 */

#ifndef TASKS_H_
#define TASKS_H_
#include <Arduino.h>

#define STATISTICS
#define MAX_NUMBER_OF_TASKS 10

extern boolean task_trace;

	class event {
	public:
		boolean evt;
				event() ;
		boolean is_set();
		boolean	was_set();
		void	set();
		void	clear();
	};

     class timer {
        public:
            unsigned long last;
            unsigned long period;
            boolean       debug;
            boolean       stopped;

                		timer(unsigned long v);
        boolean 		elapsed ();
        void    		set_period(unsigned long v);
        unsigned long 	get_period();
        void    		start_debug();
        void    		stop_debug();
        void    		start();
        void    		stop();
    };



        class task {
        public:
        	char ident[9 ];
        	boolean running;
        	timer * chrono;
			#ifdef STATISTICS
        		boolean			alarm;
				unsigned long 	iteration;
				unsigned long 	last_exe_time;
				unsigned long 	max_lapse;
				unsigned long 	max_load;
				unsigned long 	last_load;
				float 			mean_lapse;
				float 			mean_load;
			#endif

        	task(char * id,  void (*setup)(), void (*loop)(), void (*end)(), unsigned long period, boolean run);
            String 			dump();
        	void 			(*setup)();
        	void 			(*loop )();
        	void 			(*end  )();
        	void 			start();
        	void 			stop();
        	void 			schedule();
        	void 			set_period(unsigned long lapse);
        	unsigned long	get_period();
        	String 			status();
        	String 			statistics();
        	void 			statistics_reset();
        	void 			toggle();
        };

    	void task_all_stats_display();
    	void task_all_stats_reset();
        void schedule_all_tasks();
        String task_status();

		void task_stop_all();
		void task_start_all();
		void task_schedule_all();


#endif /* TASKS_H_ */
