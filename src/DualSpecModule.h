#ifndef MANGOS_DUALSPEC_MGR_H
#define MANGOS_DUALSPEC_MGR_H

#include "Module.h"
#include "DualSpecConfig.h"

#include "Platform/Define.h"

#include <unordered_map>
#include <map>

class Creature;
class GameObject;
class Item;
class ObjectGuid;
class Player;
class Unit;

struct ActionButton;

enum DualSpecMessages
{
    DUAL_SPEC_DESCRIPTION = 12000,
    DUAL_SPEC_COST_IS,
    DUAL_SPEC_CHANGE_MY_SPEC,
    DUAL_SPEC_NO_GOLD_UNLOCK,
    DUAL_SPEC_ARE_YOU_SURE_BEGIN,
    DUAL_SPEC_ARE_YOU_SURE_END,
    DUAL_SPEC_ALREADY_ON_SPEC,
    DUAL_SPEC_ACTIVATE,
    DUAL_SPEC_RENAME,
    DUAL_SPEC_UNNAMED,
    DUAL_SPEC_ACTIVE,
    DUAL_SPEC_ERR_COMBAT,
    DUAL_SPEC_ERR_INSTANCE,
    DUAL_SPEC_ERR_MOUNT,
    DUAL_SPEC_ERR_DEAD,
    DUAL_SPEC_ERR_UNLOCK,
    DUAL_SPEC_ERR_LEVEL,
    DUAL_SPEC_ACTIVATE_COLOR,
    DUAL_SPEC_RENAME_COLOR,
    DUAL_SPEC_ARE_YOU_SURE_SWITCH,
    DUAL_SPEC_PURCHASE,
    DUAL_SPEC_ERR_ITEM_CREATE,
};

struct DualSpecPlayerTalent
{
    uint8 state;
    uint8 spec;
};

struct DualSpecPlayerStatus
{
    uint8 specCount;
    uint8 activeSpec;
};

typedef std::unordered_map<uint32, DualSpecPlayerTalent> DualSpecPlayerTalentMap;
typedef std::map<uint8, ActionButton> ActionButtonList;

class DualSpecModule : public Module
{
public:
    DualSpecModule() : Module() {}
    DualSpecModuleConfig* CreateConfig() override { return new DualSpecModuleConfig(); }
    DualSpecModuleConfig* GetConfig() override { return (DualSpecModuleConfig*)GetConfigInternal(); }

    // Module Hooks
    void OnInitialize() override;

    // Player Hooks
    bool OnUseItem(Player* player, Item* item) override;
    bool OnGossipHello(Player* player, Creature* creature) override;
    bool OnGossipSelect(Player* player, Unit* creature, uint32 sender, uint32 action, const std::string& code) override;
    bool OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action, const std::string& code) override;
    void OnLearnTalent(Player* player, uint32 spellId) override;
    void OnResetTalents(Player* player, uint32 cost) override;
    void OnPreLoadFromDB(uint32 playerId) override;
    void OnLoadFromDB(Player* player) override;
    void OnSaveToDB(Player* player) override;
    void OnLogOut(Player* player) override;
    void OnCharacterCreated(Player* player) override;
    void OnCharacterDeleted(uint32 playerId) override;
    bool OnLoadActionButtons(Player* player, ActionButtonList& actionButtons) override;
    bool OnSaveActionButtons(Player* player, ActionButtonList& actionButtons) override;

private:
    void LoadPlayerSpec(uint32 playerId);
    uint8 GetPlayerActiveSpec(uint32 playerId) const;
    void SetPlayerActiveSpec(Player* player, uint8 spec);
    uint8 GetPlayerSpecCount(uint32 playerId) const;
    void SetPlayerSpecCount(Player* player, uint8 count);
    void SavePlayerSpec(uint32 playerId);

    void LoadPlayerSpecNames(Player* player);
    const std::string& GetPlayerSpecName(Player* player, uint8 spec) const;
    void SetPlayerSpecName(Player* player, uint8 spec, const std::string& name);
    void SavePlayerSpecNames(Player* player);

    void LoadPlayerTalents(Player* player);
    bool PlayerHasTalent(Player* player, uint32 spellId, uint8 spec);
    DualSpecPlayerTalentMap& GetPlayerTalents(uint32 playerId, int8 spec = -1);
    void AddPlayerTalent(uint32 playerId, uint32 spellId, uint8 spec, bool learned);
    void SavePlayerTalents(uint32 playerId);

    void SendPlayerActionButtons(const Player* player, bool clear) const;

    void ActivatePlayerSpec(Player* player, uint8 spec);
    void AddDualSpecItem(Player* player);

private:
    std::map<uint32, DualSpecPlayerTalentMap[MAX_TALENT_SPECS]> playersTalents;
    std::map<uint32, DualSpecPlayerStatus> playersStatus;
    std::map<uint32, std::string[MAX_TALENT_SPECS]> playersSpecNames;
};

static DualSpecModule dualSpecModule;
#endif