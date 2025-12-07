#pragma once
#include <vector>
#include <queue>
#include "Player.h"

class CircularBuffer {
  std::vector<Player*> slots;
  std::queue<double> arrivalQueue;
  int head = 0;
  int ringPtr = 0;
  int capacity;

public:
  CircularBuffer(int cap);
  bool add(Player* p);
  Player* removeOldest();
  Player* nextByRing();
  void print() const;
  bool isFull() const;
  bool isEmpty() const;

  // Геттеры для отображения указателей (требуется!)
  int getHead() const { return head; }
  int getRingPtr() const { return ringPtr; }
};