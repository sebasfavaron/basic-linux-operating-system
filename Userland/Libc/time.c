#include <stdint.h> 
 
extern void int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8); 
 
typedef struct { 
  int hours; 
  int minutes; 
  int seconds;   
} time; 
 
typedef struct { 
  int day; 
  int month; 
  int year;   
} date; 
 
static time currentTime; 
static date currentDate; 
 
void updateTime() { 
  int80(4, &currentTime.hours, &currentTime.minutes, &currentTime.seconds, 0); 
} 
 
void updateDate() { 
  int80(5, &currentDate.day, &currentDate.month, &currentDate.year, 0); 
} 
 
int getHour() { 
  updateTime(); 
  return currentTime.hours; 
} 
 
int getMinutes() { 
  updateTime(); 
  return currentTime.minutes; 
} 
 
int getSeconds() { 
  updateTime(); 
  return currentTime.seconds; 
} 
 
int getDay() { 
  updateDate(); 
  return currentDate.day; 
} 
 
int getMonth() { 
  updateDate(); 
  return currentDate.month; 
} 
 
int getYear() { 
  updateDate(); 
  return currentDate.year; 
} 
 
void setTimeZone(int tz) { 
  int80(3, tz, 0, 0, 0); 
} 