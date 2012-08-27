//====== Copyright � 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "stdafx.h"
#include <afxadv.h>
#include <oaidl.h>
#include <afxpriv.h>
#include "Worldcraft.h"
#include "Box3D.h"				// For units
#include "FaceEditSheet.h"
#include "MainFrm.h"
#include "MessageWnd.h"
#include "ControlBarIDs.h"
#include "CustomMessages.h"
#include "DynamicDialogWnd.h"
#include "GlobalFunctions.h"
#include "HelpID.h"
#include "Prefabs.h"
#include "PrefabsDlg.h"
#include "MapDoc.h"
#include "GameData.h"
#include "StatusBarIDs.h"
#include "Splash.h"
#include "StartupDlg.h"
#include "Options.h"
#include "OptionProperties.h"
#include "ObjectProperties.h"
#include "OP_Groups.h"
#include "MapView2D.h"
#include "MapView3D.h"
#include "ChildFrm.h"
#include "NewDocType.h"
#include "SearchReplaceDlg.h"
#include "TextureBrowser.h"
#include "TextureSystem.h"
#include "ToolManager.h"
#include "Material.h"
#include "materialsystem/IMaterialSystem.h"
#include "materialsystem/MaterialSystem_Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTIES, OnUpdateEditFunction)
	ON_COMMAND(ID_VIEW_MESSAGES, OnViewMessages)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MESSAGES, OnUpdateViewMessages)
	ON_WM_ACTIVATEAPP()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_TOOLS_PREFABFACTORY, OnToolsPrefabfactory)
	ON_COMMAND(ID_HELP_FINDERWC, OnHelpFinder)
	ON_COMMAND_EX(ID_HELP_EDITINGSITE, OnHelpOpenURL)
	ON_COMMAND_EX(ID_HELP_HANDY_VANDALS, OnHelpOpenURL)
	ON_COMMAND_EX(ID_HELP_EDITING_FORUM, OnHelpOpenURL)
	ON_COMMAND_EX(ID_HELP_TFC_FORUM, OnHelpOpenURL)
	ON_COMMAND_EX(ID_HELP_OPFOR_FORUM, OnHelpOpenURL)
	ON_COMMAND_EX(ID_HELP_WORLDCRAFT_FORUM, OnHelpOpenURL)
	ON_COMMAND_EX(ID_HELP_WORLDCRAFT_SUPPORT_MAIL, OnHelpOpenURL)
	ON_COMMAND(ID_EDIT_UNDOREDOACTIVE, OnEditUndoredoactive)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDOREDOACTIVE, OnUpdateEditUndoredoactive)
	ON_COMMAND_EX(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_SAVEWINDOWSTATE, OnSavewindowstate)
	ON_COMMAND(ID_LOADWINDOWSTATE, OnLoadwindowstate)
	ON_COMMAND_EX(ID_MAP_UNITS_NONE, OnUnits)
	ON_UPDATE_COMMAND_UI(ID_MAP_UNITS_NONE, OnUpdateUnits)
	ON_COMMAND_EX(ID_MAP_UNITS_INCHES, OnUnits)
	ON_UPDATE_COMMAND_UI(ID_MAP_UNITS_INCHES, OnUpdateUnits)
	ON_COMMAND_EX(ID_MAP_UNITS_FEET_INCHES, OnUnits)
	ON_UPDATE_COMMAND_UI(ID_MAP_UNITS_FEET_INCHES, OnUpdateUnits)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OPAQUE_MATERIALS, OnUpdateOpaqueMaterials)
	ON_UPDATE_COMMAND_UI(ID_VIEW_2DXZ, OnUpdateView2d)
	ON_UPDATE_COMMAND_UI(ID_VIEW_2DYZ, OnUpdateView2d) 
	ON_UPDATE_COMMAND_UI(ID_VIEW_2DXY, OnUpdateView2d)
	ON_UPDATE_COMMAND_UI(ID_VIEW_3DWIREFRAME, OnUpdateView3d)
	ON_UPDATE_COMMAND_UI(ID_VIEW_3DPOLYGON, OnUpdateView3d)
	ON_UPDATE_COMMAND_UI(ID_VIEW_3DTEXTURED, OnUpdateView3d)
	ON_COMMAND(ID_VIEW_OPAQUE_MATERIALS, OnOpaqueMaterials)
	ON_COMMAND_EX(ID_VIEW3D_BRIGHTER, OnView3dChangeBrightness)
	ON_COMMAND_EX(ID_VIEW3D_DARKER, OnView3dChangeBrightness)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECTBAR, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_OBJECTBAR, CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTION_MODE_BAR, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_SELECTION_MODE_BAR, CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILTERCONTROL, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_FILTERCONTROL, CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAPVIEWBAR, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_MAPVIEWBAR, CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAPTOOLSBAR, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_MAPTOOLSBAR, CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TEXTUREBAR, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TEXTUREBAR, CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANIMATIONBAR, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_ANIMATIONBAR, CFrameWnd::OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAPOPSBAR, CFrameWnd::OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_MAPOPSBAR, CFrameWnd::OnBarCheck)
	ON_COMMAND_EX(ID_TOOLS_POINTER, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_POINTER, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_CAMERA, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CAMERA, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_MAGNIFY, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MAGNIFY, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_BLOCK, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_BLOCK, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_ENTITY, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ENTITY, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_APPLYDECALS, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_APPLYDECALS, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_MORPH, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_MORPH, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_CLIPPER, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CLIPPER, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_EDITCORDON, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_EDITCORDON, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_PATH, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_PATH, OnUpdateToolUI)
	ON_COMMAND_EX(ID_TOOLS_OVERLAY, OnChangeTool)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_OVERLAY, OnUpdateToolUI)
	ON_COMMAND_EX(ID_MODE_APPLICATOR, OnApplicator)
    ON_UPDATE_COMMAND_UI(ID_MODE_APPLICATOR, OnUpdateApplicatorUI)
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_SELECTION,
	ID_INDICATOR_COORDS,
	ID_INDICATOR_SIZE,
	ID_INDICATOR_GRIDZOOM,
	ID_INDICATOR_SNAP
};


const int NUMSTATUSPANES = 7;


const char * WINSTATETAG = "WCWINSTATE";
const int	 WINSTATEEND = -1;
const int	 WINSTATE2DVIEW = 0;
const int	 WINSTATE3DVIEW = 1;
const float	 fVersion = 0.1f;


struct
{
	int nIndex;
	UINT nID;
	UINT nStyle;
	int cxWidth;
} paneinfo[NUMSTATUSPANES] = 
{
	{ SBI_PROMPT,		ID_SEPARATOR,				SBPS_STRETCH | SBPS_NOBORDERS, 0 },
	{ SBI_SELECTION,	ID_INDICATOR_SELECTION,		SBPS_NORMAL, 200 },
	{ SBI_COORDS,		ID_INDICATOR_COORDS,		SBPS_NORMAL, 100 },
	{ SBI_SIZE,			ID_INDICATOR_SIZE,			SBPS_NORMAL, 140 },
	{ SBI_GRIDZOOM,		ID_INDICATOR_GRIDZOOM,		SBPS_NORMAL, 80 },
	{ SBI_SNAP,			ID_INDICATOR_SNAP,			SBPS_NORMAL, 135 },
	{ SBI_LIGHTPROGRESS,ID_INDICATOR_LIGHTPROGRESS,	SBPS_NORMAL, 50 }
};


static GameData gd;
static CMainFrame *pMainWnd;


//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------
CMainFrame::CMainFrame(void)
{
	pTextureBrowser = NULL;
	pMsgWnd = NULL;
	pObjectProperties = NULL;
	m_bUndoActive = TRUE;
	m_bShellSessionActive = false;
	m_pFaceEditSheet = NULL;
	m_pSearchReplaceDlg = NULL;
}


//-----------------------------------------------------------------------------
// Purpose: Destructor.
//-----------------------------------------------------------------------------
CMainFrame::~CMainFrame(void)
{
	delete pObjectProperties;
	delete pTextureBrowser;
	delete m_pFaceEditSheet;
	delete m_pSearchReplaceDlg;

	CPrefabLibrary::FreeAllLibraries();
}


//-----------------------------------------------------------------------------
// Purpose: Called through the shell to begin a session of editing the map
//			via the shell. The user interface is disabled to prevent a mismatched
//			versions between Worldcraft and the shell client.
//-----------------------------------------------------------------------------
void CMainFrame::BeginShellSession(void)
{
	m_bShellSessionActive = true;
}


//-----------------------------------------------------------------------------
// Purpose: Called through the shell to end a session of editing the map
//			via the engine. The user interface is enabled.
//-----------------------------------------------------------------------------
void CMainFrame::EndShellSession(void)
{
	m_bShellSessionActive = false;
}


//-----------------------------------------------------------------------------
// Purpose: If we get here there is no active 3D view. Uncheck the button.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateView3d(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
}


//-----------------------------------------------------------------------------
// Purpose: If we get here there is no active 3D view. Uncheck the button.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateOpaqueMaterials(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(MaterialSystemConfig().bNoTransparency);
}


void CMainFrame::OnOpaqueMaterials()
{
	MaterialSystemConfig().bNoTransparency = !MaterialSystemConfig().bNoTransparency;
	MaterialSystemInterface()->UpdateConfig( &MaterialSystemConfig(), false );
}


//-----------------------------------------------------------------------------
// Purpose: If we get here there is no active 2D view. Uncheck the button.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateView2d(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(FALSE);
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : lpCreateStruct - 
// Output : 
//-----------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	const DWORD dwDefStyles = WS_CHILD | WS_VISIBLE | CBRS_TOP;
	lpCreateStruct->lpszClass = "VALVEWORLDCRAFT";

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    if(!wndMDIClient.SubclassWindow(m_hWndMDIClient)) 
	{ 
		TRACE ("Failed to subclass MDI client window\n");
		return (-1);                                        
    }                                                       

	//
	// Map view toolbar.
	//
	if (!m_wndMapToolBar.Create(this, dwDefStyles, IDCB_MAPVIEWBAR) || !m_wndMapToolBar.LoadToolBar(IDR_MAPDOC_VALVE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndMapToolBar.ModifyStyle(0, TBSTYLE_FLAT); 

	//
	// Map editing toolbar.
	//
	m_wndMapEditToolBar.Create(this, dwDefStyles, IDCB_MAPTOOLSBAR);
	m_wndMapEditToolBar.ModifyStyle(0, TBSTYLE_FLAT); 
	m_wndMapEditToolBar.LoadToolBar(IDR_MAPEDITTOOLS_VALVE);
    m_bmMapEditTools256.LoadBitmap(IDB_MAPEDITTOOLS_256);
    m_wndMapEditToolBar.SetBitmap((HBITMAP)m_bmMapEditTools256);

	//
	// Map operations toolbar.
	//
	if (!m_wndMapOps.Create(this, dwDefStyles, IDCB_MAPOPERATIONS) || !m_wndMapOps.LoadToolBar(IDR_MAPOPERATIONS_VALVE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndMapOps.ModifyStyle(0, TBSTYLE_FLAT); 

	//
	// Status bar.
	//
	if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(NULL, NUMSTATUSPANES))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	for(int i = 0; i < NUMSTATUSPANES; i++)
	{
		m_wndStatusBar.SetPaneInfo(paneinfo[i].nIndex, paneinfo[i].nID,	paneinfo[i].nStyle, paneinfo[i].cxWidth);
	}

	EnableDocking(CBRS_ALIGN_ANY);

	m_wndMapToolBar.SetBarStyle(m_wndMapToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndMapEditToolBar.SetBarStyle(m_wndMapEditToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndMapOps.SetBarStyle(m_wndMapOps.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_wndMapToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMapEditToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMapOps.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMapEditToolBar, AFX_IDW_DOCKBAR_LEFT);

	// top bars
	DockControlBar(&m_wndMapToolBar, AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf(&m_wndMapOps, &m_wndMapToolBar);

	// rightside control bars
	m_ObjectBar.Create(this);
	m_ObjectBar.SetBarStyle(m_ObjectBar.GetBarStyle() | 
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	m_ObjectBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBar(&m_ObjectBar, AFX_IDW_DOCKBAR_RIGHT);

	m_FilterControl.Create(this);
	m_FilterControl.SetBarStyle(m_FilterControl.GetBarStyle() | 
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	m_FilterControl.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarLeftOf(&m_FilterControl, &m_ObjectBar);

	m_TextureBar.Create(this);
	m_TextureBar.SetBarStyle(m_TextureBar.GetBarStyle() | 
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	m_TextureBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarLeftOf(&m_TextureBar, &m_FilterControl);

	m_pFaceEditSheet = new CFaceEditSheet( "Face Edit Sheet", this );
	m_pFaceEditSheet->Setup();
	m_pFaceEditSheet->Create( this );
	m_pFaceEditSheet->SetVisibility( false );

	//
	// Create the animation dialog bar.
	//
	#ifndef SDK_BUILD
	m_AnimationDlg.Create(this);
	m_AnimationDlg.SetBarStyle(m_TextureBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	m_AnimationDlg.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarLeftOf(&m_AnimationDlg, &m_TextureBar);
	#endif // SDK_BUILD

	//
	// Create the selection mode dialog bar.
	//
	m_SelectModeDlg.Create(this);
	m_SelectModeDlg.SetBarStyle(m_TextureBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);
	m_SelectModeDlg.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	DockControlBarLeftOf(&m_SelectModeDlg, &m_AnimationDlg);
	
	//
	// Create object properties sheet - not visible yet.
	//
	pObjectProperties = new CObjectProperties;
	pObjectProperties->SetupPages();
	pObjectProperties->Create(this, WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME);

	pMainWnd = this;

	//
	// Create the smoothing group visualization dialog.
	//
	m_SmoothingGroupDlg.Create( IDD_SMOOTHING_GROUP_VISUAL, this );

	//
	// Create message window.
	//
	CRect clientrect;
	wndMDIClient.GetClientRect(clientrect);
	pMsgWnd = (CMessageWnd*) RUNTIME_CLASS(CMessageWnd)->CreateObject();

	pMsgWnd->Create(NULL, "Messages", WS_OVERLAPPEDWINDOW | WS_CHILD,
		CRect(0, clientrect.Height() - 90, clientrect.Width(), 
		clientrect.Height()), this);

	CPrefabLibrary::LoadAllLibraries();

	g_pToolManager->SetTool(TOOL_POINTER);
	
	pTextureBrowser = new CTextureBrowser(this);

	//
	// !!!NOTE: Always do this last to ensure that the layout does not get recalculated before the
	//			window is maximized. This prevents control bars from being incorrectly wrapped to
	//			the next column.
	//
	if (VerifyBarState())
	{
		LoadBarState("Barstate");
	}

	return 0;
}


void CMainFrame::RecalcLayout( BOOL bNotify )
{
	CMDIFrameWnd::RecalcLayout(bNotify);
}


void CMainFrame::DockControlBarLeftOf(CControlBar* Bar, CControlBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_MAXIMIZE;
	cs.lpszClass = "VALVEWORLDCRAFT";

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


//-----------------------------------------------------------------------------
// Purpose: Maps menu IDs to tool IDs.
// Input  : uMsg - Menu ID from a WM_COMMAND message.
//-----------------------------------------------------------------------------
static ToolID_t _ToolMsgToEnum(UINT uMsg)
{
	struct ToolIDMap_t
	{
		UINT uMsg;
		ToolID_t eToolID;
	};

	ToolIDMap_t nIDMap[] =
	{
		{ ID_TOOLS_POINTER,		TOOL_POINTER },
		{ ID_TOOLS_BLOCK,		TOOL_BLOCK },
		{ ID_TOOLS_ENTITY,		TOOL_ENTITY },
		{ ID_TOOLS_CAMERA,		TOOL_CAMERA },
		{ ID_TOOLS_MAGNIFY,		TOOL_MAGNIFY },
		{ ID_TOOLS_MORPH,		TOOL_MORPH },
		{ ID_TOOLS_CLIPPER,		TOOL_CLIPPER },
		{ ID_TOOLS_EDITCORDON,	TOOL_EDITCORDON },
		{ ID_TOOLS_PATH,		TOOL_PATH },
		{ ID_TOOLS_OVERLAY,		TOOL_OVERLAY },
		{ ID_TOOLS_APPLYDECALS,	TOOL_DECAL },
		{ ID_MODE_APPLICATOR,	TOOL_FACEEDIT_MATERIAL },
	};

	for (int i = 0; i < sizeof(nIDMap) / sizeof(nIDMap[0]); i++)
	{
		if (uMsg == nIDMap[i].uMsg)
		{
			return nIDMap[i].eToolID;
		}
	}

	return TOOL_POINTER;
}


//-----------------------------------------------------------------------------
// Purpose: activates the current tool toolbar button
// Input  : pUI - interface to button that has had a action happen
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateToolUI(CCmdUI *pUI)
{
	if (IsShellSessionActive())
	{
		pUI->Enable(FALSE);
	}
	else
	{
		//
		// check for button enabling
		//
		CMapDoc *pDoc = CMapDoc::GetActiveMapDoc();

#if 0
		//
		// Only enable the displacement toolbar button while editing HalfLife 2 maps.
		//
		if ( pUI->m_nID == ID_TOOLS_DISPLACE )
		{
			if ( pDoc != NULL )
			{
				pUI->Enable((pDoc->GetMapFormat() == mfHalfLife2) || (pDoc->GetMapFormat() == mfTeamFortress2));
			}
			else
			{
				pUI->Enable( pDoc != NULL );
			}
		}
		else
#endif
		{
			pUI->Enable( pDoc != NULL );
		}		

		ToolID_t eToolID = _ToolMsgToEnum(pUI->m_nID);
		BOOL bEnable = CMapDoc::GetActiveMapDoc() ? TRUE : FALSE;
		pUI->Enable(bEnable);
		pUI->SetCheck(eToolID == g_pToolManager->GetToolID());
	}
}


//-----------------------------------------------------------------------------
// Purpose: Handles toolbar and menu messages that change the active tool.
// Input  : nMessageID - the id of the menu item
// Output : Returns TRUE to indicate that the message was handled.
//-----------------------------------------------------------------------------
BOOL CMainFrame::OnChangeTool(UINT nMessageID)
{
	//
	// Changing tool -- exit face edit mode if necessary.
	// This is here because face edit mode encompasses two tools: the
	// material tool and the displacement tool. Which tool we use is set
	// by the OnSetActive handler of each page of the face edit sheet.
	//
	if (IsInFaceEditMode())
	{
		EnableFaceEditMode(false);
	}

	//
	// Activate the new tool.
	//
	ToolID_t eToolID = _ToolMsgToEnum(nMessageID);
	g_pToolManager->SetTool(eToolID);
	return TRUE;
}


//-----------------------------------------------------------------------------
// Purpose: Brings up the 
//-----------------------------------------------------------------------------
void CMainFrame::OnViewMessages(void)
{
	pMsgWnd->ShowWindow(pMsgWnd->IsWindowVisible() ? SW_HIDE : SW_SHOWNA);
}


//-----------------------------------------------------------------------------
// Purpose: Manages the state of the view messages menu item.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateViewMessages(CCmdUI *pCmdUI) 
{
	pCmdUI->SetCheck(pMsgWnd->IsWindowVisible());
}


//-----------------------------------------------------------------------------
// Purpose: Brings up the Object Properties dialog.
//-----------------------------------------------------------------------------
void CMainFrame::OnEditProperties(void)
{
	pObjectProperties->ShowWindow(pObjectProperties->IsWindowVisible() ? SW_HIDE : SW_SHOW);
}


//-----------------------------------------------------------------------------
// Purpose: Tell all the documents to redraw all their views.
//-----------------------------------------------------------------------------
void CMainFrame::UpdateAllDocViews(DWORD dwCmd)
{
	POSITION p = CMapDoc::GetFirstDocPosition();
	while (p != NULL)
	{
		CMapDoc *pDoc = CMapDoc::GetNextDoc(p);

		if (pDoc->GetGame() != NULL)
		{
			pDoc->UpdateAllViews(NULL, dwCmd);
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Informs our application object when we are activated or deactivated.
// Input  : bActive - TRUE to activate, FALSE to deactivate.
//			hTask - task becoming active.
//-----------------------------------------------------------------------------
void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask)
{
	APP()->OnActivateApp(bActive == TRUE);
}


//-----------------------------------------------------------------------------
// Purpose: Called when the active document is deleted.
//-----------------------------------------------------------------------------
void CMainFrame::OnDeleteActiveDocument(void)
{
	pObjectProperties->AbortData();
}


//-----------------------------------------------------------------------------
// Purpose: Handles resize messages. Resizes any children that depend on our size.
// Input  : nType - 
//			cx - 
//			cy - 
//-----------------------------------------------------------------------------
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);

	//
	// Resize the message window if it exists.
	//
	if (pMsgWnd != NULL)
	{
		CRect clientrect;
		wndMDIClient.GetClientRect(clientrect);

		pMsgWnd->MoveWindow(CRect(0, clientrect.Height() - 130, clientrect.Width(), clientrect.Height()));
	}
}


//-----------------------------------------------------------------------------
// Purpose: Returns a pointer to the main frame window.
//-----------------------------------------------------------------------------
CMainFrame *GetMainWnd(void)
{
	return pMainWnd;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nIndex - 
//			pszText - 
//-----------------------------------------------------------------------------
void SetStatusText(int nIndex, LPCTSTR pszText)
{
	GetMainWnd()->GetStatusBar()->SetPaneText(nIndex, pszText);
}


//-----------------------------------------------------------------------------
// Purpose: Invokes the configuration dialog, saving the options if the user
//			hits the OK button.
//-----------------------------------------------------------------------------
void CMainFrame::Configure(void)
{
	COptionProperties dlg("Configure Worldcraft", NULL, 0);
	if (dlg.DoModal() == IDOK)
	{
		Options.Write(TRUE);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Invokes the configuration dialog.
//-----------------------------------------------------------------------------
void CMainFrame::OnToolsOptions(void)
{
	Configure();
}


//-----------------------------------------------------------------------------
// Purpose: Called when the main frame is closing. Cleans up the dialog bars
//			and saves the options to the registry.
//-----------------------------------------------------------------------------
void CMainFrame::OnClose() 
{
	// want to save the faceeditor as hidden
	ShowFaceEditSheetOrTextureBar( false );
	
	SaveBarState("Barstate");
	//AfxGetApp()->WriteProfileInt("General", "NewBars", TRUE);

	//
	// Save the splitter configuration of the first child window in our list.
	//
	CChildFrame *pChild = GetNextMDIChildWnd(NULL);
	if (pChild != NULL)
	{
		pChild->SaveOptions();
	}
	
	// Remove the smoothing group dialog window.
	m_SmoothingGroupDlg.DestroyWindow();

	// save options
	Options.Write(TRUE);

	CMDIFrameWnd::OnClose();
}


//-----------------------------------------------------------------------------
// Purpose: Sets a timer for destroying the splash screen.
//-----------------------------------------------------------------------------
void CMainFrame::OnPaint(void)
{
	static BOOL bFirst = TRUE;

	CPaintDC dc(this); // device context for painting

	if (bFirst)
	{
		bFirst = FALSE;
		SetTimer(1, 500, NULL);
	}
}


//-----------------------------------------------------------------------------
// Purpose: This is called ONCE when the splash wnd is to be destroyed. OnPaint()
//			sets the timer.
// Input  : nIDEvent - 
//-----------------------------------------------------------------------------
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	static BOOL bFirst = TRUE;

	if (!::IsWindow(m_hWnd))
	{
		return;
	}

	// only want it once
	KillTimer(nIDEvent);

	if (CSplashWnd::c_pSplashWnd)
	{
		CSplashWnd::c_pSplashWnd->HideSplashScreen();
	}

	SetBrightness(Options.textures.fBrightness);

	// repaint texture window
	m_TextureBar.Invalidate();

	if (bFirst && Options.configs.nConfigs == 0)
	{
		if (AfxMessageBox(IDS_NO_CONFIGS_AVAILABLE, MB_YESNO) == IDYES)
		{
			APP()->Help("Worldcraft_Setup_Guide.htm");
		}

		Configure();
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : fBrightness - 
//-----------------------------------------------------------------------------
void CMainFrame::SetBrightness(float fBrightness)
{
	if(fBrightness < 0.1f || fBrightness > 5.0f)
		return;

	// update options
	Options.textures.fBrightness = fBrightness;

	// update display
	for(int i = 0; i < Options.configs.nConfigs; i++)
		Options.configs.Configs[i]->Palette.SetBrightness(fBrightness);
	g_Textures.InformPaletteChanged();

	//
	// if current tool isn't the material tool, then redraw the texture bar
	//
	if ( g_pToolManager->GetToolID() != TOOL_FACEEDIT_MATERIAL )
	{
		m_TextureBar.RedrawWindow();
	}
	else
	{
		m_pFaceEditSheet->RedrawWindow();
	}

	// tell all the documents to redraw 3d views
	POSITION p = CMapDoc::GetFirstDocPosition();

	while(p)
	{
		CMapDoc *pDoc = CMapDoc::GetNextDoc(p);
		pDoc->UpdateAllViews(NULL, MAPVIEW_UPDATE_DISPLAY | MAPVIEW_COLOR_ONLY);
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nID - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CMainFrame::OnView3dChangeBrightness(UINT nID) 
{
	float fBrightness = Options.textures.fBrightness;
	float fModify = (nID == ID_VIEW3D_BRIGHTER) ? 0.2f : -0.2f;
	
	SetBrightness(fBrightness + fModify);

	return TRUE;
}


//-----------------------------------------------------------------------------
// Purpose: Toggles face edit mode, which encompasses two different tools, the
//			materials editing tool and the displacement editing tool.
//
//			The tool itself is set by the OnSetActive handler for each property
//			page of the face properties sheet.
//-----------------------------------------------------------------------------
void CMainFrame::OnApplicator(UINT nID)
{
	bool bNewFaceEditMode = !IsInFaceEditMode();

	//
	// Show/hide face edit sheet/texturebar and update the selection set if need be.
	//
	EnableFaceEditMode(bNewFaceEditMode);

	if (!bNewFaceEditMode)
	{
		g_pToolManager->SetTool(TOOL_POINTER);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Enables or disables face edit mode, updating the UI as necessary.
//			When we are in face edit mode, the texture bar is hidden and the
//			face edit property sheet is shown. The active tool is changed
//			to either the material tool or the displacement tool based on which
//			page in the property sheet is active.
//-----------------------------------------------------------------------------
void CMainFrame::EnableFaceEditMode(bool bEnable)
{
	CMapDoc *pDoc = CMapDoc::GetActiveMapDoc();
	if (!pDoc)
	{
		return;
	}

	ShowFaceEditSheetOrTextureBar(bEnable);
	pDoc->UpdateForApplicator(bEnable);
}


//-----------------------------------------------------------------------------
// Purpose: Returns true if we are in face edit mode, false if not.
//-----------------------------------------------------------------------------
bool CMainFrame::IsInFaceEditMode(void)
{
	if ((g_pToolManager->GetToolID() == TOOL_FACEEDIT_MATERIAL) || (g_pToolManager->GetToolID() == TOOL_FACEEDIT_DISP))
	{
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Manages the state of the texture applicator toobar button and menu item.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateApplicatorUI(CCmdUI *pUI)
{
	if (IsShellSessionActive())
	{
		pUI->Enable(FALSE);
	}
	else
	{
		pUI->SetCheck(IsInFaceEditMode());
		pUI->Enable(CMapDoc::GetActiveMapDoc() ? TRUE : FALSE);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Invokes the prefab manager dialog.
//-----------------------------------------------------------------------------
void CMainFrame::OnToolsPrefabfactory(void)
{
	CPrefabsDlg dlg;
	dlg.DoModal();
	CPrefabLibrary::LoadAllLibraries();
	m_ObjectBar.UpdateListForTool(g_pToolManager->GetToolID());
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMainFrame::OnHelpFinder(void)
{
	APP()->Help(NULL);
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : pHelpInfo - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CMainFrame::OnHelpInfo(HELPINFO *pHelpInfo)
{
	return(Default());
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : * pszURL - 
//			hWnd - 
// Output : static void
//-----------------------------------------------------------------------------
static void OpenURL(const char *pszURL, HWND hWnd)
{
	if(HINSTANCE(32) > ::ShellExecute(hWnd, "open", pszURL, NULL, NULL, 0))
	{
		AfxMessageBox("The website couldn't be opened.");
	}
}


//-----------------------------------------------------------------------------
// Purpose: Opens the URL that corresponds to the given string ID. This is used
//			to hook menu items to URLs in the string table.
//-----------------------------------------------------------------------------
void CMainFrame::OnHelpOpenURL(int nID)
{
	CString str;
	str.LoadString(nID);
	OpenURL(str, m_hWnd);
}


//-----------------------------------------------------------------------------
// Purpose: Activates or deactivates Undo/Redo.
//-----------------------------------------------------------------------------
void CMainFrame::SetUndoActive(BOOL bActive)
{
	m_bUndoActive = bActive;
	CMapDoc::GetActiveMapDoc()->SetUndoActive(bActive == TRUE);
}


//-----------------------------------------------------------------------------
// Purpose: Toggles the active state of Undo/Redo.
//-----------------------------------------------------------------------------
void CMainFrame::OnEditUndoredoactive(void)
{
	SetUndoActive(!m_bUndoActive);	
}


//-----------------------------------------------------------------------------
// Purpose: Manages the state of the Enable/Disable Undo/Redo menu item.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateEditUndoredoactive(CCmdUI *pCmdUI) 
{
	pCmdUI->Enable(IsShellSessionActive() ? FALSE : TRUE);
	pCmdUI->SetText(m_bUndoActive ? "Disable Undo/Redo" : "Enable Undo/Redo");
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCode - 
//-----------------------------------------------------------------------------
void CMainFrame::GlobalNotify(int nCode)
{
	CMapDoc *pDoc = CMapDoc::GetActiveMapDoc();

	switch (nCode)
	{
		//
		// Active document changed. Update visgroup lists. 
		//
		case WM_MAPDOC_CHANGED:
		{
			//
			// Update the visgroups.
			//
			m_FilterControl.UpdateGroupList();
		
			//
			// If the Object Properties dialog has a Groups tab, update
			// the groups tab.
			//
			if (pObjectProperties != NULL)
			{
				pObjectProperties->UpdateGrouplist();
			}

			if (pDoc != NULL)
			{
				pDoc->UpdateStatusbar();

				#ifndef SDK_BUILD
				m_AnimationDlg.SelectionChanged(*pDoc->Selection_GetList());
				#endif // SDK_BUILD
			}
	
			break;
		}
		
		//
		// Game configuration changed. Update texture and entity lists.
		//
		case WM_GAME_CHANGED:
		{
			m_TextureBar.NotifyGraphicsChanged();
			m_pFaceEditSheet->NotifyGraphicsChanged();
			m_ObjectBar.UpdateListForTool(g_pToolManager->GetToolID());
			break;
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : UINT - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CMainFrame::OnFileNew(UINT)
{
	return FALSE;

	CNewDocType dlg;
	dlg.m_iNewType = 0;

	if(dlg.DoModal() != IDOK)
		return TRUE;

	return FALSE;
}


//-----------------------------------------------------------------------------
// Purpose: Saves the position and types of all 2D and 3D views in the active document.
// dvs: This really needs to be a text file instead of a binary file!
// Input  : *pFile - 
//-----------------------------------------------------------------------------
void CMainFrame::SaveWindowStates(fstream *pFile)
{
	fstream file("winstate.wc", ios::out | ios::binary);

	CMapDoc *pDoc = CMapDoc::GetActiveMapDoc();
	if (pDoc == NULL)
	{
		return;
	}

	file.write(WINSTATETAG, sizeof WINSTATETAG);
	file.write((char*) &fVersion, sizeof fVersion);

	CRect rectClient;
	::GetClientRect(m_hWndMDIClient, &rectClient);

	// write out each view
	POSITION p = pDoc->GetFirstViewPosition();
	while (p != NULL)
	{
		CView *pView = pDoc->GetNextView(p);
		
		//
		// Determine what type of view it is.
		//
		int iDrawType;
		if (pView->IsKindOf(RUNTIME_CLASS(CMapView2D)))
		{
			file.write((char*) &WINSTATE2DVIEW, sizeof WINSTATE2DVIEW);
			iDrawType = (int)((CMapView2D*)pView)->GetDrawType();
		}
		else if (pView->IsKindOf(RUNTIME_CLASS(CMapView3D)))
		{
			file.write((char*) &WINSTATE3DVIEW, sizeof WINSTATE3DVIEW);
			iDrawType = (int)((CMapView3D*)pView)->GetDrawType();
		}
		else
		{
			//
			// It's a view type whose state we do not save - skip it.
			//
			continue;
		}

		//
		// Write view's draw type.
		//
		file.write((char*) &iDrawType, sizeof iDrawType);

		//
		// Write position of view.
		//
		CRect rectView;
		pView->GetParentFrame()->GetWindowRect(&rectView);
		CPoint pt1 = rectView.TopLeft(), pt2 = rectView.BottomRight();
		::ScreenToClient(m_hWndMDIClient, &pt1);
		::ScreenToClient(m_hWndMDIClient, &pt2);
	
		double left, top, right, bottom;
		left =		double(pt1.x) / double(rectClient.right);
		top =		double(pt1.y) / double(rectClient.bottom);
		right =		double(pt2.x) / double(rectClient.right);
		bottom =	double(pt2.y) / double(rectClient.bottom);

		file.write((char*) &left, sizeof left);
		file.write((char*) &top, sizeof top);
		file.write((char*) &right, sizeof right);
		file.write((char*) &bottom, sizeof bottom);
	}

	file.write((char *)&WINSTATEEND, sizeof WINSTATEEND);
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : pFile - 
//-----------------------------------------------------------------------------
void CMainFrame::LoadWindowStates(fstream *pFile)
{
	fstream file("winstate.wc", ios::in | ios::nocreate | ios::binary);

	if (!file.is_open())
	{
		return;
	}

	char tag[sizeof(WINSTATETAG)];
	file.read(tag, sizeof tag);

	if(memcmp(tag, WINSTATETAG, sizeof tag))
	{
		file.seekg(-int(sizeof(tag)));
		return;
	}

	float fThisVersion;
	file.read((char*) &fThisVersion, sizeof fThisVersion);

	CMapDoc *pDoc = CMapDoc::GetActiveMapDoc();

	if(!pDoc)
		return;

	// get client rect of MDI CHILD for relative positioning information
	CRect rectClient;
	::GetClientRect(m_hWndMDIClient, &rectClient);

	// keep list of views we've already modified, so if there
	//  are other views, we have to create them. this prevents
	//  us from having to delete all views and start over, 
	//  which is a slower process than simply moving existing
	//	views.
	CTypedPtrList<CPtrList, CView*> UsedViews;

	SetDefaultChildType(FALSE);

	while (1)
	{
		int iViewType;
		file.read((char *)&iViewType, sizeof iViewType);
		if ((file.eof()) || (iViewType == WINSTATEEND))
		{
			break;
		}

		int iDrawType;
		file.read((char *)&iDrawType, sizeof iDrawType);

		CView *pView = NULL;

		// find a view we haven't used
		POSITION p = pDoc->GetFirstViewPosition();
		while (p != NULL)
		{
			CView *pThisView = pDoc->GetNextView(p);

			// already used?
			if (UsedViews.Find(pThisView))
			{
				continue;
			}

			// make sure it's the right type ..
			if (iViewType == WINSTATE2DVIEW && !pThisView->IsKindOf(RUNTIME_CLASS(CMapView2D)))
			{
				continue;
			}

			if (iViewType == WINSTATE3DVIEW && !pThisView->IsKindOf(RUNTIME_CLASS(CMapView3D)))
			{
				continue;
			}

			// yes! so modify this one.
			pView = pThisView;
			UsedViews.AddTail(pView);
			break;
		}

		CChildFrame *pFrame = NULL;
		BOOL bNew = FALSE;
		CDocTemplate *pTemplate = NULL;

		if(!pView)
		{
			// if no view was created, we have to create a new one.
			CMDIChildWnd* pActiveChild = MDIGetActive();
			pTemplate = pDoc->GetDocTemplate();
			pFrame = (CChildFrame*) pTemplate->CreateNewFrame(pDoc, pActiveChild);
			pFrame->SetRedraw(FALSE);
			pTemplate->InitialUpdateFrame(pFrame, pDoc, FALSE);
			
			// find view in new frame
			pView = pFrame->GetActiveView();

			UsedViews.AddTail(pView);
			bNew = TRUE;
		}
		else
		{
			// find frame based on this view
			pFrame = (CChildFrame*) pView->GetParentFrame();

			if(pFrame->bUsingSplitter)
				pFrame->SetSplitterMode(FALSE);
		}

		// no redraws right now, please.
		pFrame->SetRedraw(FALSE);

		if (iViewType == WINSTATE3DVIEW)
		{
			//
			// Handle import of old WinState files before draw types were consolidated
			// into a single enumeration.
			//
			if ((iDrawType >= VIEW2D_XY) && (iDrawType <= VIEW2D_XZ))
			{
				iDrawType += 3;
			}
			pFrame->SetViewType((DrawType_t)iDrawType);
		}
		else
		{
			pFrame->SetViewType((DrawType_t)iDrawType);
		}
		
		// read positioning info
		double left, top, right, bottom;
		file.read((char*) &left, sizeof left);
		file.read((char*) &top, sizeof top);
		file.read((char*) &right, sizeof right);
		file.read((char*) &bottom, sizeof bottom);
		CRect r;
		r.left		= int(left * double(rectClient.right));
		r.top		= int(top * double(rectClient.bottom));
		r.right		= int(right * double(rectClient.right));
		r.bottom	= int(bottom * double(rectClient.bottom));

		// Set the frame's position.
		pFrame->MoveWindow(&r, FALSE);

		// Call OnInitialUpdate before any rendering takes place.
		if (bNew)
		{
			pTemplate->InitialUpdateFrame(pFrame, pDoc, TRUE);
		}

		// Enable WM_PAINT messages for the frame window.
		pFrame->SetRedraw(TRUE);

		// Update the window.
		pFrame->Invalidate();
		pFrame->UpdateWindow();
	}

	Invalidate();
	UpdateWindow();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMainFrame::OnSavewindowstate(void)
{
	SaveWindowStates();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMainFrame::OnLoadwindowstate(void)
{
	LoadWindowStates();
}


//-----------------------------------------------------------------------------
// Purpose: Changes the format of the units displayed in the status bar.
// Input  : nID - Menu ID corresponding to a units format.
//-----------------------------------------------------------------------------
void CMainFrame::OnUnits(int nID)
{
	switch (nID)
	{
		case ID_MAP_UNITS_NONE:
		{
			Box3D::SetWorldUnits(Units_None);
			break;
		}

		case ID_MAP_UNITS_INCHES:
		{
			Box3D::SetWorldUnits(Units_Inches);
			break;
		}

		case ID_MAP_UNITS_FEET_INCHES:
		{
			Box3D::SetWorldUnits(Units_Feet_Inches);
			break;
		}
	}

	CMapDoc::GetActiveMapDoc()->UpdateStatusbar();
}


//-----------------------------------------------------------------------------
// Purpose: Manages the state of decal application toolbar button.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateUnits(CCmdUI *pCmdUI) 
{
	pCmdUI->Enable(!IsShellSessionActive());

	if (pCmdUI->m_nID == ID_MAP_UNITS_NONE)
	{
		pCmdUI->SetCheck(Box3D::GetWorldUnits() == Units_None);
	}
	else if (pCmdUI->m_nID == ID_MAP_UNITS_INCHES)
	{
		pCmdUI->SetCheck(Box3D::GetWorldUnits() == Units_Inches);
	}
	else if (pCmdUI->m_nID == ID_MAP_UNITS_FEET_INCHES)
	{
		pCmdUI->SetCheck(Box3D::GetWorldUnits() == Units_Feet_Inches);
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : pMsg - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	//
	// See if the message is a keydown and the current focus window is the 
	// ComboBox in the ObjectBar!
	//
	/*
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((GetFocus() == &m_ObjectBar) || (GetFocus() == &this->m_FilterControl))
		{
			AfxMessageBox("Ok");
			return(TRUE);
		}
	}
	*/

	return(CMDIFrameWnd::PreTranslateMessage(pMsg));
}


//-----------------------------------------------------------------------------
// Purpose: Finds the next CChildFrame in the list of MDI child windows.
// Input  : pCurChild - Child to search from.
//-----------------------------------------------------------------------------
CChildFrame *CMainFrame::GetNextMDIChildWnd(CChildFrame *pCurChild)
{
	return GetNextMDIChildWndRecursive(pCurChild);
} 


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CChildFrame *CMainFrame::GetNextMDIChildWndRecursive(CWnd *pCurChild)
{
	CWnd *pNextChild = NULL;

	if (pCurChild == NULL)
	{
		// Get the first child window.
		pNextChild = wndMDIClient.GetWindow(GW_CHILD);
	}
	else
	{
		// Get the next child window in the list.
		pNextChild = pCurChild->GetWindow(GW_HWNDNEXT);
		if (!pNextChild)
		{
			// No child windows exist in the MDIClient,
			// or you are at the end of the list. This check
			// will terminate any recursion.
			return NULL;
		}
	}

	// Check the kind of window
    if (!pNextChild->GetWindow(GW_OWNER))
	{
        if (pNextChild->IsKindOf(RUNTIME_CLASS(CChildFrame)))
		{
			return (CChildFrame *)pNextChild;
		}
	}

	// Not one we are interested in. Try the next one.
	// Recurse over the window manager's list of windows.
	return GetNextMDIChildWndRecursive(pNextChild);
}


//-----------------------------------------------------------------------------
// Purpose: Returns true if we are currently editing via the engine, false if not.
//-----------------------------------------------------------------------------
bool CMainFrame::IsShellSessionActive(void)
{
	return(m_bShellSessionActive);
}


//-----------------------------------------------------------------------------
// Purpose: Manages the state of all Edit menu items and toolbar buttons.
//-----------------------------------------------------------------------------
void CMainFrame::OnUpdateEditFunction(CCmdUI *pCmdUI) 
{
	pCmdUI->Enable(!IsShellSessionActive());
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CMainFrame::ShowFaceEditSheetOrTextureBar( bool bShowFaceEditSheet )
{
	if( bShowFaceEditSheet )
	{
		m_pFaceEditSheet->SetVisibility( true );
		ShowControlBar( &m_TextureBar, FALSE, TRUE );
	}
	else
	{
		m_pFaceEditSheet->SetVisibility( false );
		m_pFaceEditSheet->CloseAllPageDialogs();
		ShowControlBar( &m_TextureBar, TRUE, TRUE );	
	}
}


//-----------------------------------------------------------------------------
// Purpose: Displays the search/replace dialog. It will be hide itself when the
//			user closes it.
//-----------------------------------------------------------------------------
void CMainFrame::ShowSearchReplaceDialog(void)
{
	if (m_pSearchReplaceDlg == NULL)
	{
		m_pSearchReplaceDlg = new CSearchReplaceDlg;
		m_pSearchReplaceDlg->Create(this);
	}

	m_pSearchReplaceDlg->ShowWindow(SW_SHOW);
	m_pSearchReplaceDlg->SetFocus();
}


//-----------------------------------------------------------------------------
// Purpose: Code found on codeproject.com.
//
//			Makes sure we don't have bogus dialog bar information in the registry.
//			This prevents a crash in the MFC code if a version of the editor with
//			different toolbars was run before us.
//
// TODO: fix the registry settings if they are bad so we can still load the bar state
//
// Output : Returns true if it is safe to load the toolbar settings, false if not.
//-----------------------------------------------------------------------------
bool CMainFrame::VerifyBarState(void)
{
    CDockState state;
    state.LoadState("BarState");

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];

        ASSERT(pInfo != NULL);

        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0)
				{
					continue; // row separator
				}

                if (nID > 0xFFFF)
				{
                    nID &= 0xFFFF; // placeholder - get the ID
				}

                if (GetControlBar(nID) == NULL)
				{
                    return false;
				}
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
		{
            if (GetControlBar(pInfo->m_nBarID) == NULL)
			{
                return false; // invalid bar ID
			}
		}
    }

    return true;
}