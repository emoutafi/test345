#pragma once

#include <string>
#include <sqlite3.h>
#include <iostream>
#include <vector>
#include "Database.h"

using namespace std;

class PriceCalculations
{
public:
	float furniturePrice(Database db, string furniture) {
		string s = "m.id, m.price";
		string fr = "furniture_materials fm JOIN Material m ON fm.Material_id = m.id";
		string wh = "fm.furniture_id = " + furniture;
		vector<vector<string>> materialAndPrice = db.select(s, fr, wh);

		float price = 0;
		
		for (int i = 0; i < materialAndPrice.size(); i++) {
			price += stof(materialAndPrice[i][0]) * stof(materialAndPrice[i][1]);
		}

		return price;
	}
};

