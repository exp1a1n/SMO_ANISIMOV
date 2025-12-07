#pragma once
#include "Player.h"

class GameSlot {
public:
  int id;
  bool isFree = true;
  double busyUntil = 0.0;
  Player* current = nullptr;

  GameSlot(int id);
  bool assign(Player* p, double currentTime);
  void release(double currentTime);
};