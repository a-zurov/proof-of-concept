// problem_15.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ------------------------------------------------------------------------------------------------
// https://www.hackerrank.com/challenges/magic-spells/problem

#include <iostream>
#include <vector>
#include <string>
#include <set>

// comment this macro and check includes before copying to HackerRank editor
#define INPUT_FROM_FILE

#ifdef INPUT_FROM_FILE
#include <fstream>
std::ifstream input_stream("test.txt");
#else
#define input_stream std::cin
#endif

using namespace std;

class Spell {
private:
    string scrollName;
public:
    Spell() : scrollName("") { }
    Spell(string name) : scrollName(name) { }
    virtual ~Spell() { }
    string revealScrollName() {
        return scrollName;
    }
};

class Fireball : public Spell {
private: int power;
public:
    Fireball(int power) : power(power) { }
    void revealFirepower() {
        cout << "Fireball: " << power << endl;
    }
};

class Frostbite : public Spell {
private: int power;
public:
    Frostbite(int power) : power(power) { }
    void revealFrostpower() {
        cout << "Frostbite: " << power << endl;
    }
};

class Thunderstorm : public Spell {
private: int power;
public:
    Thunderstorm(int power) : power(power) { }
    void revealThunderpower() {
        cout << "Thunderstorm: " << power << endl;
    }
};

class Waterbolt : public Spell {
private: int power;
public:
    Waterbolt(int power) : power(power) { }
    void revealWaterpower() {
        cout << "Waterbolt: " << power << endl;
    }
};

class SpellJournal {
public:
    static string journal;
    static string read() {
        return journal;
    }
};

string SpellJournal::journal = "";

void counterspell(Spell* spell) {

    /* Enter your code here */
    bool bIsNotFound = true;
    {
        Fireball* p = dynamic_cast<Fireball*>(spell);
        if (p) { p->revealFirepower(); bIsNotFound = false; }
    }
    {
        Frostbite* p = dynamic_cast<Frostbite*>(spell);
        if (p) { p->revealFrostpower(); bIsNotFound = false; }
    }
    {
        Waterbolt* p = dynamic_cast<Waterbolt*>(spell);
        if (p) { p->revealWaterpower(); bIsNotFound = false; }
    }
    {
        Thunderstorm* p = dynamic_cast<Thunderstorm*>(spell);
        if (p) { p->revealThunderpower(); bIsNotFound = false; }
    }

    if (bIsNotFound) {
        string a, b;
        a = spell->revealScrollName();
        b = SpellJournal::read();

        auto lcs = [](const string& a, const string& b)
        {
            short m[101][101] = {};
            for ( int i = 0; i < a.size(); ++i) {
                for (int j = 0; j < b.size(); ++j) m[i + 1][j + 1] =
                    ( a[i] == b[j] ? m[i][j] + 1 : max(m[i + 1][j], m[i][j + 1]) );
            }
            return m[a.size()][b.size()];
        };
        cout << lcs(a, b) << '\n';
    }
}

class Wizard {
public:
    Spell* cast() {
        Spell* spell;
        string s; input_stream >> s;
        int power; input_stream >> power;
        if (s == "fire") {
            spell = new Fireball(power);
        }
        else if (s == "frost") {
            spell = new Frostbite(power);
        }
        else if (s == "water") {
            spell = new Waterbolt(power);
        }
        else if (s == "thunder") {
            spell = new Thunderstorm(power);
        }
        else {
            spell = new Spell(s);
            input_stream >> SpellJournal::journal;
        }
        return spell;
    }
};

int main() {

#ifdef INPUT_FROM_FILE
    if (!input_stream.is_open()) {
        std::cout << "File 'test.txt' is not found : test stopped..\n";
        return 0;
    }
    /*
    Thunderstorm: 48096
    6
    Frostbite: 67386
    5
    Waterbolt: 9241
    6
    Thunderstorm: 64405
    18
    6
    15
    */
#else
#define input_stream std::cin
#endif

    int T;
    input_stream >> T;
    Wizard Arawn;
    while (T--) {
        Spell* spell = Arawn.cast();
        counterspell(spell);
    }

#ifdef INPUT_FROM_FILE
    input_stream.close();
#endif

    return 0;
}
