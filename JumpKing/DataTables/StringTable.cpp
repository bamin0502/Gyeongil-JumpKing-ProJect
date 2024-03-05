#include "pch.h"
StringTable::StringTable(Types t):DataTable(t)
{
    
}

StringTable::~StringTable()
{
}

bool StringTable::Load(Languages languages)
{
    static std::vector<std::string> filePathArray=
    {

    };
    currLang=languages;

    table.clear();
    //rapidcsv::Document doc(filePathArray[static_cast<int>(languages)],rapidcsv::LabelParams(0, -1));
    
    /*for(int i=0; i<doc.GetRowCount(); i++)
    {
        std::vector<std::string> pair=doc.GetRow<std::string>(i);
        table.insert({pair[0],pair[1]});
    }*/

    return true;
}

const std::string& StringTable::Get(const std::string& id)
{
    auto find= table.find(id);
    if(find==table.end())
    {
        return "UNKNOWN_STRING";
    }
    return find->second;
}

bool StringTable::Load()
{
    return Load(currLang);
}

void StringTable::Release()
{
    table.clear();
}
