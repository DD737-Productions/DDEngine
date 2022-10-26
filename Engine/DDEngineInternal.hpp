#ifndef DDENGINE_INTERNAL_HPP

#define DDENGINE_INTERNAL_HPP

#include <iostream>

using namespace std;

namespace DDEngineInternal
{

    using CID = size_t;

    inline CID getCID()
    {
        static CID id = 0;
        return id++;
    }

    template<typename T>
    inline CID getCID() noexcept
    {
        static CID id = getCID();
        return id;
    }

    constexpr size_t MAX_COMPONENT_COUNT = 32;

}

#endif