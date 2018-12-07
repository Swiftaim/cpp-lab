#include "telemetry-utility.h"
#include <iostream>

using namespace fb;

bool            StartupTelemetry::s_hasBeenCollected = false;
DbObjectBuilder StartupTelemetry::s_builder;

void StartupTelemetry::append(const DbObject& o)
{
    for (auto& e : o)
    {
        StartupTelemetry::s_builder << e.first << e.second;
    }
}

void StartupTelemetry::append(DbObject&& o)
{
    for (auto const& e : o)
    {
        StartupTelemetry::s_builder << e.first << e.second;
    }
}