#include "Commons.h"

#ifndef __DEBUG_LOG_SIMPLE__
#define __DEBUG_LOG_SIMPLE__ 1
#endif

#if __DEBUG_LOG_SIMPLE__
void debugLog(char* s0) {
  debugLog(s0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1) {
  debugLog(s0, s1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2) {
  debugLog(s0, s1, s2, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3) {
  debugLog(s0, s1, s2, s3, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4) {
  debugLog(s0, s1, s2, s3, s4, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5) {
  debugLog(s0, s1, s2, s3, s4, s5, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14) {
  Serial.print(s0);
  if (s1 != NULL) Serial.print(s1);
  if (s2 != NULL) Serial.print(s2);
  if (s3 != NULL) Serial.print(s3);
  if (s4 != NULL) Serial.print(s4);
  if (s5 != NULL) Serial.print(s5);
  if (s6 != NULL) Serial.print(s6);
  if (s7 != NULL) Serial.print(s7);
  if (s8 != NULL) Serial.print(s8);
  if (s9 != NULL) Serial.print(s9);
  if (s9 != NULL) Serial.print(s10);
  if (s9 != NULL) Serial.print(s11);
  if (s9 != NULL) Serial.print(s12);
  if (s9 != NULL) Serial.print(s13);
  if (s9 != NULL) Serial.print(s14);
  Serial.println();
}

#else // __DEBUG_LOG_SIMPLE__

void debugLog(char* s[10], int total) {
  if (total <= 0 || total > 10) return;
  for(int i=0; i<total-1; i++) {
    Serial.print(s[i]);
  }
  Serial.println(s[total-1]);
}

void debugLog(char* s0) {
  char* c[1] = { s0 };
  debugLog(c, 1);
}

void debugLog(char* s0, char* s1) {
  char* c[2] = { s0, s1 };
  debugLog(c, 2);
}

void debugLog(char* s0, char* s1, char* s2) {
  char* c[3] = { s0, s1, s2 };
  debugLog(c, 3);
}

void debugLog(char* s0, char* s1, char* s2, char* s3) {
  char* c[4] = { s0, s1, s2, s3 };
  debugLog(c, 4);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4) {
  char* c[5] = { s0, s1, s2, s3, s4 };
  debugLog(c, 5);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5) {
  char* c[6] = { s0, s1, s2, s3, s4, s5 };
  debugLog(c, 6);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6) {
  char* c[7] = { s0, s1, s2, s3, s4, s5, s6 };
  debugLog(c, 7);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7) {
  char* c[8] = { s0, s1, s2, s3, s4, s5, s6, s7 };
  debugLog(c, 8);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8) {
  char* c[9] = { s0, s1, s2, s3, s4, s5, s6, s7, s8 };
  debugLog(c, 9);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9) {
  char* c[10] = { s0, s1, s2, s3, s4, s5, s6, s7, s8, s9 };
  debugLog(c, 10);
}

#endif // __DEBUG_LOG_SIMPLE__
