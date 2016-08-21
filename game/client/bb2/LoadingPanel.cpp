//=========       Copyright � Reperio Studios 2015 @ Bernt Andreas Eide!       ============//
//
// Purpose: Custom Loading Dialog - Reports Loading Progress and the Loading String as well!
// Notice: We also deal with some client initialization on loading end.
//
//========================================================================================//

#include "cbase.h"
#include "LoadingPanel.h"
#include "vgui_controls/Frame.h"
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui/IInput.h>
#include <vgui/ILocalize.h>
#include "interface.h"
#include "hl2mp_gamerules.h"
#include "filesystem.h"
#include "vgui_controls/Button.h"
#include "vgui_controls/ImagePanel.h"
#include <vgui/ISystem.h>
#include "GameBase_Client.h"
#include "GameBase_Shared.h"

using namespace vgui;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLoadingPanel::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	m_pTextLoadingTip->SetFgColor(pScheme->GetColor("LoadingTipTextColor", Color(255, 255, 255, 255)));
	m_pTextLoadingTip->SetFont(pScheme->GetFont("LoadingTip", false));

	m_pTextProgress->SetFgColor(pScheme->GetColor("LoadingProgressTextColor", Color(255, 255, 255, 255)));
	m_pTextProgress->SetFont(pScheme->GetFont("BB2_PANEL", true));

	m_pTipBackground->SetBorder(NULL);
	m_pTipBackground->SetFgColor(pScheme->GetColor("LoadingTipBackgroundFg", Color(0, 0, 0, 255)));
	m_pTipBackground->SetBgColor(pScheme->GetColor("LoadingTipBackgroundBg", Color(0, 0, 0, 255)));

	m_pTextMapDetail[0]->SetFgColor(Color(255, 255, 255, 255));
	m_pTextMapDetail[1]->SetFgColor(Color(255, 255, 255, 255));
	m_pTextMapDetail[2]->SetFgColor(colMapString);
	m_pTextMapDetail[3]->SetFgColor(colStatsStatus);

	m_pTextMapDetail[0]->SetFont(pScheme->GetFont("LoadingFontHuge"));
	m_pTextMapDetail[1]->SetFont(pScheme->GetFont("LoadingFontBig"));
	m_pTextMapDetail[2]->SetFont(pScheme->GetFont("LoadingFontSmall"));
	m_pTextMapDetail[3]->SetFont(pScheme->GetFont("LoadingFontSmall"));
}

// The panel background image should be square, not rounded.
void CLoadingPanel::PaintBackground()
{
	SetBgColor(Color(0, 0, 0, 0));
	SetPaintBackgroundType(0);
	BaseClass::PaintBackground();
}

void CLoadingPanel::PerformLayout()
{
	SetupLayout();
	BaseClass::PerformLayout();
	InvalidateLayout(false, true);
}

void CLoadingPanel::OnScreenSizeChanged(int iOldWide, int iOldTall)
{
	BaseClass::OnScreenSizeChanged(iOldWide, iOldTall);
	PerformLayout();
}

void CLoadingPanel::SetupLayout(void)
{
	SetPos(0, 0);
	SetSize(ScreenWidth(), ScreenHeight());

	for (int i = 0; i < _ARRAYSIZE(m_pTextMapDetail); i++)
	{
		m_pTextMapDetail[i]->SetEnabled(!m_bDisconnected);
		m_pTextMapDetail[i]->SetVisible(!m_bDisconnected);
	}

	m_pImgTipIcon->SetEnabled(!m_bDisconnected);
	m_pImgTipIcon->SetVisible(!m_bDisconnected);

	m_pTextLoadingTip->SetEnabled(!m_bDisconnected);
	m_pTextLoadingTip->SetVisible(!m_bDisconnected);

	m_pProgressBar->SetEnabled(!m_bDisconnected);
	m_pProgressBar->SetVisible(!m_bDisconnected);

	float flScale;

	flScale = (float)((float)ScreenWidth() * 0.1);
	m_pProgressBar->SetSize((ScreenWidth() - flScale), scheme()->GetProportionalScaledValue(16));
	m_pTextProgress->SetSize((ScreenWidth() - flScale), scheme()->GetProportionalScaledValue(16));
	m_pTextProgress->SetContentAlignment(Label::a_center);

	flScale = (float)((float)ScreenHeight() * 0.1);
	m_pTipBackground->SetSize(ScreenWidth(), flScale);

	m_pImgTipIcon->SetSize((flScale - scheme()->GetProportionalScaledValue(6)), (flScale - scheme()->GetProportionalScaledValue(6)));
	m_pImgLoadingBackground->SetSize(ScreenWidth(), (ScreenHeight() - flScale));
	m_pTextLoadingTip->SetSize(ScreenWidth(), flScale);
	m_pImgLoadingBackground->SetPos(0, 0);
	m_pTextLoadingTip->SetPos(vgui::scheme()->GetProportionalScaledValue(16) + flScale, (ScreenHeight() - flScale));
	m_pTipBackground->SetPos(0, (ScreenHeight() - flScale));
	m_pImgTipIcon->SetPos(vgui::scheme()->GetProportionalScaledValue(15), (ScreenHeight() - (flScale - scheme()->GetProportionalScaledValue(3))));

	float flX = (float)((float)ScreenWidth() * 0.05);
	m_pProgressBar->SetPos(flX, (ScreenHeight() - flScale - scheme()->GetProportionalScaledValue(16)));
	m_pTextProgress->SetPos(flX, (ScreenHeight() - flScale - scheme()->GetProportionalScaledValue(34)));

	// Map Data
	m_pTextMapDetail[0]->SetPos(scheme()->GetProportionalScaledValue(5), scheme()->GetProportionalScaledValue(5));
	m_pTextMapDetail[0]->SetSize((ScreenWidth() / 2), scheme()->GetProportionalScaledValue(30));

	m_pTextMapDetail[1]->SetPos((ScreenWidth() - scheme()->GetProportionalScaledValue(105)), scheme()->GetProportionalScaledValue(5));
	m_pTextMapDetail[1]->SetSize(scheme()->GetProportionalScaledValue(100), scheme()->GetProportionalScaledValue(18));

	m_pTextMapDetail[2]->SetPos((ScreenWidth() - scheme()->GetProportionalScaledValue(68)), scheme()->GetProportionalScaledValue(18));
	m_pTextMapDetail[2]->SetSize(scheme()->GetProportionalScaledValue(60), scheme()->GetProportionalScaledValue(18));

	m_pTextMapDetail[3]->SetPos((ScreenWidth() - scheme()->GetProportionalScaledValue(68)), scheme()->GetProportionalScaledValue(30));
	m_pTextMapDetail[3]->SetSize(scheme()->GetProportionalScaledValue(60), scheme()->GetProportionalScaledValue(18));

	m_pMapRating->SetSize(scheme()->GetProportionalScaledValue(32), scheme()->GetProportionalScaledValue(8));
	m_pMapRating->SetPos((ScreenWidth() - scheme()->GetProportionalScaledValue(44)), scheme()->GetProportionalScaledValue(33));

	// IF the server kicked, banned or shut down or timed out we will notice that, fixup the GUI:
	if (m_bDisconnected)
	{
		int x, y, w, h;
		m_pTipBackground->GetBounds(x, y, w, h);

		m_pTextProgress->SetPos(x, y);
		m_pTextProgress->SetSize(w, h);

		m_pMapRating->SetVisible(false);
	}
}

void CLoadingPanel::OnThink()
{
	SetupLayout();
	BaseClass::OnThink();
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CLoadingPanel::CLoadingPanel(vgui::VPANEL parent) : BaseClass(NULL, "LoadingPanel")
{
	SetParent(parent);
	SetTitleBarVisible(false);
	SetMinimizeButtonVisible(false);
	SetMaximizeButtonVisible(false);
	SetCloseButtonVisible(false);
	SetSizeable(false);
	SetMoveable(false);
	SetProportional(true);
	SetVisible(true);
	SetKeyBoardInputEnabled(false);
	SetMouseInputEnabled(false);
	SetScheme("ClientScheme");

	SetZPos(100);

	vgui::ivgui()->AddTickSignal(GetVPanel(), 1);

	InvalidateLayout();

	// Initialize images
	m_pImgLoadingBackground = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "LoadingImage"));
	m_pImgLoadingBackground->SetImage("mainmenu/backgroundart");
	m_pImgLoadingBackground->SetShouldScaleImage(true);
	m_pImgLoadingBackground->SetZPos(110);

	// Loading Tips
	m_pTextLoadingTip = vgui::SETUP_PANEL(new vgui::Label(this, "TextTip", ""));
	m_pTextLoadingTip->SetZPos(130);
	m_pTextLoadingTip->SetText("");

	m_pTextProgress = vgui::SETUP_PANEL(new vgui::Label(this, "TextProgress", ""));
	m_pTextProgress->SetZPos(150);
	m_pTextProgress->SetText("");

	m_pImgTipIcon = vgui::SETUP_PANEL(new vgui::ImagePanel(this, "TipIco"));
	m_pImgTipIcon->SetImage("loading/icons/tip");
	m_pImgTipIcon->SetShouldScaleImage(true);
	m_pImgTipIcon->SetZPos(120);

	m_pTipBackground = vgui::SETUP_PANEL(new vgui::Divider(this, "TipBG"));
	m_pTipBackground->SetZPos(120);
	m_pTipBackground->SetBorder(NULL);

	m_pProgressBar = vgui::SETUP_PANEL(new ImageProgressBar(this, "ProgressBar", "vgui/loading/progress_bar", "vgui/loading/progress_bg"));
	m_pProgressBar->SetProgressDirection(ProgressBar::PROGRESS_EAST);
	m_pProgressBar->SetZPos(135);

	// Map Data Details
	for (int i = 0; i < _ARRAYSIZE(m_pTextMapDetail); i++)
	{
		m_pTextMapDetail[i] = vgui::SETUP_PANEL(new vgui::Label(this, "TextMapDetail", ""));
		m_pTextMapDetail[i]->SetZPos(130);
		m_pTextMapDetail[i]->SetText("");
	}

	colMapString = Color(0, 0, 0, 0);
	colStatsStatus = Color(0, 0, 0, 0);

	m_pMapRating = vgui::SETUP_PANEL(new ImageProgressBar(this, "MapRating", "vgui/loading/rating_full", "vgui/loading/rating_empty"));
	m_pMapRating->SetProgressDirection(ProgressBar::PROGRESS_EAST);
	m_pMapRating->SetZPos(135);

	PerformLayout();
}

//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CLoadingPanel::~CLoadingPanel()
{
}

const char *CLoadingPanel::GetLoadingTip(const char *text)
{
	char pszString[256];
	Q_strncpy(pszString, text, 256);
	char *p = pszString;

	int iSize = strlen(pszString);
	bool bHasFunction = false;
	for (int i = 0; i < iSize; i++)
	{
		if (pszString[i] == '[')
		{
			bHasFunction = true;
			break;
		}
	}

	if (bHasFunction)
		p = ReplaceBracketsWithInfo(p);

	return p;
}

char *CLoadingPanel::ReplaceBracketsWithInfo(char *text)
{
	char *p = text;
	char pszBeginning[128], pszEnding[128], pszMiddle[128];
	int length = Q_strlen(text);

	int iOccuranceStart = -1;
	int iOccuranceEnd = -1;

	for (int i = 0; i < length; i++)
	{
		if (iOccuranceStart == -1)
		{
			if (p[i] == '[')
			{
				iOccuranceStart = i;
			}
		}
		else if (iOccuranceEnd == -1)
		{
			if (p[i] == ']')
			{
				iOccuranceEnd = i;
			}
		}
	}

	Q_strncpy(pszBeginning, p, length);
	pszBeginning[iOccuranceStart - 1] = 0;

	Q_strncpy(pszEnding, &p[iOccuranceEnd + 2], (length - iOccuranceEnd - 1));
	Q_strncpy(pszMiddle, &p[iOccuranceStart + 1], (iOccuranceEnd - iOccuranceStart));

	char pszFunc[32];
	const char *keybind = engine->Key_LookupBinding(pszMiddle);
	if (keybind == NULL)
		keybind = "UNBOUND";
	Q_strncpy(pszFunc, keybind, 32);
	Q_strupr(pszFunc);

	Q_snprintf(p, 256, "%s %s %s", pszBeginning, pszFunc, pszEnding);

	length = Q_strlen(p);
	bool bRetry = false;
	for (int i = 0; i < length; i++)
	{
		if (p[i] == '[')
		{
			bRetry = true;
			break;
		}
	}

	if (bRetry)
		return (p = ReplaceBracketsWithInfo(p));

	return p;
}

// Reads the Tips.txt file and picks a random string for display.
void CLoadingPanel::SetRandomLoadingTip(void)
{
	m_pTextLoadingTip->SetText(""); // Clear string...

	KeyValues *kvLoadingTips = new KeyValues("LoadingTipData");
	if (kvLoadingTips->LoadFromFile(filesystem, "data/settings/Tips.txt", "MOD"))
	{
		int iAmountTips = 0;
		for (KeyValues *sub = kvLoadingTips->GetFirstSubKey(); sub; sub = sub->GetNextKey())
			iAmountTips++;

		int iRandomTip = random->RandomInt(1, iAmountTips);
		int iCurrTip = 1;
		for (KeyValues *sub = kvLoadingTips->GetFirstSubKey(); sub; sub = sub->GetNextKey())
		{
			if (iCurrTip == iRandomTip)
			{
				char pchLoadingTip[256];
				g_pVGuiLocalize->ConvertUnicodeToANSI(g_pVGuiLocalize->Find(sub->GetString()), pchLoadingTip, 256);

				m_pTextLoadingTip->SetText(GetLoadingTip(pchLoadingTip));
				m_pImgTipIcon->SetImage(sub->GetName());
				break;
			}

			iCurrTip++;
		}
	}
	else
		m_pTextLoadingTip->SetText("");

	kvLoadingTips->deleteThis();
}

void CLoadingPanel::OnTick()
{
	BaseClass::OnTick();

	FMODManager()->FadeThink();

	if (!IsVisible())
	{
		GameBaseClient->OnUpdate();

		if (m_bCanUpdateImage)
		{
			m_pTextProgress->SetText("");
			m_pProgressBar->SetProgress(0.0f);
			m_pImgLoadingBackground->SetImage("mainmenu/backgroundart"); // Reset loading img to default.
			GameBaseClient->ResetMapConVar();

			// IF our class exist and we actually loaded a while ago we're in-game if not we canceled the load...
			C_BasePlayer *pPLR = C_BasePlayer::GetLocalPlayer();
			if (!pPLR)
				GameBaseClient->RunCommand(COMMAND_DISCONNECT);
			else
			{
				GameBaseClient->RunClientEffect(PLAYER_EFFECT_ENTERED_GAME, 1);
			}
		}

		m_bCanUpdateImage = false;
		m_bDisconnected = false;
		m_bLoadedMapData = false;
	}
	else
	{
		SetLoadingAttributes();
		Activate();
		RequestFocus();
		MoveToFront();

		if (!m_bCanUpdateImage)
		{
			m_bCanUpdateImage = true;

			int iMapIndex = -1;
			if (GameBaseShared()->GetSharedMapData())
				iMapIndex = GameBaseShared()->GetSharedMapData()->GetMapIndex(GameBaseClient->GetLoadingImage());

			for (int i = 0; i < _ARRAYSIZE(m_pTextMapDetail); i++)
				m_pTextMapDetail[i]->SetText("");

			m_pTextMapDetail[3]->SetVisible(false);
			m_pMapRating->SetVisible(false);
			colMapString = Color(0, 255, 0, 255);
			colStatsStatus = Color(0, 0, 0, 0);

			vgui::VPANEL panel = GetVParent();
			if (panel)
			{
				int NbChilds = vgui::ipanel()->GetChildCount(panel);

				for (int i = 0; i < NbChilds; ++i)
				{
					VPANEL gameUIPanel = vgui::ipanel()->GetChild(panel, i);
					Panel *basePanel = vgui::ipanel()->GetPanel(gameUIPanel, "GameUI");
					if (basePanel)
					{
						// We load a different scheme file for the actual engine loading dialog so we don't screw anything up, the scheme has all colors set to BLANK so the dialog will be invisible.
						if (!strcmp(basePanel->GetName(), "LoadingDialog"))
						{
							basePanel->SetScheme("LoadingScheme");
							basePanel->InvalidateLayout(false, true);
						}
					}
				}
			}

			int iRandomImages = -1;
			char pszLoadingImage[80];

			FileFindHandle_t findHandle;
			const char *pFilename = filesystem->FindFirstEx("materials/vgui/loading/*.vmt", "MOD", &findHandle);
			while (pFilename)
			{
				if (pFilename && (strlen(pFilename) > 4) && Q_stristr(pFilename, GameBaseClient->GetLoadingImage()))
					iRandomImages++;

				pFilename = filesystem->FindNext(findHandle);
			}
			filesystem->FindClose(findHandle);

			Q_snprintf(pszLoadingImage, 80, "materials/vgui/loading/%s_0.vmt", GameBaseClient->GetLoadingImage());
			if ((iRandomImages != -1) && filesystem->FileExists(pszLoadingImage))
			{
				int iRandomImage = random->RandomInt(0, iRandomImages);
				Q_snprintf(pszLoadingImage, 80, "materials/vgui/loading/%s_%i.vmt", GameBaseClient->GetLoadingImage(), iRandomImage);
				while (!filesystem->FileExists(pszLoadingImage))
				{
					iRandomImage = random->RandomInt(0, iRandomImages);
					Q_snprintf(pszLoadingImage, 80, "materials/vgui/loading/%s_%i.vmt", GameBaseClient->GetLoadingImage(), iRandomImage);
				}

				m_pImgLoadingBackground->SetImage(VarArgs("loading/%s_%i", GameBaseClient->GetLoadingImage(), iRandomImage));
			}

			SetRandomLoadingTip();

			// Load the map details!
			if (iMapIndex != -1)
			{
				gameMapItem_t mapItem = GameBaseShared()->GetSharedMapData()->pszGameMaps[iMapIndex];
				if (!mapItem.bExclude)
				{
					int gamemodeForMap = GetGamemodeForMap(mapItem.pszMapName);
					m_bLoadedMapData = ((gamemodeForMap == MODE_ARENA) || (gamemodeForMap == MODE_OBJECTIVE));
					m_pTextMapDetail[3]->SetVisible(m_bLoadedMapData);

					m_pTextMapDetail[0]->SetText(mapItem.pszMapTitle);
					m_pTextMapDetail[1]->SetText(GetGamemodeNameForPrefix(mapItem.pszMapName));
					m_pTextMapDetail[1]->SetContentAlignment(Label::Alignment::a_east);
					m_pTextMapDetail[2]->SetContentAlignment(Label::Alignment::a_east);
					m_pTextMapDetail[3]->SetContentAlignment(Label::Alignment::a_east);

					colMapString = Color(0, 255, 0, 255);
					if (mapItem.iMapVerification == MAP_VERIFIED_WHITELISTED)
					{
						m_pMapRating->SetVisible(true);
						m_pMapRating->SetProgress(mapItem.flScore);
						m_pTextMapDetail[2]->SetText("#LoadingUI_CustomMap");
					}
					else if (mapItem.iMapVerification == MAP_VERIFIED_UNKNOWN)
					{
						colMapString = Color(255, 0, 0, 255);
						m_pTextMapDetail[2]->SetText("#LoadingUI_CustomMap");
					}
					else
						m_pTextMapDetail[2]->SetText("#LoadingUI_OfficialMap");
				}
			}
		}
	}
}

// Get Loading Progress from the engine and the loading string:
void CLoadingPanel::SetLoadingAttributes(void)
{
	vgui::VPANEL panel = GetVParent();
	if (panel)
	{
		int NbChilds = vgui::ipanel()->GetChildCount(panel);

		for (int i = 0; i < NbChilds; ++i)
		{
			VPANEL gameUIPanel = vgui::ipanel()->GetChild(panel, i);
			int newChilds = vgui::ipanel()->GetChildCount(gameUIPanel);
			for (int z = 0; z < newChilds; ++z)
			{
				VPANEL prPan = vgui::ipanel()->GetChild(gameUIPanel, z);
				Panel *myPanel = vgui::ipanel()->GetPanel(prPan, "GameUI");
				if (myPanel)
				{
					// Get Progress Value:
					if (!strcmp(myPanel->GetName(), "Progress"))
					{
						ProgressBar *pBar = dynamic_cast<ProgressBar*> (myPanel);
						if (pBar)
							m_pProgressBar->SetProgress(pBar->GetProgress());
					}
					// Get Progress String:
					else if (!strcmp(myPanel->GetName(), "InfoLabel"))
					{
						Label *pLabel = dynamic_cast<Label*>(myPanel);
						if (pLabel)
						{
							char szString[256];
							pLabel->GetText(szString, 256);
							m_pTextProgress->SetText(szString);
							m_bDisconnected = (!engine->IsDrawingLoadingImage() && IsVisible());
						}
					}
				}
			}
		}
	}

	if (!m_bDisconnected && m_bLoadedMapData && engine->IsConnected() && HL2MPRules())
	{
		colStatsStatus = Color(255, 20, 20, 255);
		switch (bb2_profile_system_status.GetInt())
		{

		case 1: // Global
		{
			m_pTextMapDetail[3]->SetText("#LoadingUI_GlobalStats");
			colStatsStatus = Color(20, 255, 20, 255);
			break;
		}

		case 2: // Local
		{
			m_pTextMapDetail[3]->SetText("#LoadingUI_LocalStats");
			colStatsStatus = Color(20, 255, 20, 255);
			break;
		}

		default:
		{
			m_pTextMapDetail[3]->SetText("#LoadingUI_NoStats");
			break;
		}

		}
	}
}