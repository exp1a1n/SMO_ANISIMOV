#pragma once

struct Event {
  double time;
  int type; // 0 = arrival, 1 = departure
  int playerId;
  int deviceId = -1;

  bool operator<(const Event& other) const {
    return time > other.time; // для min-heap
  }
};