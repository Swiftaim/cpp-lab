#include "misc.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

Journal::Journal(string_view title) : title(title) {}

void Journal::add_entry(const string& entry)
{
    static int count = 1;
    entries.push_back(to_string(count++) + ":" + entry);
}

void PersistanceManager::save(const Journal& journal, const string& filename)
{
    ofstream ofs(filename);
    for (auto& entry : journal.entries)
        ofs << entry << endl;
}