#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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

    static bool compare(const Record& l, const Record& r)
    {
        if (l.date < r.date)
            return true;
        if (l.date > r.date)
            return false;
        if (l.time.hour < r.time.hour)
            return true;
        if (l.time.hour > r.time.hour)
            return false;
        if (l.time.minute < r.time.minute)
            return true;
        return false;
    }

    bool operator==(const Record& r)
    {
        return guard == r.guard;
    }
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
    std::vector<Record> records;
    while (std::getline(file, line)) {
        records.push_back(parse(line));
    }
    std::sort(records.begin(), records.end(), Record::compare);
    
    // guards an their sleep times
    std::unordered_map<int, std::unordered_map<int, int>> asleep;
    int asleeptime = -1;
    int guard = -1;
    for (const auto& r : records)
    {
        if (r.guard != -1)
        {
            guard = r.guard;
        } else if (r.note.find("asleep") != std::string::npos)
        {
            asleeptime = r.time.minute;
        } else if (r.note.find("wakes"))
        {
            assert(asleeptime > -1);
            int wokeuptime = r.time.minute;
            for (int t = asleeptime; t < wokeuptime; ++t)
                asleep[guard][t] += 1;
            asleeptime = -1;
        }
    }

    int maxsleeptime = 0;
    int selectedguard = -1;
    for (const auto& guardsleep : asleep)
    {
        int minutes = 0;
        const auto& asleepMap = guardsleep.second;
        for (const auto& p : asleepMap)
            minutes += p.second;
        std::cout << "guard #" << guardsleep.first << " asleep " << minutes << std::endl;
        if (minutes > maxsleeptime)
        {
            maxsleeptime = minutes;
            selectedguard = guardsleep.first;
        }
    }
    std::cout << "Guard #" << selectedguard << " slept for " << maxsleeptime << " minutes" << std::endl;

    int sleepyminute = -1;
    int maxrepeats = 0;
    const auto& guardSleepLog = asleep[selectedguard];
    for (const auto& p : guardSleepLog)
    {
        if (p.second > maxrepeats)
        {
            sleepyminute = p.first;
            maxrepeats = p.second;
        }
    }

    std::cout << "Chosen guard: #" << selectedguard << ", chosen minute: " << sleepyminute << "; Answer: " << selectedguard * sleepyminute << std::endl;
    return 0;
}