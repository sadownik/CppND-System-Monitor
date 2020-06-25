#include <string>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string convert_2digit_string(long timet){ // add a zero for nicer look if necessary
    if (timet<10){
        return "0" + std::to_string(timet);
    }
    else{
        return std::to_string(timet);
    } 
}

string Format::ElapsedTime(long seconds) { //return string(); 

long hours = seconds / (int) 3600; // divide without remainder
int remain_secs = seconds- hours * 3600;
long minutes = remain_secs / (int) 60;
remain_secs=remain_secs- minutes * 60;

string HH = convert_2digit_string(hours); 
string MM = convert_2digit_string(minutes); 
string SS = convert_2digit_string(remain_secs); 

return HH +":"+ MM +":"+SS;
}