#include "UpdateBools.h"
#include "Globals.h"
#include "Constants.h"
#include "Logger.h"
#include "Levels/Overworld.h"

void updateBools(const std::shared_ptr<NPC>& NPC)
{
	//Check if the first quest is completed
	if (g_RDF_isInvestigationAssigned == false) 
	{
		//Check quest completion parameters if current NPC is The King
		if (NPC->getName() == STRING_KING_NAME) 
		{
			//Check if quest is completed, set parameters for completed quest
			if (NPC->mHappiness >= 0.59 && NPC->mHostility <= 0.31)
			{
				LOG_INFO("<<QUEST>> First quest has been completed.");

				//Set quest state to finished
				g_RDF_isInvestigationAssigned = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_2_USER_PROMPT;

				//Set required RDF strings
				gKing->mRDFDynamicContext = RDF_INVESTIGATION_ASSIGNED_TRUE;
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_WOMAN_FALSE;

				//Set required emotion attributes
				gInnkeeper->mHappiness = 0.5f;
				gInnkeeper->mTrust = 0.2f;
				gInnkeeper->mHostility = 0.4f;
			}
		}
	}
	//If the previous quest is done, check for the second quest
	else if (g_RDF_innkeeperToldAboutWoman == false) 
	{
		//Check quest completion parameters if current NPC is The Innkeeper
		if (NPC->getName() == STRING_INNKEEPER_NAME)
		{
			//Check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.49)
			{
				LOG_INFO("<<QUEST>> Second quest has been completed.");

				//Set quest state to finished
				g_RDF_innkeeperToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_2_ADDITIONAL_USER_PROMPT;
				//Change to QUEST_3 upon entering Maqruis' house

				//Set required RDF strings
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_WOMAN_TRUE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_WOMAN_FALSE;

				//Set required emotion attributes
				gMarquis->mHappiness = 0.6f;
				gMarquis->mTrust = 0.3f;
				gMarquis->mHostility = 0.3f;
			}
		}
	}
	//If the previous quest is done, check for the third quest
	else if (g_RDF_marquisToldAboutWoman == false)
	{
		//Check quest completion parameters if current NPC is The Marquis
		if (NPC->getName() == STRING_MARQUIS_NAME)
		{
			//Check if quest is completed, set parameters for completed quest
			if (NPC->mHostility >= 0.69)
			{
				LOG_INFO("<<QUEST>> Third quest has been completed.");

				//Set quest state to finished
				g_RDF_marquisToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_4_USER_PROMPT;

				//Set required RDF strings
				gFather->mRDFDynamicContext = RDF_FATHER_TOLD_ABOUT_WOMAN_FALSE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_WOMAN_TRUE;

				//RoyalGuard: Castle -> Overworld
				gRoyalGuard->mRDFDynamicContext = RDF_ROYALGUARD_GUARDING_HOUSE;

				//Set required emotion attributes
				gFather->mHappiness = 0.5f;
				gFather->mTrust = 0.4f;
				gFather->mHostility = 0.3f;
			}
		}
	}
	//If the previous quest is done, check for the fourth quest
	else if (g_RDF_fatherToldAboutWoman == false)
	{
		//Check quest completion parameters if current NPC is The Father
		if (NPC->getName() == STRING_FATHER_NAME)
		{
			//Check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.69) //not always registering at 0.7
			{
				LOG_INFO("<<QUEST>> Fourth quest has been completed.");

				//Set quest state to finished
				g_RDF_fatherToldAboutWoman = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_5_USER_PROMPT;

				//Set required RDF strings
				gFather->mRDFDynamicContext = RDF_FATHER_TOLD_ABOUT_WOMAN_TRUE;
				gRoyalGuard->mRDFDynamicContext = RDF_ROYALGUARD_CONVINCED_FALSE;
				//Ready to be convinced

				//Set required emotion attributes
				gRoyalGuard->mHappiness = 0.4f;
				gRoyalGuard->mTrust = 0.2f;
				gRoyalGuard->mHostility = 0.4f;
			}
		}
	}
	//If the previous quest is done, check for the fifth quest
	else if (g_RDF_isGuardConvinced == false) 
	{
		//Check quest completion parameters if current NPC is The Royal Guard
		if (NPC->getName() == STRING_ROYALGUARD_NAME)
		{
			//Check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.59)
			{
				LOG_INFO("<<QUEST>> Fifth quest has been completed.");

				//Set quest state to finished
				g_RDF_isGuardConvinced = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_6_USER_PROMPT;

				//Set required RDF strings
				gRoyalGuard->mRDFDynamicContext = RDF_ROYALGUARD_CONVINCED_TRUE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_INNKEEPER_FALSE;

				//Set required emotion attributes
				gMarquis->mHappiness = 0.1f;
				gMarquis->mTrust = 0.3f;
				gMarquis->mHostility = 0.7f;

				//Unlock the house
				if (Overworld::get()->mHouseWallInserted)
				{
					Overworld::get()->mWalls.pop_back();
					Overworld::get()->mHouseWallInserted = false;

					LOG_INFO("<wall> Unlocked Marquis house (UpdateBools)");
				}
			}
		}
	}
	//If the previous quest is done, check for the sixth quest
	else if (g_RDF_marquisToldAboutInnkeeper == false)
	{
		//Check quest completion parameters if current NPC is The Marquis
		if (NPC->getName() == STRING_MARQUIS_NAME)
		{
			//Check if quest is completed, set parameters for completed quest
			if (NPC->mHostility <= 0.41 && NPC->mTrust >= 0.49)
			{
				LOG_INFO("<<QUEST>> Sixth quest has been completed.");

				//Set quest state to finished
				g_RDF_marquisToldAboutInnkeeper = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_7_USER_PROMPT;

				//Set required RDF strings
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_REBELS_FALSE;
				gMarquis->mRDFDynamicContext = RDF_MARQUIS_TOLD_ABOUT_INNKEEPER_TRUE;

				//Set required emotion attributes
				gInnkeeper->mHappiness = 0.4f;
				gInnkeeper->mTrust = 0.2f;
				gInnkeeper->mHostility = 0.4f;
			}
		}
	}
	//If the previous quest is done, check for the seventh quest
	else if (g_RDF_innkeeperToldAboutRebels == false)
	{
		//Check quest completion parameters if current NPC is The Innkeeper
		if (NPC->getName() == STRING_INNKEEPER_NAME)
		{
			//Check if quest is completed, set parameters for completed quest
			if (NPC->mTrust >= 0.79 && NPC->mHostility <= 0.31)
			{
				LOG_INFO("<<QUEST>> Seventh quest has been completed.");

				//Set quest state to finished
				g_RDF_innkeeperToldAboutRebels = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_7_ADDITIONAL_USER_PROMPT;

				//Set required RDF strings
				gInnkeeper->mRDFDynamicContext = RDF_INNKEEPER_TOLD_ABOUT_REBELS_TRUE;
				gKing->mRDFDynamicContext = RDF_JOIN_THE_REBELLION_SIDE_WITH_KING_FALSE;
			}
		}
	}
	//If the previous quest is done and the player has left The Inn
	else if (g_additional_playerLeftInnAfterQuestSeven == true) 
	{
		//Check if the player is talking to The King
		if (NPC->getName() == STRING_KING_NAME) 
		{
			if (NPC->mHostility >= 0.69)
			{
				LOG_INFO("<<QUEST>> Eigth quest has been completed. Sided with the king.");

				//Set quest state to finished
				g_final_playerSidedWithKing = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_FINAL_KING_SIDE;

				//Set required RDF strings
				gKing->mRDFDynamicContext = RDF_JOIN_THE_REBELLION_SIDE_WITH_KING_TRUE;
			}
		}
		//Check if the player is talking to The Innkeeper
		else if (NPC->getName() == STRING_INNKEEPER_NAME)
		{
			if (NPC->mTrust >= 0.89)
			{
				LOG_INFO("<<QUEST>> Eigth quest has been completed. Sided with the innkeeper.");

				//Set quest state to finished
				g_final_playerSidedWithInnkeeper = true;

				//Set user quest prompt to the next quest
				gCurrentQuestPrompt = &STRING_QUEST_FINAL_REBEL_SIDE;

				//Set required RDF strings
				gInnkeeper->mRDFDynamicContext = RDF_JOIN_THE_REBELLION_SIDE_WITH_INNKEEPER_TRUE;
			}
		}
	}
}