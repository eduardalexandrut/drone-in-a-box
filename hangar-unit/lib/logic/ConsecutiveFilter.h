// lib/logic/ConsecutiveFilter.h
#pragma once

class ConsecutiveFilter {
public:
  ConsecutiveFilter(bool initial, int toTrue, int toFalse)
    : value(initial), toTrue(toTrue), toFalse(toFalse), count(0) {}

  // chiamata a ogni reazione con la lettura grezza; restituisce il valore filtrato
  bool update(bool raw) {
    if (raw == value) { count = 0; return value; }
    count++;
    if ((raw && count >= toTrue) || (!raw && count >= toFalse)) {
      value = raw;
      count = 0;
    }
    return value;
  }

  bool get() const { return value; }

  void reset(bool newValue)
  {
    value = newValue;
    count = 0;
  }

private:
  bool value;
  int toTrue, toFalse, count;
};