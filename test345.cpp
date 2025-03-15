
#include <iostream> 
#include <sqlite3.h> 
#include "Database.h"
#include "PriceCalculations.h"

using namespace std;

void print2DVector(const std::vector<std::vector<std::string>>& vec) {
    for (const auto& row : vec) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv)
{
    
    string dbPath = "C:\\Users\\Ermis\\Documents\\MaterialsDatabase\\testMaterials.db";
    Database database(dbPath);
    /*
    vector<string> s = { "tmx","5.0","mentesedes" };
    database.insert("Material", s);
    vector<vector<string>> data = database.select("*", "Material", "NULL");
    print2DVector(data);
    */
    PriceCalculations calc;
    float pr = calc.furniturePrice(database, "2");
    cout << pr << endl;
}
