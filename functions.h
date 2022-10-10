#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <string>

class Composition {

public:
    Composition();
    Composition(const Composition& comp);
    Composition(const char* Name, const char* Author, std::int32_t feelMood, std::int32_t speedMood);
    virtual ~Composition();
    void setName(const char* newName);
    void setAuthor(const char* newAuth);
    virtual void copy(const Composition& comp);
    virtual Composition* clone() const { return new Composition(*this); }
    void setFellMood(const int32_t newFeel) { compFeelMood = newFeel; }
    void setSpeedMood(const int32_t newSpeed) { compSpeedMood = newSpeed; }
    virtual void write(std::ostream& file);
    virtual void read(std::istream& file);
    virtual void out(uint32_t i);
    char* getName() const { return compName; };
    char* getAuthor() const { return compAuthor; };
    std::int32_t getFeelMood() const { return compFeelMood; }
    std::int32_t getSpeedMood() const { return compSpeedMood; }
private:
    char* compName;
    char* compAuthor;
    std::int32_t compFeelMood;
    std::int32_t compSpeedMood;
};

class Playlist {
public:
    Playlist();
    Playlist(const Playlist& obj);
    ~Playlist();
    void add(const Composition& obj);
    void del(uint32_t position);
    void delAll();
    void insert(uint32_t position, const Composition& obj);
    void set(uint32_t position, const Composition& obj);
    void write(const char* filename) const;
    void read(const char* filename);
    Composition& getData(uint32_t position) const;
    uint32_t getCount() const { return count; }
    Composition& getListFeel(int32_t startMood, int32_t endMood) const;
    Composition& getListSpeed(int32_t startMood, int32_t endMood) const;
    bool compare(const Playlist& obj) const;
private:
    Composition* objects;
    uint32_t count;
    uint32_t allocated;
    void grow();
};

void outPlaylist(const Playlist& play);

class Potpourri : public Composition
{
public:
    Potpourri();
    Potpourri(const Potpourri& obj);
    Potpourri(const char* Name, const char* Author, std::int32_t startFeelMood, 
        std::int32_t endFeelMood, std::int32_t startSpeedMood, std::int32_t endSpeedMood);
    virtual ~Potpourri();
    virtual void write(std::ostream& file);
    virtual void read(std::istream& file);
    void out(uint32_t i);
    void copy(const Potpourri& obj);
    Composition* clone() const { return new Potpourri(*this); }
    int32_t getStartFeelMood() const { return startFellMood; }
    int32_t getEndFeelMood() const { return endFellMood; }
    int32_t getStartSpeedMood() const { return startSpeedMood; }
    int32_t getEndSpeedMood() const { return endSpeedMood; }
    void setStartFeelMood(const int32_t newFeel) { startFellMood = newFeel; }
    void setEndFeelMood(const int32_t newFeel) { endFellMood = newFeel; }
    void setStartSpeedMood(const int32_t newSpeed) { startFellMood = newSpeed; }
    void setEndSpeedMood(const int32_t newSpeed) { endFellMood = newSpeed; }
private:
    int32_t startFellMood;
    int32_t endFellMood;
    int32_t startSpeedMood;
    int32_t endSpeedMood;
};

#endif // FUNCTIONS_H

