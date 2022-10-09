#include "functions.h"
#include "string.h"
#define _DEBUG
#define IMP_ANSWER "Impossible"
#include <limits.h>
#include <map>
#include <algorithm>
#include <vector>
#include <fstream>

Composition::Composition() {
    compName = NULL;
    compAuthor = NULL;
    compFeelMood = INT32_MIN;
    compSpeedMood = INT32_MIN;
}

Composition::Composition(const Composition& comp) {
    compName = new char[strlen(comp.compName) + 1];
    strcpy_s(compName, strlen(comp.compName) + 1, comp.compName);
    compAuthor = new char[strlen(comp.compAuthor) + 1];
    strcpy_s(compAuthor, strlen(comp.compAuthor) + 1, comp.compAuthor);
    compFeelMood = comp.compFeelMood;
    compSpeedMood = comp.compSpeedMood;
}

Composition::Composition(const char* Name, const char* Author, std::int32_t feelMood, std::int32_t speedMood) {
    compName = new char[strlen(Name) + 1];
    strcpy_s(compName, strlen(Name)+1, Name);
    compAuthor = new char[strlen(Author) + 1];
    strcpy_s(compAuthor, strlen(Author)+1, Author);
    compFeelMood = feelMood;
    compSpeedMood = speedMood;
}

Composition::~Composition() {
   delete[] compName;
   delete[] compAuthor;
}

void Composition::setName(const char* newName) {
    delete compName;
    compName = new char[strlen(newName) + 1];
    strcpy_s(compName, strlen(newName) + 1, newName);
}

void Composition::setAuthor(const char* newAuth) {
    delete compAuthor;
    compAuthor = new char[strlen(newAuth) + 1];
    strcpy_s(compAuthor, strlen(newAuth) + 1, newAuth);

}

void Composition::copy(const Composition& comp) {
    delete compName;
    compName = new char[strlen(comp.compName) + 1];
    strcpy_s(compName, strlen(comp.compName) + 1, comp.compName);
    delete compAuthor;
    compAuthor = new char[strlen(comp.compAuthor) + 1];
    strcpy_s(compAuthor, strlen(comp.compAuthor) + 1, comp.compAuthor);
    compFeelMood = comp.compFeelMood;
    compSpeedMood = comp.compSpeedMood;
}

void Composition::write(std::ostream& file) {
    uint32_t stl1, stl2;
    stl1 = strlen(compName);
    file.write((char*)&stl1, sizeof(uint32_t));
    file.write(compName, stl1);
    stl2 = strlen(compAuthor);
    file.write((char*)&stl2, sizeof(uint32_t));
    file.write(compAuthor, stl2);
    file.write((char*)&compFeelMood, sizeof(int32_t));
    file.write((char*)&compSpeedMood, sizeof(int32_t));
}

void Composition::read(std::istream& file) {
    uint32_t stl1, stl2;
    file.read((char*)&stl1, sizeof(uint32_t));
    compName = new char[stl1 + 1];
    file.read(compName, stl1);
    compName[stl1] = '\0';
    file.read((char*)&stl2, sizeof(uint32_t));
    compAuthor = new char[stl2 + 1];
    file.read(compAuthor, stl2);
    compAuthor[stl2] = '\0';
    file.read((char*)&compFeelMood, sizeof(int32_t));
    file.read((char*)&compSpeedMood, sizeof(int32_t));
}

Playlist::Playlist() {
    objects = NULL;
    count = 0;
    allocated = 0;
}

Playlist::Playlist(const Playlist& obj) {
    count = obj.getCount();
    objects = new Composition[count];
    allocated = count;
    for (uint32_t i = 0; i < count; i++) {
        objects[i].copy(obj.getData(i));
    }
}

Playlist::~Playlist() {
    if (allocated) {
        delete[] objects;
    }
}

Composition& Playlist::getData(std::uint32_t position) const {
    if (position < 0 || position >= count) {
        position = 0;
    }
    return objects[position];
}

void Playlist::grow() {
    Composition* comp = new Composition[count * 2];
    for (auto i = 0; i < std::min(count, allocated); i++) {
        comp[i].copy(this->getData(i));
    }
    delete[] objects;
    objects = comp;
    if (count == 1)
        allocated++;
    allocated *= 2;
}

void Playlist::insert(uint32_t position, const Composition& obj) {
    if (++count >= allocated) {
        grow();
    }
    for (uint32_t i = count-1; i > position; i--) {
        objects[i].copy(objects[i - 1]);
    }
    objects[position].copy(obj);
}

void Playlist::add(const Composition& obj) {
    if (count++ >= allocated) {
        grow();
    }
    objects[count - 1].copy(obj);
}

void Playlist::set(uint32_t position, const Composition& obj) {
    if (position >= count) {
#ifdef _DEBUG
        std::cout << IMP_ANSWER;
#endif
    }
    else {
        objects[position].copy(obj);
    }
}

void Playlist::write(const char* filename) const{
    std::ofstream ofile(filename, std::ios::binary);
    ofile.write((char*)&count, sizeof(uint32_t));
    for (auto i = 0; i < count; i++) {
        objects[i].write(ofile);
    }
}

void Playlist::read(const char* filename) {
    std::ifstream ifile(filename, std::ios::binary);
    ifile.read((char*)&count, sizeof(uint32_t));
    objects = new Composition[count];
    allocated = count;
    for (auto i = 0; i < count; i++) {
        objects[i].read(ifile);
    }
}

void Playlist::del(uint32_t position)
{
    if (position >= count) {
#ifndef _DEBUG
        std::cout << IMP_ANSWER;
#endif
    }
    else {
        Composition* comp = new Composition[count];
        for (uint32_t i = position; i < count - 1; i++) {
            objects[i].copy(objects[i + 1]);
        }
        for (uint32_t i = position; i < count - 1; i++) {
            comp[i].copy(this->getData(i));
        }
        delete[] objects;
        objects = comp;
        count--;
    }
}

void Playlist::delAll() {
    this->~Playlist();
    allocated = 0;
    count = 0;
}

Composition& Playlist::getListFeel(int32_t startMood, int32_t endMood) const{
    std::map<int32_t, int32_t> myMap;
    for (auto i = 0; i < count; i++) {
        if ((this->getData(i).getFeelMood() >= startMood) &&
            (this->getData(i).getFeelMood() <= endMood)) {
            myMap.insert({i, this->getData(i).getFeelMood()});
        }
    }
    if (myMap.size()) {
        std::vector<std::pair<int32_t, int32_t>> myVec;
        std::copy(myMap.begin(), myMap.end(),
            std::back_inserter<std::vector<std::pair<int32_t, int32_t>>>(myVec));
        std::sort(myVec.begin(), myVec.end(),
            [](std::pair<int32_t, int32_t>& a,
                std::pair<int32_t, int32_t>& b) {
                    return (a.second < b.second);
            });
        Composition* comp = new Composition[myMap.size()];
        uint32_t iter = 0;
        for (auto const &pair : myVec) {
            comp[iter++].copy(this->getData(pair.first));
        }
        return *comp;
    }
    else
    {
        Composition *c = new Composition();
        return *c;
    }
}

Composition& Playlist::getListSpeed(int32_t startMood, int32_t endMood) const{
    std::map<int32_t, int32_t> myMap;
    for (auto i = 0; i < count; i++) {
        if ((this->getData(i).getSpeedMood() >= startMood) && 
            (this->getData(i).getSpeedMood() <= endMood)) {
            myMap.insert({ i, this->getData(i).getSpeedMood() });
        }
    }
    if (myMap.size()) {
        std::vector<std::pair<int32_t, int32_t>> myVec;
        std::copy(myMap.begin(), myMap.end(),
            std::back_inserter<std::vector<std::pair<int32_t, int32_t>>>(myVec));
        std::sort(myVec.begin(), myVec.end(),
            [](std::pair<int32_t, int32_t>& a,
                std::pair<int32_t, int32_t>& b) {
                    return (a.second < b.second);
            });
        Composition* comp = new Composition[myMap.size()];
        uint32_t iter = 0;
        for (auto const& pair : myVec) {
            comp[iter++].copy(this->getData(pair.first));
        }
        return *comp;
    }
    else
    {
        Composition* c = new Composition();
        return *c;
    }
}

bool Playlist::compare(const Playlist& obj) const {
    if (this->getCount() != obj.getCount()) {
        return false;
    }
    for (auto i = 0; i < obj.getCount(); i++) {
        Composition c1 = this->getData(i);
        Composition c2 = obj.getData(i);
        if ((strcmp(c1.getAuthor(), c2.getAuthor()) != 0) ||
            (strcmp(c1.getName(), c2.getName()) != 0) ||
            (c1.getFeelMood() != c2.getFeelMood()) ||
            (c1.getSpeedMood() != c2.getSpeedMood())) {
            return false;
        }
    }
    return true;
}
void outPlaylist(const Playlist& play) {
    for (auto i = 0; i < play.getCount(); i++) {
        Composition c = play.getData(i);
        std::cout << "Composition " << i << ":\n" 
            << c.getName() << ' '
            << c.getAuthor() << ' '
            << c.getFeelMood() << ' '
            << c.getSpeedMood() << std::endl;
    }
}
Potpourri::Potpourri() {
    startFellMood = INT32_MIN;
    endFellMood = INT32_MIN;
    startSpeedMood = INT32_MIN;
    endSpeedMood = INT32_MIN;
}
Potpourri::Potpourri(const Potpourri& obj) {
    this->setName(obj.getName());
    this->setAuthor(obj.getAuthor());
    this->setFellMood(obj.getFeelMood());
    this->setSpeedMood(obj.getSpeedMood());
    startFellMood = obj.startFellMood;
    endFellMood = obj.endFellMood;
    startSpeedMood = obj.startSpeedMood;
    endSpeedMood = obj.endSpeedMood;
}

Potpourri::Potpourri(const char* Name, const char* Author, std::int32_t startFeelMood,
    std::int32_t endFeelMood, std::int32_t startSpeedMood, std::int32_t endSpeedMood) 
{
    this->setName(Name);
    this->setAuthor(Author);
    this->setFellMood((startFeelMood + endFeelMood) / 2);
    this->setSpeedMood((startSpeedMood + endSpeedMood) / 2);
    this->startFellMood = startFeelMood;
    this->endFellMood = endFeelMood;
    this->startSpeedMood = startSpeedMood;
    this->endSpeedMood = endSpeedMood;
}

Potpourri::~Potpourri() {

}

void Potpourri::write(std::ostream& file) {

}

void Potpourri::read(std::istream& file) {

}

