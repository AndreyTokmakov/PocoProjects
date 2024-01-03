/**============================================================================
Name        : MultiThreading.cpp
Created on  : 01.01.2024
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : MultiThreading.cpp
============================================================================**/

#include "MultiThreading.h"

#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/ThreadLocal.h"
#include "Poco/Timer.h"
#include "Poco/Task.h"
#include "Poco/TaskManager.h"
#include "Poco/TaskNotification.h"
#include "Poco/Observer.h"
#include "Poco/Activity.h"

using Poco::Timer;
using Poco::TimerCallback;


#include <iostream>


namespace CounterThreadTest
{
    class Counter: public Poco::Runnable
    {
        void run() override
        {
            static Poco::ThreadLocal<int> tls;
            for (*tls = 0; *tls < 10; ++(*tls))
            {
                std::cout << *tls << std::endl;
            }
        }
    };

    void Test()
    {
        Counter counter;
        Poco::Thread t1;
        Poco::Thread t2;

        t1.start(counter);
        t2.start(counter);

        t1.join();
        t2.join();
    }
}

namespace TimerExample
{
    class TimerExample
    {
    public:
        void onTimer(Poco::Timer&)
        {
            std::cout << "onTimer called." << std::endl;
        }
    };

    void Test()
    {
        TimerExample te;
        Timer timer(250, 500); // fire after 250ms, repeat every 500ms
        timer.start(TimerCallback<TimerExample>(te, &TimerExample::onTimer));
        Poco::Thread::sleep(5000);
        timer.stop();
    }
}

namespace Task_Observer
{

    struct SampleTask: public Poco::Task
    {
        explicit SampleTask(const std::string& name): Task(name) {
        }

        void runTask() override
        {
            for (int i = 0; i < 100; ++i)
            {
                setProgress(float(i)/100); // report progress
                if (sleep(1000))
                    break;
            }
        }
    };

    struct ProgressHandler
    {
        void onProgress(Poco::TaskProgressNotification* pNf)
        {
            std::cout << pNf->task()->name() << " progress: " << pNf->progress() << std::endl;
            pNf->release();
        }

        void onFinished(Poco::TaskFinishedNotification* pNf)
        {
            std::cout << pNf->task()->name() << " finished." << std::endl;
            pNf->release();
        }
    };

    void Test()
    {
        Poco::TaskManager tm;
        ProgressHandler pm;
        tm.addObserver(Poco::Observer<ProgressHandler,
                       Poco::TaskProgressNotification>(pm, &ProgressHandler::onProgress));
        tm.addObserver(Poco::Observer<ProgressHandler,
                       Poco::TaskFinishedNotification>(pm, &ProgressHandler::onFinished));
        tm.start(new SampleTask("Task 1")); // tm takes ownership
        tm.start(new SampleTask("Task 2"));
        tm.joinAll();
    }
}

namespace Activities
{
    using Poco::Thread;

    class ActivityExample
    {
    public:
        ActivityExample(): activity {this, &ActivityExample::runActivity} {
        }

        void start()
        {
            activity.start();
        }
        void stop()
        {
            activity.stop(); // request stop
            activity.wait(); // wait until activity actually stops
        }

    protected:

        void runActivity()
        {
            while (!activity.isStopped())
            {
                std::cout << "Activity running." << std::endl;
                Thread::sleep(200);
            }
        }
    private:
        Poco::Activity<ActivityExample> activity;
    };


    void Test()
    {
        ActivityExample example;
        example.start();
        Thread::sleep(2000);
        example.stop();
    }
}


void MultiThreading::TestAll()
{
    // CounterThreadTest::Test();
    // TimerExample::Test();
    // Task_Observer::Test();
    Activities::Test();
}