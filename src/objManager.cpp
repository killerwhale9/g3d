#include "objManager.hpp"

std::map<std::string, objReader*> objManager::models;

void objManager::loadObj(const std::string& file, const std::string& texture, const std::string &key)
{
    if (models.find(key) == models.end()) {
        models[key] = new objReader(file, texture.c_str());
    } else {
        std::cout<<"ObjManager Error loading obj "<<file<<": key '"<<key<<"' already exist\n";
    }

}

void objManager::free()
{
    for (std::map<std::string, objReader*>::iterator it(models.begin()); it != models.end(); ++it) {
        delete it->second;
    }
    models.clear();
}

