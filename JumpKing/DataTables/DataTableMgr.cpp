#include "pch.h"

DataTableMgr::DataTableMgr()
{
}

DataTableMgr::~DataTableMgr()
{
    Release();
}

void DataTableMgr::Init()
{
    Release();

    DataTable* table = new StringTable(DataTable::Types::String);
    table->Load();
    tables.insert({DataTable::Types::String,table});

    //table = new ZombieTable(DataTable::Types::Zombie);
    //table->Load();
    //tables.insert({DataTable::Types::Zombie,table});
}

void DataTableMgr::Release()
{
    for(auto pair:tables)
    {
        delete pair.second;
    }
    tables.clear();
}
