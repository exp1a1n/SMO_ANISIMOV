#include "CircularBuffer.h"
#include <iostream>

CircularBuffer::CircularBuffer(int cap) : capacity(cap) {
  slots.assign(cap, nullptr);
}

bool CircularBuffer::add(Player* p) {
  if (isFull()) return false;
  int start = head;
  int pos = start;
  do {
    if (slots[pos] == nullptr) {
      slots[pos] = p;
      arrivalQueue.push(p->arrivalTime);
      head = (pos + 1) % capacity;
      return true;
    }
    pos = (pos + 1) % capacity;
  } while (pos != start);
  return false;
}

Player* CircularBuffer::removeOldest() {
  if (isEmpty()) return nullptr;
  double oldest = arrivalQueue.front();
  arrivalQueue.pop();

  for (int i = 0; i < capacity; ++i) {
    if (slots[i] && slots[i]->arrivalTime == oldest) {
      Player* p = slots[i];
      slots[i] = nullptr;
      return p;
    }
  }
  return nullptr;
}

Player* CircularBuffer::nextByRing() {
  for (int i = 0; i < capacity; ++i) {
    int idx = (ringPtr + i) % capacity;
    if (slots[idx] != nullptr) {
      Player* p = slots[idx];
      slots[idx] = nullptr;
      ringPtr = (idx + 1) % capacity;
      // Не удаляем из arrivalQueue — для простоты
      return p;
    }
  }
  return nullptr;
}

void CircularBuffer::print() const {
  std::cout << "  [";
  for (int i = 0; i < capacity; ++i) {
    if (slots[i])
      std::cout << "P" << slots[i]->id << " ";
    else
      std::cout << ". ";
  }
  std::cout << "]\n";
}

bool CircularBuffer::isFull() const {
  for (auto* p : slots)
    if (p == nullptr) return false;
  return true;
}

bool CircularBuffer::isEmpty() const {
  return arrivalQueue.empty();
}