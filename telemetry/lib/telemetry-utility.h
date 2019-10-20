#pragma once

#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
    #define FB_API __declspec(dllexport)
#else
    #define FB_API
#endif

namespace fb
{

/** Thread safe telemetry event builder and harvester
 *
 * To be used when telemetry data is aggregated from several different
 * threads, e.g. when running multiple initializations processes in
 * parallel.
 */

class DbObject final
{
public:
    DbObject() = default;
    DbObject(std::string k, std::string v) { m_dataMap[k] = v; }
    DbObject(const DbObject&) = default;
    DbObject(DbObject&&) = default;
    ~DbObject() = default;

    void append(const std::string& k, const std::string& v)
    {
        m_dataMap[k] = v;
    }

    std::unordered_map<std::string, std::string>::const_iterator begin() const { return m_dataMap.begin(); }
    std::unordered_map<std::string, std::string>::const_iterator end() const { return m_dataMap.end(); }

private:
    std::unordered_map<std::string, std::string> m_dataMap;
};

class DbObjectBuilder final
{
public:
    DbObjectBuilder() = default;
    DbObjectBuilder(const DbObjectBuilder&) = delete;
    DbObjectBuilder(DbObjectBuilder&&) = default;
    ~DbObjectBuilder() = default;

    DbObjectBuilder& operator<<(const std::string& e)
    {
        m_data.push_back(e);
        return *this;
    }

    DbObject done()
    {
        assert(m_data.size() % 2 == 0);
        DbObject obj;
        for (auto e = m_data.begin(), end = m_data.end(); e != end; e++) {
            obj.append(*e, *(e++));
        }
        m_data.clear();
        return obj;
    }

private:
    std::vector<std::string> m_data;
};

class StartupTelemetry
{
public:
	///{
	/// Append fields to the pipeline startup telemetry event
	FB_API static void append(const DbObject& o);
    FB_API static void append(DbObject&& o);
	///}

	/// Harvest the pipeline startup telemetry event. May only be called once
	FB_API static DbObject collect()
    {
        if (StartupTelemetry::s_hasBeenCollected)
            return DbObject();
        StartupTelemetry::s_hasBeenCollected = true;
        return s_builder.done();
    }

private:
	static bool             s_hasBeenCollected;
    static DbObjectBuilder  s_builder;
};

}