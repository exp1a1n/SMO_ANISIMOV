#pragma once
#include <queue>
#include <random>
#include <vector>
#include <string>
#include "Event.h"
#include "GameSlot.h"
#include "CircularBuffer.h"

class Simulation {
  std::priority_queue<Event> eventQueue;
  std::vector<GameSlot> slots;
  CircularBuffer buffer;
  std::default_random_engine gen;
  std::exponential_distribution<double> interArrival;
  std::exponential_distribution<double> serviceTime;
  int nextPlayerId = 1;
  std::vector<std::string> timeline;

  // Счётчики для статистики (требуется в пошаговом режиме)
  int totalGenerated = 0;
  int totalRejected = 0;

public:
  Simulation(int numSlots, int bufferSize, double lambda, double mu);
  void runStepByStep();

private:
  void processArrival(const Event& ev, double currentTime);
  void processDeparture(const Event& ev, double currentTime);
  void printState(double currentTime) const;
  void printEventCalendar() const; // <-- НОВОЕ
};