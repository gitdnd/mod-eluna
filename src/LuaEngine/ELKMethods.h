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

    int WarforgeItems(lua_State* L, Player* player)
    {
        int bagMain = Eluna::CHECKVAL<int>(L, 2, true) + INVENTORY_SLOT_BAG_START - 1;
        int slotMain = Eluna::CHECKVAL<int>(L, 3, true) - 1;
        if (bagMain == INVENTORY_SLOT_BAG_START - 1)
        {
            bagMain = 255;
            slotMain += INVENTORY_SLOT_ITEM_START;
        }
        Item* itemMain = player->GetItemByPos(bagMain, slotMain);
        if (itemMain)
        {
            if (itemMain->GetLvlBonus() >= 300)
                return 0;
            ItemTemplate const* templateMain = itemMain->GetTemplate();
            if (templateMain->Class == ITEM_CLASS_ARMOR || templateMain->Class == ITEM_CLASS_WEAPON)
            {
                for (uint8 i = 1; i <= 3; i++)
                {
                    int bag = Eluna::CHECKVAL<int>(L, i * 2 + 2, true) + INVENTORY_SLOT_BAG_START - 1;
                    int slot = Eluna::CHECKVAL<int>(L, i * 2 + 3, true) - 1;
                    if (bag == INVENTORY_SLOT_BAG_START - 1)
                    {
                        bag = 255;
                        slot += INVENTORY_SLOT_ITEM_START;
                    }
                    Item* itemSacrifice = player->GetItemByPos(bag, slot);
                    if (itemSacrifice && itemSacrifice != itemMain && itemSacrifice->GetTemplate() == templateMain)
                    {
                        itemMain->ModLvlBonus(5 + itemSacrifice->GetLvlBonus() / 2);
                        player->DestroyItem(itemSacrifice->GetBagSlot(), itemSacrifice->GetSlot(), true);
                    }
                }
                Eluna::Push(L, itemMain->GetLvlBonus());
                Eluna::Push(L, itemMain->GetGUID().GetCounter());
                return 2;
            }
        }
        return 0;
    }
    int GetBonusILVL(lua_State* L, Player* player)
    {
        uint32 itemId = Eluna::CHECKVAL<int>(L, 2, true);
        if (itemId)
        {
            Eluna::Push(L, player->GetItemByGuidCounter(itemId)->GetLvlBonus());
            Eluna::Push(L, itemId);
            return 2;
        }
        return 0;
    }
};
#endif
