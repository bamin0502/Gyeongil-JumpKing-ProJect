//#include "pch.h"
//#include "ZombieTable.h"
//#include "rapidcsv.h"
//
//DataZombie ZombieTable::Undefined={};
//
//ZombieTable::ZombieTable(DataTable::Types t):DataTable(t)
//{
//}
//
//ZombieTable::~ZombieTable()
//{
//    //Release();
//}
//
//const DataZombie& ZombieTable::Get(Zombie::Types id)
//{
//    auto find= table.find(id);
//    if(find==table.end())
//    {
//        return Undefined;
//    }
//    return find->second;
//}
//
//bool ZombieTable::Load()
//{
//    Release();
//    rapidcsv::Document doc("tables/ZombieTable.csv");
//    for( int i =0; i<doc.GetRowCount(); ++i )
//    {
//        auto row=doc.GetRow<std::string>(i);
//
//        Zombie::Types id=static_cast<Zombie::Types>(std::stoi(row[0]));
//        if(table.find(id) != table.end())
//        {
//            std::cout<<"ZombieTable::Load() error: duplicate id "<<std::endl;
//            return false;
//        }
//        table[id] = DataZombie();
//        table[id].id=id;
//        table[id].nameId=row[1];
//        table[id].descriptionId=row[2];
//        table[id].textureId=row[3];
//        table[id].maxHp=std::stoi(row[4]);
//        table[id].speed=std::stof(row[5]);
//        table[id].damage=std::stoi(row[6]);
//        table[id].attackInterval=std::stof(row[7]);
//    }
//
//    return false;
//}
//
//void ZombieTable::Release()
//{
//    table.clear();
//    
//}
