#pragma once

#include <string>
#include <sqlite3.h>
#include <iostream>
#include <vector>

using namespace std;

class Database
{
private:
	string url;
    
public:
	Database(string url) {
		this->url = url;
	}

    std::string getUrl() {
        return url;
    }

    static int callback(void* data, int argc, char** argv, char** azColName)
    {
        auto* results = static_cast<std::vector<std::vector<std::string>>*>(data);
        std::vector<std::string> row;
        for (int i = 0; i < argc; i++) {
            
            row.push_back(argv[i] ? argv[i] : "NULL");
        }
        results->push_back(row);
        return 0;
    }

    vector<vector<string>> select(std::string select, std::string from, std::string condition) {
        sqlite3* DB;
        vector<vector<string>> results;
        int exit = 0;
        exit = sqlite3_open(url.c_str(), &DB);
        

        string sql;

        if (condition == "NULL") {
            sql = "SELECT "+ select + " FROM "+ from +"; ";
        }
        else {
            sql = "SELECT " + select + " FROM " + from + " WHERE "+ condition +"; ";
        }
        if (exit) {
            std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
            return results;
            
        }
        else
            std::cout << "Opened Database Successfully!" << std::endl;

        int rc = sqlite3_exec(DB, sql.c_str(), callback, &results, NULL);

        if (rc != SQLITE_OK)
            cerr << "Error SELECT" << endl;
        else {
            cout << "Operation OK!" << endl;
        }

        sqlite3_close(DB);
        return results;
        
    }

    int maxID(string select, string from) {
        sqlite3* DB;
        sqlite3_stmt* stmt;
        int max_id = -1;  

        
        if (sqlite3_open(url.c_str(), &DB) != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << std::endl;
            return max_id;
        }

        
        std::string sql = "SELECT MAX(" + select + ") FROM " + from + ";";

        
        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
            sqlite3_close(DB);
            return max_id;
        }

        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            max_id = sqlite3_column_int(stmt, 0);
        }

        
        sqlite3_finalize(stmt);
        sqlite3_close(DB);

        return max_id;

    }

    void insert(string tableName, vector<string> values){
        sqlite3* DB;
        sqlite3_stmt* stmt;
        char* messaggeError;
        int exit = sqlite3_open(url.c_str(), &DB);

        int newId = maxID("id", tableName) + 1;
        
        string sql = "INSERT INTO " + tableName + " VALUES (?";
        for (size_t i = 0; i < values.size(); ++i) {
            sql += ", ?";
        }
        sql += ");";

        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
            sqlite3_close(DB);
            return;
        }

        sqlite3_bind_int(stmt, 1, newId);
        for (size_t i = 0; i < values.size(); ++i) {
            sqlite3_bind_text(stmt, i + 2, values[i].c_str(), -1, SQLITE_STATIC);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error executing insert: " << sqlite3_errmsg(DB) << endl;
        }
        else {
            cout << "Record inserted successfully!" << endl;
        }

        sqlite3_finalize(stmt);
        sqlite3_close(DB);
    }

    float getPrice(int id) {
        sqlite3* DB;
        sqlite3_stmt* stmt;
        float price = -1;


        if (sqlite3_open(url.c_str(), &DB) != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(DB) << std::endl;
            return price;
        }


        std::string sql = "SELECT price FROM Material WHERE id="+ to_string(id) + "; ";


        if (sqlite3_prepare_v2(DB, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << std::endl;
            sqlite3_close(DB);
            return price;
        }


        if (sqlite3_step(stmt) == SQLITE_ROW) {
            price = sqlite3_column_double(stmt, 0);
        }


        sqlite3_finalize(stmt);
        sqlite3_close(DB);

        return price;
    }
};

