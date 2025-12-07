#include "Simulation.h"
#include <iostream>
#include <iomanip>
#include <vector>

Simulation::Simulation(int numSlots, int bufferSize, double lambda, double mu)
    : buffer(bufferSize), interArrival(lambda), serviceTime(mu) {
    slots.reserve(numSlots);
    for (int i = 0; i < numSlots; ++i) {
        slots.emplace_back(i + 1);
    }
    double firstArrival = interArrival(gen);
    eventQueue.push({firstArrival, 0, nextPlayerId++});
    totalGenerated = 1;
}

void Simulation::runStepByStep() {
    while (!eventQueue.empty()) {
        std::cout << "\n--- Нажмите Enter для следующего шага ---\n";
        std::cin.ignore();
        std::cin.get();

        Event ev = eventQueue.top(); eventQueue.pop();
        double currentTime = ev.time;

        if (ev.type == 0) {
            processArrival(ev, currentTime);
        } else {
            processDeparture(ev, currentTime);
        }

        printState(currentTime);
    }
}

void Simulation::processArrival(const Event& ev, double currentTime) {
    Player* p = new Player(ev.playerId, currentTime);
    p->serviceTime = serviceTime(gen);

    bool assigned = false;
    for (auto& slot : slots) {
        if (slot.isFree && slot.assign(p, currentTime)) {
            eventQueue.push({currentTime + p->serviceTime, 1, p->id, slot.id});
            timeline.push_back("t=" + std::to_string(currentTime) + ": Player " + std::to_string(p->id) + " -> Slot " + std::to_string(slot.id));
            assigned = true;
            break;
        }
    }

    if (!assigned) {
        if (!buffer.add(p)) {
            Player* old = buffer.removeOldest();
            if (old) {
                old->rejected = true;
                totalRejected++;
                timeline.push_back("t=" + std::to_string(currentTime) + ": REJECTED oldest Player " + std::to_string(old->id));
                delete old;
            }
            buffer.add(p);
        }
        timeline.push_back("t=" + std::to_string(currentTime) + ": Player " + std::to_string(p->id) + " -> Buffer");
    }

    // Генерация следующей заявки
    eventQueue.push({currentTime + interArrival(gen), 0, nextPlayerId++});
    totalGenerated++;
}

void Simulation::processDeparture(const Event& ev, double currentTime) {
    int slotIndex = ev.deviceId - 1;
    slots[slotIndex].release(currentTime);
    timeline.push_back("t=" + std::to_string(currentTime) + ": Slot " + std::to_string(ev.deviceId) + " freed");

    Player* next = buffer.nextByRing();
    if (next) {
        if (slots[slotIndex].assign(next, currentTime)) {
            eventQueue.push({currentTime + next->serviceTime, 1, next->id, ev.deviceId});
            timeline.push_back("t=" + std::to_string(currentTime) + ": Buffer -> Player " + std::to_string(next->id) + " -> Slot " + std::to_string(ev.deviceId));
        }
    }
}

void Simulation::printEventCalendar() const {
    if (eventQueue.empty()) {
        std::cout << "Календарь событий: пуст\n";
        return;
    }

    // Копируем очередь для вывода (std::priority_queue не позволяет итерироваться)
    auto tempQueue = eventQueue;
    std::vector<Event> events;
    while (!tempQueue.empty()) {
        events.push_back(tempQueue.top());
        tempQueue.pop();
    }

    std::cout << "Календарь событий:\n";
    std::cout << "Время\t| Тип события\t\t| Player\t| Device\n";
    std::cout << "--------|-----------------------|------------|--------\n";
    for (const auto& e : events) {
        std::string typeStr = (e.type == 0) ? "Arrival" : "Departure";
        std::cout << std::fixed << std::setprecision(3) << e.time << "\t| "
                  << typeStr << "\t\t| " << e.playerId << "\t\t| "
                  << (e.deviceId == -1 ? "-" : std::to_string(e.deviceId)) << "\n";
    }
}

void Simulation::printState(double currentTime) const {
    std::cout << "\n=== Текущее состояние (t = " << std::fixed << std::setprecision(3) << currentTime << ") ===\n";

    // Слоты
    std::cout << "Слоты:\n";
    for (const auto& slot : slots) {
        if (slot.isFree)
            std::cout << "  Slot " << slot.id << ": свободен\n";
        else
            std::cout << "  Slot " << slot.id << ": занят Player " << slot.current->id << " до " << std::fixed << std::setprecision(2) << slot.busyUntil << "\n";
    }

    // Буфер + указатели
    std::cout << "Буфер:\n";
    buffer.print();
    std::cout << "Указатели буфера: head (запись) = " << buffer.getHead()
              << ", ringPtr (чтение) = " << buffer.getRingPtr() << "\n";

    // Статистика
    double rejectionRate = (totalGenerated > 0) ? (static_cast<double>(totalRejected) / totalGenerated * 100.0) : 0.0;
    std::cout << "Статистика: сгенерировано = " << totalGenerated
              << ", отказано = " << totalRejected
              << ", % отказа = " << std::fixed << std::setprecision(1) << rejectionRate << "%\n";

    // Временная диаграмма
    std::cout << "\nПоследние события (временная диаграмма):\n";
    size_t start = (timeline.size() > 6) ? timeline.size() - 6 : 0;
    for (size_t i = start; i < timeline.size(); ++i) {
        std::cout << "  " << timeline[i] << "\n";
    }

    // Календарь событий (НОВОЕ — требование преподавателя)
    std::cout << "\n";
    printEventCalendar();
}