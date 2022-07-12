/*
* Copyright (C) 2010 - 2016 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef ELKMETHODS_H
#define ELKMETHODS_H

#include "GameTime.h"
#include <DataStores/DBCStores.cpp>
 
namespace LuaELK
{
    /**
     * Attempts to develop a Talent
     */ 
    int DevelopTalent(lua_State* L, Player* player)
    { 
        int talent = Eluna::CHECKVAL<int>(L, 2, true);
        if(talent)
            player->LearnTalent(talent, 0);

        return 0;
    }
    /**
     * Attempts to develop a Talent
     */
    int GetTalentDevelopment(lua_State* L, Player* player)
    {
        int talentId = Eluna::CHECKVAL<int>(L, 2, true);
        if (talentId)
        {
            TalentSpellPosMap::iterator talent = std::find_if(std::begin(sTalentSpellPosMap), std::end(sTalentSpellPosMap),
                [&](std::pair<uint32, TalentSpellPos> const& talent) { return talent.second.talent_id == talentId; });
            if (talent == sTalentSpellPosMap.end())
                return 1;
            auto find = player->GetTalentMap().find((*talent).first);
            if (find == player->GetTalentMap().end())
                return 1;
            auto str = std::to_string(find->second->development) + "." + std::to_string(talentId);
            Eluna::Push(L, str); 
            return 1;
        }

        return 0;
    }

    int GetRuneState(lua_State* L, Player* player)
    {
        
        for (uint32 i = 0; i < MAX_RUNES; ++i)
        {
            Eluna::Push(L, player->GetRuneCooldown(i));
            Eluna::Push(L, player->GetRuneStartCooldown(i));
        } 
        return MAX_RUNES * 2;
    }
};
#endif
