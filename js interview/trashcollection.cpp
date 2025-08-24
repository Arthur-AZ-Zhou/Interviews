#include <bits/stdc++.h>
using namespace std;

struct Object {
    int id;
    vector<Object*> references; 
    bool marked;

    Object(int id) : id(id), marked(false) {}
};

class GarbageCollector {
public:
    GarbageCollector(vector<Object*>& allObjects, vector<Object*>& roots)
        : allObjects(allObjects), roots(roots) {}

    void mark() {
        for (Object* root : roots) {
            if (!root->marked) { //do not want to double count
                markFrom(root); 
            }
        }
    }

    void sweep() {
        vector<Object*> newAllObjects;

        for (Object* object : allObjects) {
            if (!object->marked) { //reality speaking this should probably be a list so we don't have to allocate an entirely new array
                delete object;
            } else {
                newAllObjects.push_back(object);
            }
        }

        allObjects = newAllObjects;
        
        for (Object* object : allObjects) {
            object->marked = false;
        }
    }

    void collect() {
        mark();
        sweep();
    }

    void printObjects() {
        cout << "Remaining objects: ";

        for (auto obj : allObjects) {
            cout << obj->id << " ";
        }

        cout << endl;
    }

private:
    vector<Object*>& allObjects;
    vector<Object*>& roots;

    void markFrom(Object* obj) {
        if (obj->marked) {
            return;
        }

        obj->marked = true;

        for (Object* referenceObject : obj->references) {
            markFrom(referenceObject);
        }
    }
};

int main() {
    // Example: build objects and references
    Object* a = new Object(1);
    Object* b = new Object(2);
    Object* c = new Object(3);
    Object* d = new Object(4);

    a->references.push_back(b);
    b->references.push_back(c);
    // d is unreachable

    vector<Object*> objects = {a, b, c, d};
    vector<Object*> roots = {a};

    GarbageCollector gc(objects, roots);

    gc.collect();
    gc.printObjects(); // Should not include d

    return 0;
}
