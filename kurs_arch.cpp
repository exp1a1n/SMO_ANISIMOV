#include <iostream>
#include "Simulation.h"

int main() {
  setlocale(LC_ALL, "ru");
  const int NUM_SLOTS = 3;      // количество игровых слотов
  const int BUFFER_SIZE = 5;    // размер буфера
  const double LAMBDA = 2.0;    // интенсивность потока заявок (ИЗ1)
  const double MU = 3.0;        // интенсивность обслуживания (ПЗ1)

  std::cout << "=== Имитационная модель СМО ВАРИАНТ 21 ===\n";
  std::cout << "Бизнес-домен: онлайн-игровой сервер\n";
  std::cout << "Пошаговый режим (ОД3): временные диаграммы + текущее состояние\n\n";

  Simulation sim(NUM_SLOTS, BUFFER_SIZE, LAMBDA, MU);
  sim.runStepByStep();

  std::cout << "\nМоделирование завершено.\n";
  return 0;
}