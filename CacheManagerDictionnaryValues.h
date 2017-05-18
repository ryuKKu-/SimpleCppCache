//
// Created by Luc on 20/10/2015.
//

#ifndef PROJECT_CACHEMANAGERDICTIONNARYVALUES_H
#define PROJECT_CACHEMANAGERDICTIONNARYVALUES_H

#include <chrono>

namespace Simple_Cache {

    /*
     * Base class for cache item
     * Mandatory, because we can't declare a map with a template parameter
     */
    class DictionnaryBase {

    public:
        virtual ~DictionnaryBase() { }

        template<class T>
        T getObject();

        virtual std::chrono::system_clock::time_point getOffsetTime() = 0;
    };


    /*
     * Dictionnary used to store an item in the cache
     * Include T object and TTL variable
     */
    template<typename T>
    class DictionnaryValues : public DictionnaryBase {

    public :
        DictionnaryValues(T obj, std::chrono::system_clock::time_point time) : object(obj), offsetTime(time) { }

        ~DictionnaryValues() { }

        T getObject() { return object; }

        std::chrono::system_clock::time_point getOffsetTime() { return offsetTime; }

    private:
        T object;
        std::chrono::system_clock::time_point offsetTime;

    };


    template<class T>
    T DictionnaryBase::getObject() {
        return dynamic_cast<DictionnaryValues<T> &>(*this).getObject();
    }

}

#endif //PROJECT_CACHEMANAGERDICTIONNARYVALUES_H
