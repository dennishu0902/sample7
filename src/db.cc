#include "db.hpp"
#include <stdexcept>
using namespace std;
//  Database 
//         map<string, DbTable*>
//                     
void Database::CreateTable(const std::string& table_name)
{
    for(auto it:tables_)
      if(it.first == table_name) return; 
    tables_.insert({table_name, new DbTable()});
}
void Database::DropTable(const std::string& table_name)
{
   for(auto it:tables_)
      if(it.first == table_name) 
      {
        delete it.second;
        tables_.erase(it.first);
        break;
      }
}
DbTable& Database::GetTable(const std::string& table_name)
{
  for(auto &it:tables_)
     if(it.first == table_name) return *it.second; // Saved is DBtable *, convert to value
  throw std::invalid_argument("Table is noexsited");  //throw exception
}
std::ostream& operator<<(std::ostream& os, const Database& db)
{
  os<<"Database"<<std::endl;
  for(auto it:db.tables_)
    {
    os<<it.first<<"Table content"<<std::endl;
    os<< *it.second <<std::endl;//Table name
    }
  return os;
}