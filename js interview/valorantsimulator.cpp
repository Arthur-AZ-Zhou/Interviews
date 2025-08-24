#include <bits/stdc++.h>
using namespace std;

enum class HitLocation { 
    HEAD, 
    BODY, 
    LEG,
};

class Gun {
    public:
        Gun(int reload, int rawdmg, int maxam, double headmult = 3, double legmult = 0.7) 
            : reloadAmt(reload), rawDamage(rawdmg), maxAmmo(maxam), ammo(0),
            headMultiplier(headmult), legMultiplier(legmult) {}

        virtual double calculateDamage(HitLocation loc, int numShots) {
            if (loc == HitLocation::HEAD) {
                return numShots * headMultiplier * rawDamage;
            } else if (loc == HitLocation::BODY) {
                return numShots * rawDamage;
            } else {
                return numShots * legMultiplier * rawDamage;
            }
        }

        virtual void reload() = 0;

        int numBullets() { return ammo; }

    protected:
        int reloadAmt;
        int rawDamage;
        int maxAmmo;

        double headMultiplier;  // shared across all guns
        double legMultiplier;   // shared across all guns

        int ammo;
};

class Pistol : public Gun {
    public: 
        Pistol(int reload, int rawdmg, int maxam, double headmult, double legmult) : Gun(reload, rawdmg, maxam, headmult, legmult) {
            cout << "created pistol!" << endl;
        }

        virtual void reload() {
            if (ammo < maxAmmo) {
                ammo = maxAmmo;
            }
        }

    private:
};

class Rifle : public Gun {
    public:
        Rifle(int reload, int rawdmg, int maxam, double headmult, double legmult) : Gun(reload, rawdmg, maxam, headmult, legmult) {
            cout << "created rifle!" << endl;
        }

        virtual void reload() {
            if (ammo >= maxAmmo) {
                ammo = maxAmmo;
            }
        }

    private:
        
};

class Character {
    public:
        Character(double h = 100) : health(h) {
            secondaryWeapon = new Pistol(7, 26, 12, 3, 0.8);
        }

        void takeDamage(double dmg) {
            health -= dmg;
        }

        void heal(double healAmt) {
            health += healAmt;
        }

        double getHealth() {
            return health;
        }

    private: 
        double health;
        Rifle* primaryWeapon;
        Pistol* secondaryWeapon;
};

int main() {
    // Create a character
    Character player(150);
    cout << "Initial health: " << player.getHealth() << endl;

    // Create a pistol and rifle
    Pistol pistol(7, 25, 12, 3, 0.8);
    Rifle rifle(30, 40, 30, 2.5, 0.6);

    // Test reload mechanics
    cout << "\n--- Reloading pistol ---" << endl;
    pistol.reload();
    cout << "Pistol ammo: " << pistol.numBullets() << "/" << 12 << endl;

    cout << "\n--- Reloading rifle ---" << endl;
    rifle.reload();
    cout << "Rifle ammo: " << rifle.numBullets() << "/" << 30 << endl;

    // Test damage calculations
    cout << "\n--- Shooting Character ---" << endl;
    double dmg1 = pistol.calculateDamage(HitLocation::HEAD, 1);
    player.takeDamage(dmg1);
    cout << "Player took " << dmg1 << " headshot damage. Health: " 
         << player.getHealth() << endl;

    double dmg2 = rifle.calculateDamage(HitLocation::BODY, 2);
    player.takeDamage(dmg2);
    cout << "Player took " << dmg2 << " bodyshot damage. Health: " 
         << player.getHealth() << endl;

    double dmg3 = pistol.calculateDamage(HitLocation::LEG, 3);
    player.takeDamage(dmg3);
    cout << "Player took " << dmg3 << " legshot damage. Health: " 
         << player.getHealth() << endl;

    // Heal test
    cout << "\n--- Healing ---" << endl;
    player.heal(20);
    cout << "Player healed. Health: " << player.getHealth() << endl;
}