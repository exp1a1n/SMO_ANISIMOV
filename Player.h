#pragma once

struct Player {
  int id;
  double arrivalTime;
  double serviceTime;
  bool rejected = false;

  Player(int id, double arrivalTime);
};