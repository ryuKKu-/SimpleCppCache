//
// Created by Luc on 20/10/2015.
//

#include "CacheManager.h"

namespace Simple_Cache {

    /*
     * CacheManager constructor
     */
    CacheManager::CacheManager() {
        std::thread cleaner_(&CacheManager::cleanCacheFn, this);
        cleaner_.detach(); // background execution
    }


    /*
     * Get the unique instance of the class (singleton)
     * http://stackoverflow.com/a/1008289
     * return : CacheManager&
     */
    CacheManager& CacheManager::getInstance() {
        static CacheManager instance;
        return instance;
    }


    /*
     * Clear the cache through a background thread
     */
    void CacheManager::cleanCacheFn() {
        while (1) {

#ifdef DEBUG
            std::cout << "CLEANER TASK" << "\n";
#endif

            std::unordered_map<std::string, std::shared_ptr<DictionnaryBase>>::iterator it;
            it = cachedItems_.begin();

            while (it != cachedItems_.end()) {
                auto now = std::chrono::system_clock::now();
                auto offset = it->second->getOffsetTime();

                if (now > offset) {

#ifdef DEBUG
                    std::cout << "ERASE ITEM : " << it->first << "\n";
#endif

                    it = cachedItems_.erase(it);
                } else {
                    it++;
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }


    /*
     * Get a cached item
     * return : shared_ptr<DictionnaryBase>
     */
    std::shared_ptr<DictionnaryBase> CacheManager::getEntry(std::string cacheKey) {
        std::unordered_map<std::string, std::shared_ptr<DictionnaryBase>>::iterator it;

        it = cachedItems_.find(cacheKey);

        if (it != cachedItems_.end()) {

#ifdef DEBUG
            std::cout << "CACHE HIT : " << cacheKey << "\n";
#endif

            return cachedItems_[cacheKey];
        }

#ifdef DEBUG
        std::cout << "CACHE MISS : " << cacheKey << "\n";
#endif

        return nullptr;
    }

}