#include "db_table.hpp"
const unsigned int kRowGrowthRate = 2;
//unsigned int next_unique_id_ = 0;
//unsigned int row_col_capacity_ = 2;
//std::map<unsigned int, void**> rows_;
//std::vector<std::pair<std::string, DataType>> col_descs_;
//row: id, point save in rows_
//col: point col, initial includes 2 void*, col description in col_descs_
void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc)
{
    col_descs_.push_back(col_desc);
    size_t cur_size = col_descs_.size();
    int f_cp = 0;
    if(row_col_capacity_ <  cur_size )
    {
         row_col_capacity_ = row_col_capacity_ + kRowGrowthRate;
         f_cp = 1;
    }
    cur_size--; // before
    for(auto &it:rows_)
    {
      void **p_row =  it.second;
      if(f_cp)
        {// array, elements type is [void *]
          void** p_nrow = new void*[row_col_capacity_ + kRowGrowthRate];
          for(size_t i =0; i< cur_size; i++)
               p_nrow[i] = p_row[i];
          delete[] p_row;   
           p_row = p_nrow;
          }
       p_row[cur_size+1] = nullptr;//new col, set to nullptr   
    }
}
void DbTable::DeleteColumnByIdx(unsigned int col_idx)
{
    DataType idx_dt = DataType::kString;
    unsigned int i=0;
    if(col_idx > static_cast<unsigned int> (col_descs_.size()-1)) return; 
    for(auto it:col_descs_) //get data type
    {
       if(i == col_idx) 
        {
          idx_dt = it.second;
          break;
        }
       i++;
    }    
    for(auto it:rows_)
      {
        void **p_row =  it.second;
        switch(idx_dt)
        {
        case  DataType::kString:
             {delete[] static_cast<char*>(p_row[col_idx]); 
             break;}
        case DataType::kDouble:
            {delete  static_cast<double*>(p_row[col_idx]);           
             break;}
        case DataType::kInt: 
             {delete  static_cast<int*>(p_row[col_idx]);           
             break;}
        }
      for(i=col_idx; i < static_cast<unsigned int>(col_descs_.size()) - 1; i++)
          p_row[i] = p_row[i+1];//move pointer in row
      p_row[col_descs_.size() - 1] = nullptr;    
    }
    col_descs_.erase(col_descs_.begin() + col_idx);
}
void DbTable::AddRow(const std::initializer_list<std::string>& col_data)
{//etc: data is {"Zhang","101","60.0"}
  void** p_row = new void*[row_col_capacity_];
  int i=0;
  for(auto it_col:col_data)
  { 
        if(col_descs_[i].second == DataType::kString)
             {  p_row[i] = static_cast<void*>(new char[it_col.size()]);
             char *p_ch = static_cast<char*>(p_row[i]); 
             for(size_t j=0;j<it_col.size();j++)
                p_ch[j] = it_col[j];
                }
        else if(col_descs_[i].second == DataType::kDouble)
              p_row[i] = static_cast<void*>(new double(stod(it_col)));
        else if (col_descs_[i].second == DataType::kInt)
             p_row[i] = static_cast<void*>(new int(stoi(it_col)));
    i++;
  }
  rows_.insert({next_unique_id_,p_row});
  next_unique_id_++;
}
void DbTable::DeleteRowById(unsigned int id)
{
   for(auto it:rows_)
      if(it.first == id)
      {
        void **p_row =  it.second;
        int i=0;
        for(auto it_col:col_descs_)
        {
         switch(it_col.second)
         {
        case  DataType::kString:
             {delete[] static_cast<char*>(p_row[i]); 
             break;}
        case DataType::kDouble:
            {delete  static_cast<double*>(p_row[i]);           
             break;}
        case DataType::kInt: 
             {delete  static_cast<int*>(p_row[i]);           
             break;}
         }
         i++;
        }
        delete[] p_row;  
        rows_.erase(it.first);
        break;
      } 
}
DbTable::DbTable(const DbTable& rhs)
{
  // Guard self assignment
    col_descs_ = rhs.col_descs_;
    row_col_capacity_ = rhs.row_col_capacity_;
    next_unique_id_ = rhs.next_unique_id_;
    for(auto it:rhs.rows_)
    {
      void **p_row =  it.second;
      void **p_nrow = new void*[row_col_capacity_];
      int i = 0;
      for(auto it_col : col_descs_)
      {//copy 
       if(p_row[i])
         switch(it_col.second)
         {
        case  DataType::kString:
            { char *p_ch = static_cast<char*>(p_row[i]); 
              char *p_nch = new char[sizeof(p_row[i])]; 
              for(size_t j=0;j<sizeof(p_row[i]);j++)
                p_nch[j] = p_ch[j];
             p_nrow[i] = static_cast<void*>(p_nch);  
             break;}
        case DataType::kDouble:
             p_nrow[i] = static_cast<void*>(new double(*static_cast<double*>(p_row[i])));
             break;
        case DataType::kInt: 
             p_nrow[i] = static_cast<void*>(new int(*static_cast<int*>(p_row[i])));
             break;
        default:
             break;  
        }
        i++;
      }
      rows_.insert({it.first,p_nrow});  
    }
}
DbTable& DbTable::operator=(const DbTable& rhs)
{
    if (this == &rhs)
          return *this;
    this->Clear();      
    this->col_descs_ = rhs.col_descs_;
    this->row_col_capacity_ = rhs.row_col_capacity_;
    this->next_unique_id_ = rhs.next_unique_id_;
    for(auto it:rhs.rows_)
    {
      void **p_row =  it.second;
      void **p_nrow = new void*[this->row_col_capacity_];
      int i = 0;
      for(auto it_col : this->col_descs_)
      {//copy 
       if(p_row[i])
        switch(it_col.second)
        {
          case  DataType::kString:
            { char *p_ch = static_cast<char*>(p_row[i]); 
              char *p_nch = new char[sizeof(p_row[i])]; 
              for(size_t j=0;j<sizeof(p_row[i]);j++)
                p_nch[j] = p_ch[j];
             p_nrow[i] = static_cast<void*>(p_nch);  
             break;}
        case DataType::kDouble:
             p_nrow[i] = static_cast<void*>(new double(*static_cast<double*>(p_row[i])));
             break;
        case DataType::kInt: 
             p_nrow[i] = static_cast<void*>(new int(*static_cast<int*>(p_row[i])));
             break;
        default:
             break;  
        }
        i++;
      }
      this->rows_.insert({it.first ,p_nrow});  
    }
    return *this;
}
void DbTable::Clear()
{
     for(auto it:rows_)
    {
      void **p_row =  it.second;
      int i=0;
      if(p_row[i])
      switch(col_descs_[i].second)
         {
        case  DataType::kString:
             {delete[] static_cast<char*>(p_row[i]); 
             break;}
        case DataType::kDouble:
            {delete  static_cast<double*>(p_row[i]);           
             break;}
        case DataType::kInt: 
             {delete  static_cast<int*>(p_row[i]);           
             break;}
         }
         i++;
      delete[] p_row;    
    } 
    next_unique_id_ = 0;
    row_col_capacity_ = 2;
    col_descs_.clear();
    rows_.clear();
}
DbTable::~DbTable()
{
  Clear();
}

 std::ostream& operator<<(std::ostream& os, const DbTable& table)
 {
      os<<"Table Description"<<std::endl;
      for(auto it_col:table.col_descs_)
      { 
         os<<'\"'<<it_col.first<<'\"' ;//name
         switch(it_col.second)
         {
        case  DataType::kString:
             {os<<" Type is String" << std::endl; break;}
        case DataType::kDouble:
             {os<<" Type is Double" << std::endl; break;}
        case DataType::kInt: 
             {os<<" Type is Int" << std::endl;break;}
         }
      }
      os<<"Table Content"<<std::endl;
      for(auto it:table.rows_)
      { os<<"ID "<<it.first<<std::endl;
       void **p_row = it.second;
       int i = 0; 
      for(auto it_col:table.col_descs_)
      { 
         os<<'\"'<<it_col.first<<'\"' ;//name
         switch(it_col.second)
         {
        case  DataType::kString:
              {if(p_row[i]) os<<static_cast<char*>(p_row[i]) << std::endl; break;}
        case DataType::kDouble:
              {if(p_row[i]) os<<*static_cast<double*>(p_row[i]) << std::endl; break;}
        case DataType::kInt: 
              {if(p_row[i]) os<<*static_cast<int*>(p_row[i]) << std::endl; break;}
         }
        i++; 
      }
      }
      return os;  
 }