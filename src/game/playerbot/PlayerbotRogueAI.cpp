/*
   Name    : PlayerbotRogueAI.cpp
   Complete: maybe around 28%
   Author    : Natsukawa
   Version : 0.37
   */
#include "PlayerbotRogueAI.h"
#include "PlayerbotMgr.h"

class PlayerbotAI;
PlayerbotRogueAI::PlayerbotRogueAI(Player* const master, Player* const bot, PlayerbotAI* const ai) : PlayerbotClassAI(master, bot, ai)
{
	ADRENALINE_RUSH = m_ai->initSpell(ADRENALINE_RUSH_1);
	SINISTER_STRIKE = m_ai->initSpell(SINISTER_STRIKE_1);
	BACKSTAB = m_ai->initSpell(BACKSTAB_1);
	KICK = m_ai->initSpell(KICK_1);
	FEINT = m_ai->initSpell(FEINT_1);
	GOUGE = m_ai->initSpell(GOUGE_1);
	SPRINT = m_ai->initSpell(SPRINT_1);

	STEALTH = m_ai->initSpell(STEALTH_1);
	VANISH = m_ai->initSpell(VANISH_1);
	EVASION = m_ai->initSpell(EVASION_1);
	HEMORRHAGE = m_ai->initSpell(HEMORRHAGE_1);
	GHOSTLY_STRIKE = m_ai->initSpell(GHOSTLY_STRIKE_1);
	BLIND = m_ai->initSpell(BLIND_1);
	DISTRACT = m_ai->initSpell(DISTRACT_1);
	PREPARATION = m_ai->initSpell(PREPARATION_1);
	PREMEDITATION = m_ai->initSpell(PREMEDITATION_1);
	PICK_POCKET = m_ai->initSpell(PICK_POCKET_1);

	EVISCERATE = m_ai->initSpell(EVISCERATE_1);
	KIDNEY_SHOT = m_ai->initSpell(KIDNEY_SHOT_1);
	SLICE_DICE = m_ai->initSpell(SLICE_AND_DICE_1);
	GARROTE = m_ai->initSpell(GARROTE_1);
	EXPOSE_ARMOR = m_ai->initSpell(EXPOSE_ARMOR_1);
	RUPTURE = m_ai->initSpell(RUPTURE_1);
	CHEAP_SHOT = m_ai->initSpell(CHEAP_SHOT_1);
	AMBUSH = m_ai->initSpell(AMBUSH_1);
	COLD_BLOOD = m_ai->initSpell(COLD_BLOOD_1);

	RECENTLY_BANDAGED = 11196; // first aid check
	// racial
	STONEFORM = m_ai->initSpell(STONEFORM_ALL); // dwarf
	ESCAPE_ARTIST = m_ai->initSpell(ESCAPE_ARTIST_ALL); // gnome
	EVERY_MAN_FOR_HIMSELF = m_ai->initSpell(EVERY_MAN_FOR_HIMSELF_ALL); // human
	SHADOWMELD = m_ai->initSpell(SHADOWMELD_ALL);
	BLOOD_FURY = m_ai->initSpell(BLOOD_FURY_MELEE_CLASSES); // orc
	BERSERKING = m_ai->initSpell(BERSERKING_ALL); // troll
	WILL_OF_THE_FORSAKEN = m_ai->initSpell(WILL_OF_THE_FORSAKEN_ALL); // undead
}

PlayerbotRogueAI::~PlayerbotRogueAI() {}

CombatManeuverReturns PlayerbotRogueAI::DoFirstCombatManeuver(Unit* pTarget)
{
	// There are NPCs in BGs and Open World PvP, so don't filter this on PvP scenarios (of course if PvP targets anyone but tank, all bets are off anyway)
	// Wait until the tank says so, until any non-tank gains aggro or X seconds - whichever is shortest
	if (m_ai->GetCombatOrder() & PlayerbotAI::ORDERS_TEMP_WAIT_TANKAGGRO)
	{
		if (m_WaitUntil > m_ai->CurrentTime() && m_ai->GroupTankHoldsAggro())
		{
			return RETURN_NO_ACTION_OK; // wait it out
		}
		else
		{
			m_ai->ClearGroupCombatOrder(PlayerbotAI::ORDERS_TEMP_WAIT_TANKAGGRO);
		}
	}

	if (m_ai->GetCombatOrder() & PlayerbotAI::ORDERS_TEMP_WAIT_OOC)
	{
		if (m_WaitUntil > m_ai->CurrentTime() && !m_ai->IsGroupInCombat())
			return RETURN_NO_ACTION_OK; // wait it out
		else
			m_ai->ClearGroupCombatOrder(PlayerbotAI::ORDERS_TEMP_WAIT_OOC);
	}

	if (!m_ai)  return RETURN_NO_ACTION_ERROR;
	if (!m_bot) return RETURN_NO_ACTION_ERROR;

	switch (m_ai->GetScenarioType())
	{
	case PlayerbotAI::SCENARIO_PVP_DUEL:
	case PlayerbotAI::SCENARIO_PVP_BG:
	case PlayerbotAI::SCENARIO_PVP_ARENA:
	case PlayerbotAI::SCENARIO_PVP_OPENWORLD:
		return DoFirstCombatManeuverPVP(pTarget);
	case PlayerbotAI::SCENARIO_PVE:
	case PlayerbotAI::SCENARIO_PVE_ELITE:
	case PlayerbotAI::SCENARIO_PVE_RAID:
	default:
		return DoFirstCombatManeuverPVE(pTarget);
		break;
	}

	return RETURN_NO_ACTION_ERROR;
}

CombatManeuverReturns PlayerbotRogueAI::DoFirstCombatManeuverPVE(Unit *pTarget)
{
	if (STEALTH > 0 && !m_bot->HasAura(STEALTH, EFFECT_INDEX_0) && m_ai->CastSpell(STEALTH, *m_bot))
	{

		return RETURN_FINISHED_FIRST_MOVES; // DoNextCombatManeuver handles active stealth
	}
	else if (m_bot->HasAura(STEALTH, EFFECT_INDEX_0))
	{
		m_bot->GetMotionMaster()->MoveFollow(pTarget, 4.5f, m_bot->GetOrientation()); // TODO: this isn't the place for movement code, is it?
		return RETURN_FINISHED_FIRST_MOVES; // DoNextCombatManeuver handles active stealth
	}

	// Not in stealth, can't cast stealth; Off to DoNextCombatManeuver
	return RETURN_NO_ACTION_OK;
}

// TODO: blatant copy of PVE for now, please PVP-port it
CombatManeuverReturns PlayerbotRogueAI::DoFirstCombatManeuverPVP(Unit *pTarget)
{
	if (STEALTH > 0 && !m_bot->HasAura(STEALTH, EFFECT_INDEX_0) && m_ai->CastSpell(STEALTH, *m_bot))
	{

		return RETURN_FINISHED_FIRST_MOVES; // DoNextCombatManeuver handles active stealth
	}
	else if (m_bot->HasAura(STEALTH, EFFECT_INDEX_0))
	{
		m_bot->GetMotionMaster()->MoveFollow(pTarget, 4.5f, m_bot->GetOrientation()); // TODO: this isn't the place for movement code, is it?
		return RETURN_FINISHED_FIRST_MOVES; // DoNextCombatManeuver handles active stealth
	}

	// Not in stealth, can't cast stealth; Off to DoNextCombatManeuver
	return RETURN_NO_ACTION_OK;
}

CombatManeuverReturns PlayerbotRogueAI::DoNextCombatManeuver(Unit *pTarget)
{
	// Face enemy, make sure bot is attacking
	if (!m_bot->HasInArc(M_PI_F, pTarget))
		m_bot->SetFacingTo(m_bot->GetAngle(pTarget));
	switch (m_ai->GetScenarioType())
	{
	case PlayerbotAI::SCENARIO_PVP_DUEL:
	case PlayerbotAI::SCENARIO_PVP_BG:
	case PlayerbotAI::SCENARIO_PVP_ARENA:
	case PlayerbotAI::SCENARIO_PVP_OPENWORLD:
		return DoNextCombatManeuverPVP(pTarget);
	case PlayerbotAI::SCENARIO_PVE:
	case PlayerbotAI::SCENARIO_PVE_ELITE:
	case PlayerbotAI::SCENARIO_PVE_RAID:
	default:
		return DoNextCombatManeuverPVE(pTarget);
		break;
	}

	return RETURN_NO_ACTION_ERROR;
}

CombatManeuverReturns PlayerbotRogueAI::DoNextCombatManeuverPVE(Unit *pTarget)
{
	if (!pTarget) return RETURN_NO_ACTION_ERROR;
	if (!m_ai)    return RETURN_NO_ACTION_ERROR;
	if (!m_bot)   return RETURN_NO_ACTION_ERROR;

	Unit* pVictim = pTarget->getVictim();
	bool meleeReach = m_bot->CanReachWithMeleeAttack(pTarget);
	//use healing posion
	if (m_ai->GetHealthPercent() < 20)
	{
		Item* healingPosion = m_ai->FindHealingPoison();
		if (healingPosion)
			m_ai->UseItem(healingPosion);
	}
	if (m_bot->getRace() == RACE_UNDEAD && (m_bot->HasAuraType(SPELL_AURA_MOD_FEAR) || m_bot->HasAuraType(SPELL_AURA_MOD_CHARM)) && !m_bot->HasSpellCooldown(WILL_OF_THE_FORSAKEN) && m_ai->CastSpell(WILL_OF_THE_FORSAKEN, *m_bot))
		return RETURN_CONTINUE;
	if (m_bot->getRace() == RACE_TROLL && !m_bot->HasSpellCooldown(BERSERKING) && m_ai->CastSpell(BERSERKING, *m_bot))
		return RETURN_CONTINUE;
	if (m_bot->getRace() == RACE_ORC && !m_bot->HasAura(BLOOD_FURY, EFFECT_INDEX_0) && !m_bot->HasSpellCooldown(BLOOD_FURY) && m_ai->CastSpell(BLOOD_FURY, *m_bot))
		return RETURN_CONTINUE;
	// TODO: make this work better...
	/*if (pVictim)
	   {
	   if( pVictim!=m_bot && !m_bot->hasUnitState(UNIT_STAT_FOLLOW) && !pTarget->isInBackInMap(m_bot,10) ) {
	   m_ai->TellMaster( "getting behind target" );
	   m_bot->GetMotionMaster()->Clear( true );
	   m_bot->GetMotionMaster()->MoveFollow( pTarget, 1, 2*M_PI );
	   }
	   else if( pVictim==m_bot && m_bot->hasUnitState(UNIT_STAT_FOLLOW) )
	   {
	   m_ai->TellMaster( "chasing attacking target" );
	   m_bot->GetMotionMaster()->Clear( true );
	   m_bot->GetMotionMaster()->MoveChase( pTarget );
	   }
	   }*/
	// If bot is stealthed: pre-combat actions
	if (m_bot->HasAura(STEALTH, EFFECT_INDEX_0))
		 {
		if (PICK_POCKET > 0 && (pTarget->GetCreatureTypeMask() & CREATURE_TYPEMASK_HUMANOID_OR_UNDEAD) != 0 && m_ai->PickPocket(pTarget))
			 return RETURN_CONTINUE;
		if (PREMEDITATION > 0 && m_ai->CastSpell(PREMEDITATION, *pTarget))
			 return RETURN_CONTINUE;
		if (AMBUSH > 0 && !pTarget->HasInArc(M_PI_F, m_bot) && m_ai->CastSpell(AMBUSH, *pTarget))
			 return RETURN_CONTINUE;
		if (CHEAP_SHOT > 0 && !pTarget->HasAura(CHEAP_SHOT, EFFECT_INDEX_0) && m_ai->CastSpell(CHEAP_SHOT, *pTarget))
			 return RETURN_CONTINUE;
		if (GARROTE > 0 && !pTarget->HasInArc(M_PI_F, m_bot) && m_ai->CastSpell(GARROTE, *pTarget))
			 return RETURN_CONTINUE;
		
			        // No appropriate action found, remove stealth
			m_bot->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
		return RETURN_CONTINUE;
		}
	
		    //Used to determine if this bot has highest threat
		Unit* newTarget = m_ai->FindAttacker((PlayerbotAI::ATTACKERINFOTYPE) (PlayerbotAI::AIT_VICTIMSELF | PlayerbotAI::AIT_HIGHESTTHREAT), m_bot);
	if (newTarget && !(m_ai->GetCombatOrder() & PlayerbotAI::ORDERS_TANK)) // TODO: && party has a tank
		 {
		        // Aggroed by an elite
			if (m_ai->IsElite(newTarget))
			 {
			if (EVASION > 0 && m_ai->GetHealthPercent() <= 35 && !m_bot->HasSpellCooldown(EVASION) && !m_bot->HasAura(EVASION, EFFECT_INDEX_0) && m_ai->CastSpell(EVASION))
				 return RETURN_CONTINUE;
			if (BLIND > 0 && m_ai->GetHealthPercent() <= 30 && m_ai->HasSpellReagents(BLIND) && !pTarget->HasAura(BLIND, EFFECT_INDEX_0) && m_ai->CastSpell(BLIND, *pTarget))
				 return RETURN_CONTINUE;
			if (VANISH > 0 && m_ai->GetHealthPercent() <= 20 && !m_bot->HasSpellCooldown(VANISH) && !m_bot->HasAura(FEINT, EFFECT_INDEX_0) && m_ai->CastSpell(VANISH))
				 {
				m_ai->SetIgnoreUpdateTime(11);
				return RETURN_CONTINUE;
				}
			if (FEINT > 0 && !m_bot->HasSpellCooldown(FEINT) && m_ai->CastSpell(FEINT))
				 return RETURN_CONTINUE;
			if (PREPARATION > 0 && !m_bot->HasSpellCooldown(PREPARATION) && (m_bot->HasSpellCooldown(EVASION) || m_bot->HasSpellCooldown(VANISH)) && m_ai->CastSpell(PREPARATION))
				 return RETURN_CONTINUE;
			}
		
			        // Default: Gouge the target
			if (GOUGE > 0 && !pTarget->HasAura(GOUGE, EFFECT_INDEX_0) && m_ai->CastSpell(GOUGE, *pTarget))
			 return RETURN_CONTINUE;
		}
	
		    // Buff bot with cold blood if available
		    // This buff is done after the stealth and aggro management code because we don't want to give starting extra damage (= extra threat) to a bot
		    // as it is obviously not soloing his/her target
		if (COLD_BLOOD > 0 && !m_bot->HasAura(COLD_BLOOD, EFFECT_INDEX_0) && !m_bot->HasSpellCooldown(COLD_BLOOD) && m_ai->CastSpell(COLD_BLOOD, *m_bot))
		 return RETURN_CONTINUE;
	
	// Rogue like behaviour ^^
	/*if (VANISH > 0 && GetMaster()->isDead()) { //Causes the server to crash :( removed for now.
		m_bot->AttackStop();
		m_bot->RemoveAllAttackers();
		m_ai->CastSpell(VANISH);
		//m_bot->RemoveAllSpellCooldown();
		m_ai->TellMaster("AttackStop, CombatStop, Vanish");
		}*/
		// we fight in melee, target is not in range, skip the next part!
		if (!meleeReach)
			 return RETURN_CONTINUE;
		
			    // If target is elite and wounded: use adrenaline rush to finish it quicker
			if (ADRENALINE_RUSH > 0 && m_ai->IsElite(pTarget) && pTarget->GetHealthPercent() < 50 && !m_bot->HasAura(ADRENALINE_RUSH, EFFECT_INDEX_0) && !m_bot->HasSpellCooldown(ADRENALINE_RUSH) && m_ai->CastSpell(ADRENALINE_RUSH, *m_bot))
			 return RETURN_CONTINUE;
		
			    // Bot's target is casting a spell: try to interrupt it
			if (pTarget->IsNonMeleeSpellCasted(true))
			 {
			if (KIDNEY_SHOT > 0 && !m_bot->HasSpellCooldown(KIDNEY_SHOT) && m_bot->GetComboPoints() >= 1 && m_ai->CastSpell(KIDNEY_SHOT, *pTarget))
				 return RETURN_CONTINUE;
			else if (KICK > 0 && !m_bot->HasSpellCooldown(KICK) && m_ai->CastSpell(KICK, *pTarget))
				 return RETURN_CONTINUE;
			}
		
			    // Finishing moves
			    // Bot will try to activate finishing move at 4 combos points (5 combos points case will be bonus)
			    // TODO : define combo points treshold depending on target rank and HP
			if (m_bot->GetComboPoints() >= 4)
			 {
			Creature * pCreature = (Creature*)pTarget;
			        // wait for energy
				if (m_ai->GetEnergyAmount() < 25 && (KIDNEY_SHOT || SLICE_DICE || EXPOSE_ARMOR || RUPTURE))
				 return RETURN_NO_ACTION_OK;
			
				        // If target is elite Slice & Dice is a must have
				if (SLICE_DICE > 0 && m_ai->IsElite(pTarget) && !m_bot->HasAura(SLICE_DICE, EFFECT_INDEX_1) && m_ai->CastSpell(SLICE_DICE, *pTarget)) // 25 energy (checked above)
				 return RETURN_CONTINUE;
			
				        // If target is a warrior or paladin type (high armor): expose its armor
				if (EXPOSE_ARMOR > 0 && pCreature && pCreature->GetCreatureInfo()->UnitClass != 8 && !pTarget->HasAura(EXPOSE_ARMOR, EFFECT_INDEX_0) && m_ai->CastSpell(EXPOSE_ARMOR, *pTarget)) // 25 energy (checked above)
				 return RETURN_CONTINUE;
			
				if (RUPTURE > 0 && !pTarget->HasAura(RUPTURE, EFFECT_INDEX_0) && m_ai->CastSpell(RUPTURE, *pTarget)) // 25 energy (checked above)
				 return RETURN_CONTINUE;
			
				        // default combo action or if other combo action is unavailable/failed
				        // wait for energy
				if (m_ai->GetEnergyAmount() < 35 && EVISCERATE > 0)
				 return RETURN_NO_ACTION_OK;
			if (EVISCERATE > 0 && m_ai->CastSpell(EVISCERATE, *pTarget))
				 return RETURN_CONTINUE;
			
				       // failed for some (non-energy related) reason, fall through to normal attacks to maximize DPS
				}
		
			    // Combo generating or damage increasing attacks
			if (HEMORRHAGE > 0 && !pTarget->HasAura(HEMORRHAGE, EFFECT_INDEX_2) && m_ai->CastSpell(HEMORRHAGE, *pTarget))
			 return RETURN_CONTINUE;
		if (BACKSTAB > 0 && !pTarget->HasInArc(M_PI_F, m_bot) && m_ai->CastSpell(BACKSTAB, *pTarget))
			 return RETURN_CONTINUE;
		if (GHOSTLY_STRIKE > 0 && !m_bot->HasSpellCooldown(GHOSTLY_STRIKE) && m_ai->CastSpell(GHOSTLY_STRIKE, *pTarget))
			 return RETURN_CONTINUE;
		if (SINISTER_STRIKE > 0 && m_ai->CastSpell(SINISTER_STRIKE, *pTarget))
			 return RETURN_CONTINUE;
		
			return RETURN_NO_ACTION_OK;
		
} // end DoNextCombatManeuver

CombatManeuverReturns PlayerbotRogueAI::DoNextCombatManeuverPVP(Unit* pTarget)
 {
	if (!pTarget) return RETURN_NO_ACTION_ERROR;
	if (!m_ai)    return RETURN_NO_ACTION_ERROR;
	if (!m_bot)   return RETURN_NO_ACTION_ERROR;
	
		Unit* pVictim = pTarget->getVictim();
	bool meleeReach = m_bot->CanReachWithMeleeAttack(pTarget);
	// decide what to do:
	if (m_bot->HasAura(STEALTH, EFFECT_INDEX_0))
		SpellSequence = RogueStealth;
	else if (pTarget->IsNonMeleeSpellCasted(true))
		SpellSequence = RogueSpellPreventing;
	else if (pVictim == m_bot && m_ai->GetHealthPercent() < 40)
		SpellSequence = RogueThreat;
	else
		SpellSequence = RogueCombat;

	// we fight in melee, target is not in range, skip the next part!
	if (!meleeReach)
		return RETURN_CONTINUE;

	std::ostringstream out;
	switch (SpellSequence)
	{
	case RogueStealth:
		
		if (PREMEDITATION > 0 && m_ai->CastSpell(PREMEDITATION, *pTarget))
			return RETURN_CONTINUE;
		if (AMBUSH > 0 && m_ai->CastSpell(AMBUSH, *pTarget))
			return RETURN_CONTINUE;
		if (CHEAP_SHOT > 0 && !pTarget->HasAura(CHEAP_SHOT, EFFECT_INDEX_0) && m_ai->CastSpell(CHEAP_SHOT, *pTarget))
			return RETURN_CONTINUE;
		if (GARROTE > 0 && m_ai->CastSpell(GARROTE, *pTarget))
			return RETURN_CONTINUE;

		// No appropriate action found, remove stealth
		m_bot->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
		return RETURN_CONTINUE;

	case RogueThreat:
		if (GOUGE > 0 && !pTarget->HasAura(GOUGE, EFFECT_INDEX_0) && m_ai->CastSpell(GOUGE, *pTarget))
			return RETURN_CONTINUE;
		if (EVASION > 0 && m_ai->GetHealthPercent() <= 35 && !m_bot->HasAura(EVASION, EFFECT_INDEX_0) && m_ai->CastSpell(EVASION))
			return RETURN_CONTINUE;
		if (BLIND > 0 && m_ai->GetHealthPercent() <= 30 && !pTarget->HasAura(BLIND, EFFECT_INDEX_0) && m_ai->CastSpell(BLIND, *pTarget))
			return RETURN_CONTINUE;
		if (FEINT > 0 && m_ai->GetHealthPercent() <= 25 && m_ai->CastSpell(FEINT))
			return RETURN_CONTINUE;
		if (VANISH > 0 && m_ai->GetHealthPercent() <= 20 && !m_bot->HasAura(FEINT, EFFECT_INDEX_0) && m_ai->CastSpell(VANISH))
			return RETURN_CONTINUE;
		if (PREPARATION > 0 && m_ai->CastSpell(PREPARATION))
			return RETURN_CONTINUE;
		
		break;

	case RogueSpellPreventing:
		if (KIDNEY_SHOT > 0 && m_bot->GetComboPoints() >= 2 && m_ai->CastSpell(KIDNEY_SHOT, *pTarget))
			return RETURN_CONTINUE;
		else if (KICK > 0 && m_ai->CastSpell(KICK, *pTarget))
			return RETURN_CONTINUE;
		// break; // No action? Go combat!

	case RogueCombat:
	default:
		if (m_bot->GetComboPoints() >= 5)
		{
			// wait for energy
			if (m_ai->GetEnergyAmount() < 25 && (KIDNEY_SHOT || SLICE_DICE || EXPOSE_ARMOR))
				return RETURN_NO_ACTION_OK;

			switch (pTarget->getClass())
			{
			case CLASS_SHAMAN:
				if (KIDNEY_SHOT > 0 && m_ai->CastSpell(KIDNEY_SHOT, *pTarget)) // 25 energy (checked above)
					return RETURN_CONTINUE;
				break;

			case CLASS_WARLOCK:
			case CLASS_HUNTER:
				if (SLICE_DICE > 0 && m_ai->CastSpell(SLICE_DICE, *pTarget)) // 25 energy (checked above)
					return RETURN_CONTINUE;
				break;

			case CLASS_WARRIOR:
			case CLASS_PALADIN:
				if (EXPOSE_ARMOR > 0 && !pTarget->HasAura(EXPOSE_ARMOR, EFFECT_INDEX_0) && m_ai->CastSpell(EXPOSE_ARMOR, *pTarget)) // 25 energy (checked above)
					return RETURN_CONTINUE;
				break;


			case CLASS_MAGE:
			case CLASS_PRIEST:
				if (RUPTURE > 0 && m_ai->CastSpell(RUPTURE, *pTarget)) // 25 energy (checked above)
					return RETURN_CONTINUE;
				break;

			case CLASS_ROGUE:
			case CLASS_DRUID:
			default:
				break; // fall through to below
			}

			// default combo action for rogue/druid or if other combo action is unavailable/failed
			// wait for energy
			if (m_ai->GetEnergyAmount() < 35 && EVISCERATE)
				return RETURN_NO_ACTION_OK;
			if (EVISCERATE > 0 && m_ai->CastSpell(EVISCERATE, *pTarget))
				return RETURN_CONTINUE;

			// failed for some (non-energy related) reason, fall through to normal attacks to maximize DPS
		}

		if (CHEAP_SHOT > 0 && !pTarget->HasAura(CHEAP_SHOT, EFFECT_INDEX_0) && m_ai->CastSpell(CHEAP_SHOT, *pTarget))
			return RETURN_CONTINUE;
		if (AMBUSH > 0 && m_ai->CastSpell(AMBUSH, *pTarget))
			return RETURN_CONTINUE;
		if (GARROTE > 0 && m_ai->CastSpell(GARROTE, *pTarget))
			return RETURN_CONTINUE;
		if (BACKSTAB > 0 && pTarget->isInBackInMap(m_bot, 1) && m_ai->CastSpell(BACKSTAB, *pTarget))
			return RETURN_CONTINUE;
		
		if (SINISTER_STRIKE > 0 && m_ai->CastSpell(SINISTER_STRIKE, *pTarget))
			return RETURN_CONTINUE;
		if (GHOSTLY_STRIKE > 0 && m_ai->CastSpell(GHOSTLY_STRIKE, *pTarget))
			return RETURN_CONTINUE;
		if (HEMORRHAGE > 0 && m_ai->CastSpell(HEMORRHAGE, *pTarget))
			return RETURN_CONTINUE;
		
		break;
	}

	return RETURN_NO_ACTION_OK;

}

void PlayerbotRogueAI::DoNonCombatActions()
{
	if (!m_ai)  return;
	if (!m_bot) return;
	m_bot->RemoveAllSpellCooldown();
	// remove stealth
	if (m_bot->HasAura(STEALTH))
		m_bot->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

	if (EatDrinkBandage())
		return;
	// hp/mana check
	//if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
	//m_bot->SetStandState(UNIT_STAND_STATE_STAND);

	// Search and apply poisons to weapons
	// Mainhand ...
	Item * poison, *weapon;
	weapon = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
	if (weapon && weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 0)
	{
		poison = m_ai->FindConsumable(INSTANT_POISON_DISPLAYID);
		if (!poison)
			poison = m_ai->FindConsumable(WOUND_POISON_DISPLAYID);
		if (!poison)
			poison = m_ai->FindConsumable(DEADLY_POISON_DISPLAYID);
		if (poison)
		{
			m_ai->UseItem(poison, EQUIPMENT_SLOT_MAINHAND);
			m_ai->SetIgnoreUpdateTime(5);
		}
	}
	//... and offhand
	weapon = m_bot->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
	if (weapon && weapon->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT) == 0)
	{
		poison = m_ai->FindConsumable(DEADLY_POISON_DISPLAYID);
		if (!poison)
			poison = m_ai->FindConsumable(WOUND_POISON_DISPLAYID);
		if (!poison)
			poison = m_ai->FindConsumable(INSTANT_POISON_DISPLAYID);
		if (poison)
		{
			m_ai->UseItem(poison, EQUIPMENT_SLOT_OFFHAND);
			m_ai->SetIgnoreUpdateTime(5);
		}
	}
} // end DoNonCombatActions
