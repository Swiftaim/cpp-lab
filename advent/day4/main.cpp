#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

// guardId has map of [minutes : count]
using SleepLog_t = std::unordered_map<int, std::unordered_map<int, int>>;

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

std::pair<int, int> sleepyMinute(SleepLog_t& sleepLog, int guardId)
{
    int sleepyminute = -1;
    int maxrepeats = -1;
    const auto& guardSleepLog = sleepLog[guardId];
    for (const auto& p : guardSleepLog)
    {
        if (p.second > maxrepeats)
        {
            sleepyminute = p.first;
            maxrepeats = p.second;
        }
    }
    return std::make_pair(sleepyminute, maxrepeats);
}

std::pair<int, int> mostSleepyMinute(SleepLog_t& sleepLog)
{
    struct GuardMostSleepyMinute {
        int guardId;
        int minute;
        int count;
        bool operator<(const GuardMostSleepyMinute& g) const { return count < g.count; }
    };
    std::vector<GuardMostSleepyMinute> sleepyMinutes;

    int mostSleepyMinute = -1;
    int count = -1;
    int chosenGuard = -1;
    for (const auto& guard : sleepLog)
    {
        auto sleepy = sleepyMinute(sleepLog, guard.first);
        sleepyMinutes.push_back({guard.first, sleepy.first, sleepy.second});
        if (sleepy.second > count)
        {
            count = sleepy.second;
            mostSleepyMinute = sleepy.first;
            chosenGuard = guard.first;
        }
    }
    std::sort(sleepyMinutes.begin(), sleepyMinutes.end());
    for (auto& e : sleepyMinutes)
    {
        std::cout << "#" << e.guardId << ", minute: " << e.minute << ", " << e.count << std::endl;
    }
    return std::make_pair(chosenGuard, mostSleepyMinute);
}

void solution(const char* path)
{
    std::ifstream file(path);
    std::string line;
    std::vector<Record> records;
    while (std::getline(file, line)) {
        records.push_back(parse(line));
    }
    std::sort(records.begin(), records.end(), Record::compare);
    
    // guards and their sleep times
    SleepLog_t sleepLog;
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
                sleepLog[guard][t] += 1;
            asleeptime = -1;
        }
    }

    int maxsleeptime = 0;
    int selectedguard = -1;
    for (const auto& guardsleep : sleepLog)
    {
        int minutes = 0;
        const auto& asleepMap = guardsleep.second;
        for (const auto& p : asleepMap)
            minutes += p.second;
        if (minutes > maxsleeptime)
        {
            maxsleeptime = minutes;
            selectedguard = guardsleep.first;
        }
    }

    int sleepy = sleepyMinute(sleepLog, selectedguard).first;
    std::cout << "Part One - Chosen guard: #" << selectedguard << ", chosen minute: " << sleepy << "; Answer: " << selectedguard * sleepy << std::endl;

    auto mostSleepy = mostSleepyMinute(sleepLog);
    std::cout << "Part Two - Chosen guard: #" << mostSleepy.first << ", chosen minute: " << mostSleepy.second << "; Answer: " << mostSleepy.first * mostSleepy.second << std::endl;
}

int main(int argc, char* argv[])
{
    solution(argv[1]);
    return 0;
}