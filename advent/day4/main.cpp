#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum class Event
{
    ASLEEP = 0,
    AWAKE = 1
};

struct Record
{
    struct Time
    {
        int hour;
        int minute;
    } time;
    Event event;
    std::string date;
    std::string note;
    int guard = -1;
};

std::ostream& operator<<(std::ostream& o, const Record& r)
{
    return o << r.date << "\t" << r.time.hour << ":" << r.time.minute << "\t#" << r.guard << " " << r.note;
}

Record parse(const std::string& data)
{
    enum class State
    {
        BEGIN = 0,
        DATE,
        HOUR,
        MINUTE,
        NOTE,
        GUARD
    };

    Record record;
    std::string input;
    State state;
    for (auto c : data)
    {   
        switch (c) {
            case ' ':
                if (state == State::HOUR)
                {
                    input.clear();
                }
                else if (state == State::GUARD)
                {
                    record.guard = std::stoi(input);
                    state = State::NOTE;
                }
                else input += c;
                break;
            case '[':
                state = State::DATE;
                input.clear();
                break;
            case ']':
                input.clear();
                break;
            case '#':
                state = State::GUARD;
                input.clear();
                break;
            case ':':
                state = State::MINUTE;
                input.clear();
                break;
            default:
                input += c;
                if (state == State::DATE &&
                    input.size() == 10)
                {
                    record.date = input;
                    state = State::HOUR;
                } 
                else if (state == State::HOUR &&
                        input.size() == 2)
                {
                    record.time.hour = std::stoi(input);    
                }
                else if (state == State::MINUTE &&
                        input.size() == 2)
                {
                    record.time.minute = std::stoi(input);
                    state = State::NOTE;
                }
        }
    }

    if (state == State::NOTE)
        record.note = input;

    return record;
}

int main(int argc, char* argv[])
{
    std::ifstream file(argv[1]);
    std::string line;

    while (std::getline(file, line)) {
        std::cout << parse(line) << std::endl;
    }

    return 0;
}