//#include "BooterResult.h"

// Created by Luc on 20/10/2015.
//

#ifndef PROJECT_CACHEMANAGER_H
#define PROJECT_CACHEMANAGER_H

#include <string>
#include <unordered_map>
#include <chrono>
#include <memory>
#include <iostream>
#include <functional>
#include <vector>
#include <thread>

#include "CacheManagerDictionnaryValues.h"

namespace Simple_Cache {

    /*
     * CacheManager Class
     * http://stackoverflow.com/a/972197
     */
    class CacheManager {

    public:
        static CacheManager &getInstance();

        template<class T>
        T getOrStore(std::string cacheKey, std::function<T()> getItemsCallback,
                     std::chrono::system_clock::time_point offsetTime);

        void cleanCacheFn();

    private:
        std::unordered_map<std::string, std::shared_ptr<DictionnaryBase>> cachedItems_;

        CacheManager();

        CacheManager(CacheManager const &) = delete;

        void operator=(CacheManager const &) = delete;

        std::shared_ptr<DictionnaryBase> getEntry(std::string cacheKey);

        template<class T>
        void setEntry(std::string cacheKey, T item, std::chrono::system_clock::time_point offsetTime);
    };


    /*
     * Get an object from the cache or insert it
     * Object can be a pointer, a ref or a copy
     * return : T
     */
    template<class T>
    T CacheManager::getOrStore(std::string cacheKey, std::function<T()> getItemsCallback,
                               std::chrono::system_clock::time_point offsetTime) {

        std::shared_ptr<DictionnaryBase> cacheItem = getEntry(cacheKey);

        T object;

        if (cacheItem == nullptr) {
            object = getItemsCallback();
            setEntry<T>(cacheKey, object, offsetTime);
        } else {
            object = cacheItem->getObject<T>();
        }

        return object;
    }


    /*
     * Insert a new item in the cache
     */
    template<class T>
    void CacheManager::setEntry(std::string cacheKey, T item, std::chrono::system_clock::time_point offsetTime) {
#ifdef DEBUG
        std::cout << "CACHE SET : " << cacheKey << "\n";
#endif
        cachedItems_[cacheKey] = std::make_shared<DictionnaryValues<T>>(item, offsetTime);
    }

}



#endif //PROJECT_CACHEMANAGER_H
