#ifndef DB_HPP
#define DB_HPP

#include <iostream>
#include <map>
#include <string>

#include "db_table.hpp"
using namespace std;
class Database {
 public:
     void CreateTable(const std::string& table_name);
     void DropTable(const std::string& table_name);
     DbTable& GetTable(const std::string& table_name);

     Database() = default;
     Database(const Database& rhs)
     {
      for(auto it:rhs.tables_)
         this->tables_.insert({it.first,it.second});
     }
     Database& operator=(const Database& rhs)
     {
       // Guard self assignment
       if (this == &rhs)
          return *this;
       for(auto it=tables_.rbegin();it!=tables_.rend();it++)
         {
          delete it->second;
          tables_.erase(it->first);
         }
      for(auto it:rhs.tables_)
         this->tables_.insert(std::pair<std::string, DbTable*>(it.first,it.second));   
      return *this;
     };
     ~Database()
     {
     for(auto it=tables_.rbegin();it !=tables_.rend();it++)
        delete it->second;
       tables_.clear();
     };
     friend std::ostream& operator<<(std::ostream& os, const Database& db);

 private:
    std::map<std::string, DbTable*> tables_;  // maps table name -> table
};

#endif