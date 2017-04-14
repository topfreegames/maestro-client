#include <iostream>
#include <ctime>

long int unix_timestamp()  
{
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}
