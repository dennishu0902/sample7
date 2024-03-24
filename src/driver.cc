#include <iostream>

#include "db.hpp"
#include "db_table.hpp"

int main() {
  // write informal tests here
  Database my_db;
  my_db.CreateTable("users");
  my_db.CreateTable("works");
 
  DbTable& tb_users=my_db.GetTable("users");
  tb_users.AddColumn({"name",DataType::kString});
  tb_users.AddColumn({"id",DataType::kInt});
  tb_users.AddColumn({"score",DataType::kDouble});
  tb_users.AddRow({"Zhang","1","30.1"});
  tb_users.AddRow({"Li","2","30.2"});
  tb_users.AddRow({"Wang","3","30.3"});
  tb_users.AddColumn({"tall",DataType::kDouble});
  DbTable& tb_works=my_db.GetTable("works");
  tb_works.AddColumn({"wo",DataType::kString});
  tb_works.AddColumn({"price",DataType::kDouble});
  tb_works.AddColumn({"so",DataType::kInt});
  tb_works.AddRow({"W11","10.13","301"});
  tb_works.AddRow({"W12","23.2","302"});
  tb_works.AddRow({"W13","33.1","303"});
  //my_db.CreateTable("works");
  //my_db.CreateTable("orders");
  std::cout<<"Output DB"<<std::endl; 
  std::cout<<my_db<<std::endl;
  std::cout<<"Output Table"<<std::endl; 
//  std::cout<<tb_users<<std::endl;
  my_db.CreateTable("orders");
  my_db.CreateTable("works");
  my_db.CreateTable("users");
  my_db.CreateTable("asers");  
  //std::cout<<my_db.GetTable("orders")<<std::endl;
 /// std::cout<<"Output DB"<<std::endl; 
  //std::cout<<my_db<<std::endl;  
 // std::cout<<tb_users<<std::endl;
  DbTable tb_users_cp(tb_users);
 // std::cout<<tb_users_cp<<std::endl;
  tb_users.AddRow({"Song","4","30.3"});
 // std::cout<<tb_users<<std::endl; 
  swap(tb_users,tb_users_cp);
  swap(tb_users,tb_works);
 // std::cout<<tb_users_cp<<std::endl; 
  tb_users.DeleteColumnByIdx(1);
  tb_users.DeleteRowById(1);
   tb_users.DeleteRowById(4);
  tb_users.DeleteColumnByIdx(2);
  tb_users.DeleteColumnByIdx(3);
  tb_users.DeleteRowById(3);
//  std::cout<<tb_users<<std::endl; 

  std::cout<<"Output DB"<<std::endl; 
  std::cout<<my_db<<std::endl;  
  return 0;
}