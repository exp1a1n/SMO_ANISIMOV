#include "GameSlot.h"

GameSlot::GameSlot(int _id) : id(_id) {}

bool GameSlot::assign(Player* p, double currentTime) {
  if (!isFree) return false;
  isFree = false;
  current = p;
  busyUntil = currentTime + p->serviceTime;
  return true;
}

void GameSlot::release(double currentTime) {
  current = nullptr;
  isFree = true;
}