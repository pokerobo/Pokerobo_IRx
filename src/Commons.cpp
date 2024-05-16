#include "Commons.h"

#ifndef __DEBUG_LOG_SIMPLE__
#define __DEBUG_LOG_SIMPLE__ 1
#endif

HangingDetector::HangingDetector(void (*trigger)(), uint16_t limit) {
  begin(trigger, limit);
}

void HangingDetector::begin(void (*trigger)(), uint16_t limit) {
  _trigger = trigger;
  if (limit > 0) {
    _limit = limit;
  }
  reset();
}

bool HangingDetector::check(bool ok) {
  if (ok) {
    _count = 0;
    _triggered = false;
  } else {
    if (_count >= _limit) {
      if (_trigger != NULL && !_triggered) {
        _triggered = true;
        _trigger();
      }
    } else {
      _count++;
    }
  }
}

void HangingDetector::reset() {
  check(true);
}

#if __DEBUG_LOG_SIMPLE__
void debugLog(char* s0) {
  debugLog(s0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1) {
  debugLog(s0, s1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2) {
  debugLog(s0, s1, s2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3) {
  debugLog(s0, s1, s2, s3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4) {
  debugLog(s0, s1, s2, s3, s4, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5) {
  debugLog(s0, s1, s2, s3, s4, s5, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, NULL, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, NULL, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14, char* s15) {
  debugLog(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, NULL, NULL, NULL, NULL);
}

void debugLog(char* s0, char* s1, char* s2, char* s3, char* s4, char* s5, char* s6, char* s7, char* s8, char* s9,
    char* s10, char* s11, char* s12, char* s13, char* s14, char* s15, char* s16, char* s17, char* s18, char* s19) {
  Serial.print(s0);
  while(1) {
    if (s1 != NULL) Serial.print(s1); else break;
    if (s2 != NULL) Serial.print(s2); else break;
    if (s3 != NULL) Serial.print(s3); else break;
    if (s4 != NULL) Serial.print(s4); else break;
    if (s5 != NULL) Serial.print(s5); else break;
    if (s6 != NULL) Serial.print(s6); else break;
    if (s7 != NULL) Serial.print(s7); else break;
    if (s8 != NULL) Serial.print(s8); else break;
    if (s9 != NULL) Serial.print(s9); else break;
    if (s10 != NULL) Serial.print(s10); else break;
    if (s11 != NULL) Serial.print(s11); else break;
    if (s12 != NULL) Serial.print(s12); else break;
    if (s13 != NULL) Serial.print(s13); else break;
    if (s14 != NULL) Serial.print(s14); else break;
    if (s15 != NULL) Serial.print(s15); else break;
    if (s16 != NULL) Serial.print(s16); else break;
    if (s17 != NULL) Serial.print(s17); else break;
    if (s18 != NULL) Serial.print(s18); else break;
    if (s19 != NULL) Serial.print(s19); else break;
    break;
  }
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
