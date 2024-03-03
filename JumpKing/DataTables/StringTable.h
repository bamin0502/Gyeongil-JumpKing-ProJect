#pragma once
#include "DataTable.h"

class StringTable: public DataTable
{
    
protected:
    std::unordered_map<std::string,std::string> table;
    Languages currLang=Languages::KOREAN;
public:
    StringTable(Types t);
    ~StringTable() override;

    bool Load(Languages languages);

    const std::string& Get(const std::string& id);
    
    bool Load() override;
    void Release() override;
    
};
