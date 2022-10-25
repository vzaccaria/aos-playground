
#include "active_object.h"
#include <chrono>
#include <functional>
#include <iostream>

using namespace std;
using namespace std::chrono;

ActiveObject::ActiveObject() : t(&ActiveObject::run, this), quit(false) {}

ActiveObject::~ActiveObject()
{
    if(quit.load()) return; //For derived classes
    quit.store(true);
    t.join();
}
