#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

struct Marble {
    Marble(uint64_t v, Marble* p, Marble* n) : value(v), prev(p), next(n) {}
    uint64_t value;
    bool isCurrent = false;
    Marble* prev;
    Marble* next;
    static void print(Marble& head);
};

std::ostream& operator<<(std::ostream& o, const Marble& m)
{
    if (m.isCurrent)
        o << " (" << m.value << ")";
    else o << "  " << m.value;
    return o; 
}

void Marble::print(Marble& head)
{
    std::cout << head;
    auto current = head.next;
    while (current != &head) {
        std::cout << *current;
        current = current->next;
    }
    std::cout << std::endl;
}

using Circle_t = Marble;
using ScoreBoard_t = std::vector<uint64_t>;

Marble* insertNewMarble(uint64_t value, Marble& prev)
{
    Marble* newMarble = new Marble(value, &prev, prev.next);
    prev.next->prev = newMarble;
    prev.next = newMarble;
    return newMarble;
}

Marble* counterClockwise(uint64_t steps, Marble& current)
{
    Marble* selected = &current;
    while (steps--) {
        selected = selected->prev;
    }
    return selected;
}

Marble* clockwise(uint64_t steps, Marble& current)
{
    Marble* selected = &current;
    while (steps--) {
        selected = selected->next;
    }
    return selected;
}

void removeMarble(Marble* marble)
{
    marble->prev->next = marble->next;
    marble->next->prev = marble->prev;
    delete marble;
}

std::pair<Marble*, uint64_t> placeMarble(ScoreBoard_t& scoreBoard, uint64_t elf, uint64_t value, Marble& current)
{
    // score!
    if (value % 23 == 0) {
        auto scoreMarble = counterClockwise(7, current);
        uint64_t score = value + scoreMarble->value;
        scoreBoard[elf] += score;
        auto newCurrent = scoreMarble->next;
        newCurrent->isCurrent = true;
        current.isCurrent = false;
        removeMarble(scoreMarble);
        return std::make_pair(newCurrent, score);
    } 
    else {
        auto next = current.next;
        auto newMarble = insertNewMarble(value, *next);
        newMarble->isCurrent = true;
        current.isCurrent = false;
        return std::make_pair(newMarble, 0);
    }
    return std::make_pair(nullptr, 0);
}

void playMarbles(uint64_t players, uint64_t lastValue)
{
    ScoreBoard_t scoreBoard(players, 0);
    Circle_t circle(0, nullptr, nullptr); 
    circle.next = &circle; circle.prev = &circle; circle.isCurrent = true;
    
    uint64_t elf = 0;
    auto current = &circle;
    auto placement = std::make_pair(&circle, 0);
    for (uint64_t value = 1; /*placement.second*/ value != lastValue; ++value) {
        placement = placeMarble(scoreBoard, elf, value, *current);
        current = placement.first;
        elf = (elf + 1) % players;
    }

    
    std::sort(scoreBoard.begin(), scoreBoard.end());
    std::cout << "high score is " << scoreBoard.back() << std::endl;
}

int main(int argc, char* argv[])
{
    // players = 405, value of last marble = 71700
    // multiple of 23 gives points = marble + 7th counter-clockwise
    // marble which is removed. 
    // Next clockwise from the removed marbel is the new current.
    playMarbles(405, 7170000);
    //playMarbles(10, 1618);
    return 0;
}