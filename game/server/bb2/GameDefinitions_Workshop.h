//=========       Copyright � Reperio Studios 2016 @ Bernt Andreas Eide!       ============//
//
// Purpose: Handle server-side workshop downloading/updating, specifically for gameservers using SteamCMD.
//
//========================================================================================//

#ifndef GAME_DEFINITIONS_WORKSHOP_H
#define GAME_DEFINITIONS_WORKSHOP_H

#ifdef _WIN32
#pragma once
#endif

#include <steam/steam_api.h>
#include <steam/steam_gameserver.h>

class CGameDefinitionsWorkshop;
class CGameDefinitionsWorkshop
{
public:
	CGameDefinitionsWorkshop();
	~CGameDefinitionsWorkshop();

	void Initialize();
	void DownloadThink();
	void UpdateDownloadedItems();

	void AddItemToList(PublishedFileId_t itemID);
	void RemoveItemFromList(PublishedFileId_t itemID);
	int GetItemInList(PublishedFileId_t itemID);
	int GetLastItemInList(void) { return (m_pWorkshopItemUpdateList.Count() - 1); }

	const char *GetWorkshopDir() { return pszWorkshopDir; }

protected:
	STEAM_GAMESERVER_CALLBACK(CGameDefinitionsWorkshop, DownloadedItem, DownloadItemResult_t, m_CallbackItemDownloaded);

private:
	char pszWorkshopDir[2048];
	float m_flDownloadInfoDelay;
	CUtlVector<PublishedFileId_t> m_pWorkshopItemUpdateList;
};

#endif // GAME_DEFINITIONS_WORKSHOP_H