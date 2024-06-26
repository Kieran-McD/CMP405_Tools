#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_TEXTURE, &MFCMain::MenuTextureSelect)
	ON_COMMAND(ID_EDIT_CREATEOBJECT, &MFCMain::MenuCreateObjectSelect)
	ON_COMMAND(ID_EDIT_MODIFYOBJECT, &MFCMain::MenuModifyObjectSelect)
	ON_COMMAND(ID_BUTTON40001, &MFCMain::ToolBarButton1)
	ON_COMMAND(ID_BUTTON40007, &MFCMain::ToolBarButton2)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}

int MFCMain::Run()
{


	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{

		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{



			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);

			switch (msg.message)
			{
				//Global inputs,  mouse position and keys etc
			case WM_RBUTTONDOWN:
				//m_ToolSelectDialogue.DeSelect
				//if (m_ToolSelectDialogue) {
				//	m_ToolSelectDialogue.DeSelect();
				//}
				break;
			case WM_KEYDOWN:
				//Press Delete
				if (msg.wParam == 46)
				{

				}
			
				break;
			}
		}
		else
		{	
			//int ID = m_ToolSystem.getCurrentSelectionID();
			//std::wstring statusString = L"Selected Object: " + std::to_wstring(ID);

			//Check for deleted objects
			if (m_ToolSystem.UpdateDeleteObjects()) {
				if (m_ToolSelectDialogue) m_ToolSelectDialogue.DeleteSelected();
				if (m_ToolModifyObjectDialogue) m_ToolModifyObjectDialogue.UpdateDelete();
			}
		
			m_ToolSystem.Tick(&msg);

			//Check for scene data update from tools
			if (m_ToolSystem.m_updatedSceneData) {
				if (m_ToolModifyObjectDialogue) m_ToolModifyObjectDialogue.UpdateParameters();
				m_ToolSystem.m_updatedSceneData = false;
			}

			//Updates the selected
			if (m_ToolSystem.UpdateSelected) {
				if(m_ToolSelectDialogue) m_ToolSelectDialogue.UpdatedSelected();
				if (m_ToolModifyObjectDialogue) m_ToolModifyObjectDialogue.UpdateObjectList();

				m_ToolSystem.UpdateSelected = false;
			}

			//Handles when the select object list needs updated
			if (m_ToolSelectDialogue) {
				if (m_ToolSystem.UpdateList()) {
					m_ToolSelectDialogue.UpdateList();
					m_ToolSelectDialogue.UpdatedSelected();
				}
				if (m_ToolSelectDialogue.m_listUpdated && m_ToolModifyObjectDialogue) {
					m_ToolModifyObjectDialogue.UpdateObjectList();
					m_ToolSelectDialogue.m_listUpdated = false;
				}
			}

			//Handles when the scene needs rebuilt
			if (m_ToolTextureDialogue.RebuildScene || m_ToolCreateObjectDialogue.RebuildScene || m_ToolModifyObjectDialogue.RebuildScene) {				
				m_ToolSystem.onActionRebuildScene();
				if (m_ToolSelectDialogue) {
					m_ToolSelectDialogue.UpdateList();
					m_ToolSelectDialogue.UpdatedSelected();
				}
				
				m_ToolTextureDialogue.RebuildScene = false;
				m_ToolCreateObjectDialogue.RebuildScene = false;
				m_ToolModifyObjectDialogue.RebuildScene = false;
			}

			m_ToolSystem.UpdateLastFrameInput();
			//send current object ID to status bar in The main frame
			//##m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);	
		}
	}

	return (int)msg.wParam;
}



void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

//Handles launching the object selection menu
void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	
	//Initialises object selection box
	if (!m_ToolSelectDialogue) {
		m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
		m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedID);
		
	}
	else {
		m_ToolSelectDialogue.ShowWindow(SW_SHOW);
	}
	
}

//Handles launching the texture modify menu
void MFCMain::MenuTextureSelect()
{
	//Initialises Texture selection box
	if (!m_ToolTextureDialogue) {
		m_ToolTextureDialogue.Create(IDD_DIALOG2);	//Start up modeless
		m_ToolTextureDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolTextureDialogue.FindTextures(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedID);
	}
	else {
		m_ToolTextureDialogue.ShowWindow(SW_SHOW);
	}
}

//Handles launching the create object menu
void MFCMain::MenuCreateObjectSelect()
{
	//Initialises object creation dialog box
	if (!m_ToolCreateObjectDialogue) {
		m_ToolCreateObjectDialogue.Create(IDD_DIALOG3);	//Start up modeless
		m_ToolCreateObjectDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolCreateObjectDialogue.RetrieveData(&m_ToolSystem.m_sceneGraph);
	}
	else {
		m_ToolCreateObjectDialogue.ShowWindow(SW_SHOW);
	}
}

//Handles launching the modify object menu
void MFCMain::MenuModifyObjectSelect()
{
	//Initialises object creation dialog box
	if (!m_ToolModifyObjectDialogue) {
		m_ToolModifyObjectDialogue.Create(IDD_DIALOG4);	//Start up modeless
		m_ToolModifyObjectDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolModifyObjectDialogue.RetrieveData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedID);
	}
	else {
		m_ToolModifyObjectDialogue.ShowWindow(SW_SHOW);
	}
}

//Saves the scene
void MFCMain::ToolBarButton1()
{	
	m_ToolSystem.onActionSave();
}

//Used to load the saved scene
void MFCMain::ToolBarButton2() {
	m_ToolSystem.onActionLoad();
	if (m_ToolSelectDialogue) {
		m_ToolSelectDialogue.UpdateList();
	}

	MessageBox(NULL, L"Level Loaded", L"Notification", MB_OK);
}
//Rebuilds the rendered scene
void MFCMain::OnActionRebuildScene()
{
	m_ToolSystem.onActionRebuildScene();
}

MFCMain::MFCMain()
{

}


MFCMain::~MFCMain()
{
}
