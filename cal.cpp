#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;
//
const char * months [] =  {"January","February",
			   "March", "April",
			   "May", "June",
			   "July","August",
			   "September", "October",
			   "November", "December"};


/**
Returns a month from integer 0-11 from a string
@param month month in char * format
return index of month
*/			   
int month(const char * month) {
  for (int i = 0; i < 12; i++) {
    if (strcmp(month,months[i]) == 0) {
      return i;
    }
  }
  return -1;


}

/**
Returns length of specified month with year
@param year specified year
@param month specified month
return length of month
*/
int monthlength(int year, int month) {
  if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11) {
    return 31;
  }
  else if ((month == 1 && (year % 4 == 0 && year % 100 != 0)) || (year % 400 == 0)) return 29; //check for leap year 
  else if (month == 1) return 28;
  else return 30;
}

/*
Prints a specified month
@param year specified year
@param month specified month
@param daycount count of days
*/
void printmonth(int year, int month, int daycount) {
  cout.setf(ios::unitbuf);
  time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );
  now->tm_year += 1900;
  cout << "    " << months[month] << " " << year << endl;
  cout << "Su Mo Tu We Th Fr Sa" << endl;
  for (int i = 0; i < daycount; i++) {
    cout << "   ";
  }
  cout << " ";
  int count = 1;
  int mlength = monthlength(year,month);
  if ((year == 1752) && (month == 8)) mlength = 19;
  for (int i = 0; i < mlength; i++) {
    daycount++;
	if ((year == now->tm_year) && (month == now->tm_mon) && (count == now->tm_mday)) {
		cout << "\033[0;7m" << count << "\033[0m" << " ";
	}
	else {
		cout << count << " ";
	}
    int julianbool = 0;
    if (mlength == 19 && i == 1) julianbool = 1; //check if date is under julian calendar
    if ((count + 1) < 10 && julianbool != 1) {
      cout << " ";
    }
    if ((daycount % 7) == 0) {
      cout << endl;
      if ((count + 1) < 10) {
		cout << " ";
      }
    }
    if ((i == 1) && (year == 1752) && (month == 8)) {
      count += 11;
    }
    count++;
  }
  cout << endl;



}

/*
checks if a specified string is an int
@param argv string
@param length length of string
return true or false
*/
bool checkdigits(char * argv, int length) {
  for (int i = 0; i < length; i++) {
    if (!isdigit(argv[i])) {
      return false;
    }
  }
  if (atoi(argv) < 1 || atoi(argv) > 9999) {
    return false;
  }
  return true;
}

/**
Handles command line arguments and prints out specified calendar
@param argc number of args
@param argv string array of args
return exit value
*/
int main (int argc, char * argv[]) {
  cout.setf(ios::unitbuf);
  time_t rtime;
  struct tm * tinfo;
  time(&rtime);
  tinfo = localtime(&rtime);
  tinfo->tm_year = 2016 - 1900;
  tinfo->tm_mon = 10 - 1; //october 1st 2016
  tinfo->tm_mday = 1;
  mktime(tinfo);
  int daycount = tinfo->tm_wday;
  if (argc == 3 && atoi(argv[1]) >= 1 && atoi(argv[1]) <= 12 && checkdigits(argv[2],strnlen(argv[2],1000))) {
    tinfo->tm_mon = atoi(argv[1]) - 1;
    tinfo->tm_year = atoi(argv[2]) - 1900;
    mktime(tinfo);
    if ((atoi(argv[2]) > 1752)) daycount = tinfo->tm_wday;
    else if ((atoi(argv[2]) == 1752) && (atoi(argv[1]) > 9)) daycount = tinfo->tm_wday;
    else daycount = (tinfo->tm_wday + 4) % 7; //extract julian day
    printmonth(atoi(argv[2]), tinfo->tm_mon, daycount);
    
  }
  else if (argc == 2 && checkdigits(argv[1], strnlen(argv[1],1000))) {
    tinfo->tm_year = atoi(argv[1]) - 1900;
    for(int i = 0; i < 12; i++) {
      tinfo->tm_mon = i;
      mktime(tinfo);
      if ((atoi(argv[1]) > 1752)) daycount = tinfo->tm_wday;
      else if ((atoi(argv[1]) == 1752) && i > 8) daycount = tinfo->tm_wday;
      else daycount = (tinfo->tm_wday + 4) % 7; //extract julian day	  
      printmonth(atoi(argv[1]), tinfo->tm_mon, daycount);
    }
  }
  else if(argc == 1) {
    rtime = time(0);
    tinfo = localtime(&rtime);
    tinfo->tm_mday = 1;
    mktime(tinfo);
    daycount = tinfo->tm_wday;
    printmonth(tinfo->tm_year + 1900,tinfo->tm_mon,daycount);
  }
  else {
    cout << "Please enter in [[month] year] format" << endl;
  }
}
