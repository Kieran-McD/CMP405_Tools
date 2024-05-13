//
// Game.cpp
//

#include "DisplayObject.h"
#include "Game.h"
#include "pch.h"
#include <string>


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game()

{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
	m_displayList.clear();
	//initial Settings
	//modes
	m_grid = true;
}

Game::~Game()
{

#ifdef DXTK_AUDIO
    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }
#endif
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height, std::vector<int>* selectedID)
{
    m_gamePad = std::make_unique<GamePad>();

    m_keyboard = std::make_unique<Keyboard>();

    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    GetClientRect(window, &m_windowSize);

    m_selectedID = selectedID;

#ifdef DXTK_AUDIO
    // Create DirectXTK for Audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine = std::make_unique<AudioEngine>(eflags);

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_waveBank = std::make_unique<WaveBank>(m_audEngine.get(), L"adpcmdroid.xwb");

    m_soundEffect = std::make_unique<SoundEffect>(m_audEngine.get(), L"MusicMono_adpcm.wav");
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

    m_effect1->Play(true);
    m_effect2->Play();
#endif
}

void Game::SetGridState(bool state)
{
	m_grid = state;
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick(InputCommands *Input)
{
	//copy over the input commands so we have a local version to use elsewhere.
	m_InputCommands = *Input;



    m_timer.Tick([&]()
    {
        Update(m_timer);
    });


#ifdef DXTK_AUDIO
    // Only update audio engine once per frame
    if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
#endif

    Render();
}

void Game::UpdateWidgets()
{
    if (m_selectedID->size() > 0) {
        for (int i = 0; i < m_widgetList.size(); i++) {
            //Sets teh appropriate rotation/position of the widgets
            switch (i)
            {
            case 0:
                m_widgetList[i].m_position = m_displayList[m_selectedID->at(0)].m_position;
                break;
            case 1:
                m_widgetList[i].m_position = m_displayList[m_selectedID->at(0)].m_position;
                m_widgetList[i].m_orientation = Vector3(0, 0, 90);
                break;
            case 2:
                m_widgetList[i].m_position = m_displayList[m_selectedID->at(0)].m_position;
                m_widgetList[i].m_orientation = Vector3(0, 270, 0);
                break;
            }

            DisplayObject object = m_widgetList[i];

            //Gets the direction from the camera to the objects
            Vector3 direction = Vector3(object.m_position.x, object.m_position.y, object.m_position.z) - m_cam.GetCamPosition();
            direction.Normalize();

            //Places the widget 3 units in the direction of the object
            m_widgetList[i].m_position = m_cam.GetCamPosition() + direction * 3.f;

            //Sets the position of the widgets
            switch (i)
            {
            case 0:
                m_widgetList[i].m_position += Vector3(0.375f, 0, 0);
                break;
            case 1:
                m_widgetList[i].m_position += Vector3(0, 0.375f, 0);
                break;
            case 2:
                m_widgetList[i].m_position += Vector3(0, 0, 0.375f);
                break;
            }

        }
    }
}

bool Game::UpdateMoveWidget(InputCommands* Input)
{
    if (m_selectedID->size() < 1) return false;
    if (m_currentWidgetSelected < 0) return false;

    //Checks if the scene objects needs to updates
    if (!Input->mouseLeftButton && m_widgetClicked) {
        m_widgetClicked = false;
        return true;
    }

    if (!Input->mouseLeftButton) return false;
    

    Vector3 changeInMovement = Vector3(0, 0, 0);
    float direction;
    switch (m_currentWidgetSelected)
    {
        //X Axis transform
    case 0:
        direction = m_cam.GetCamRightDir().Dot(Vector3(1, 0, 0));
        changeInMovement = Vector3(m_InputCommands.mouseXDrag / 50.f, 0, 0) * direction;
        direction = m_cam.GetCamUpDir().Dot(Vector3(1, 0, 0));
        changeInMovement += Vector3(-m_InputCommands.mouseYDrag / 50.f, 0, 0) * -direction;
        break;
        //Y axis transform
    case 1:
        direction = m_cam.GetCamRightDir().Dot(Vector3(0, 1, 0));
        changeInMovement = Vector3(0, m_InputCommands.mouseXDrag / 50.f, 0) * direction;
        direction = m_cam.GetCamUpDir().Dot(Vector3(0, 1, 0));
        changeInMovement += Vector3(0, m_InputCommands.mouseYDrag / 50.f, 0) * direction;
        break;
    case 2:
        //Z axis transform
        direction = m_cam.GetCamRightDir().Dot(Vector3(0, 0, 1));
        changeInMovement = Vector3(0, 0, m_InputCommands.mouseXDrag / 50.f) * direction;
        direction = m_cam.GetCamUpDir().Dot(Vector3(0, 0, 1));
        changeInMovement += Vector3(0, 0, m_InputCommands.mouseYDrag / 50.f) * direction;
        break;
    }

    //Updates display objects positon
    for (int i = 0; i < m_selectedID->size(); i++) {
        m_displayList.at(m_selectedID->at(i)).m_position += changeInMovement;
    }
    return false;
}

//Used to set the scene tranform data to the display object data, when display object is used to change the data
void Game::UpdateSceneObjects(std::vector<SceneObject>* SceneGraph)
{
    for (int i = 0; i < m_selectedID->size(); i++) {
         SceneGraph->at(m_selectedID->at(i)).posX = m_displayList.at(m_selectedID->at(i)).m_position.x;
         SceneGraph->at(m_selectedID->at(i)).posY = m_displayList.at(m_selectedID->at(i)).m_position.y;
         SceneGraph->at(m_selectedID->at(i)).posZ = m_displayList.at(m_selectedID->at(i)).m_position.z;

         SceneGraph->at(m_selectedID->at(i)).rotX = m_displayList.at(m_selectedID->at(i)).m_orientation.x;
         SceneGraph->at(m_selectedID->at(i)).rotY = m_displayList.at(m_selectedID->at(i)).m_orientation.y;
         SceneGraph->at(m_selectedID->at(i)).rotZ = m_displayList.at(m_selectedID->at(i)).m_orientation.z;

         SceneGraph->at(m_selectedID->at(i)).scaX = m_displayList.at(m_selectedID->at(i)).m_scale.x;
         SceneGraph->at(m_selectedID->at(i)).scaY = m_displayList.at(m_selectedID->at(i)).m_scale.y;
         SceneGraph->at(m_selectedID->at(i)).scaZ = m_displayList.at(m_selectedID->at(i)).m_scale.z;
    }
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{   
    Ray ray = Ray();


    m_cam.Update(m_InputCommands);
    //UpdateMoveWidget();
    UpdateWidgets();

    m_batchEffect->SetView(m_cam.GetViewMatrix());
    m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainEffect->SetView(m_cam.GetViewMatrix());
	m_displayChunk.m_terrainEffect->SetWorld(Matrix::Identity);


#ifdef DXTK_AUDIO
    m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
    if (m_audioTimerAcc < 0)
    {
        if (m_retryDefault)
        {
            m_retryDefault = false;
            if (m_audEngine->Reset())
            {
                // Restart looping audio
                m_effect1->Play(true);
            }
        }
        else
        {
            m_audioTimerAcc = 4.f;

            m_waveBank->Play(m_audioEvent++);

            if (m_audioEvent >= 11)
                m_audioEvent = 0;
        }
    }
#endif

   
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

	if (m_grid)
	{
		// Draw procedurally generated dynamic grid
		const XMVECTORF32 xaxis = { 512.f, 0.f, 0.f };
		const XMVECTORF32 yaxis = { 0.f, 0.f, 512.f };
		DrawGrid(xaxis, yaxis, g_XMZero, 512, 512, Colors::Gray);
	}
	
	//RENDER OBJECTS FROM SCENEGRAPH
	int numRenderObjects = m_displayList.size();
	for (int i = 0; i < numRenderObjects; i++)
	{
		m_deviceResources->PIXBeginEvent(L"Draw model");
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x, m_displayList[i].m_scale.y, m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x, m_displayList[i].m_position.y, m_displayList[i].m_position.z };

		//convert degrees into radians for rotation matrix
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
															m_displayList[i].m_orientation.x *3.1415 / 180,
															m_displayList[i].m_orientation.z *3.1415 / 180);
       
		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);       
		m_displayList[i].m_model->Draw(context, *m_states, local, m_cam.GetViewMatrix(), m_cam.GetProjectionMatrix(), false);	//last variable in draw,  make TRUE for wireframe       

		m_deviceResources->PIXEndEvent();
	}

    //Renders object to identify selected objects
    for (int i = 0; i < m_selectedID->size(); i++) {
        m_deviceResources->PIXBeginEvent(L"Draw highlight");

        DisplayObject object = m_displayList[m_selectedID->at(i)];
        const XMVECTORF32 scale = {object.m_scale.x/2.f, object.m_scale.y / 2.f, object.m_scale.z / 2.f };
        const XMVECTORF32 translate = { object.m_position.x, object.m_position.y + object.m_model->meshes[0]->boundingBox.Center.x * object.m_scale.y * 2 + 1 * object.m_scale.y + 1, object.m_position.z };
        XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(object.m_orientation.y * 3.1415 / 180,
            object.m_orientation.x-180 * 3.1415 / 180,
            object.m_orientation.z * 3.1415 / 180);


        XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);
        object.m_model->Draw(context, *m_states, local, m_cam.GetViewMatrix(), m_cam.GetProjectionMatrix(), true);	//last variable in draw,  make TRUE for wireframe       
        m_deviceResources->PIXEndEvent();
    }


    m_deviceResources->PIXEndEvent();

	//RENDER TERRAIN
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault(),0);
	context->RSSetState(m_states->CullNone());
//	context->RSSetState(m_states->Wireframe());		//uncomment for wireframe

	//Render the batch,  This is handled in the Display chunk becuase it has the potential to get complex
	m_displayChunk.RenderBatch(m_deviceResources);

    //Checks if there is a selected object
    if (m_selectedID->size() > 0) {
        m_deviceResources->PIXBeginEvent(L"Draw Widget");
        //Sets up so that the meshes that spawn will not use the depth
        context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
        context->OMSetDepthStencilState(m_states->DepthNone(), 0);
        context->RSSetState(m_states->CullCounterClockwise());
        //Loops through all the widgers
        for (int i = 0; i < m_widgetList.size(); i++) {

            DisplayObject object = m_widgetList[i];
            const XMVECTORF32 scale = { object.m_scale.x,object.m_scale.y,object.m_scale.z };
            const XMVECTORF32 translate = { object.m_position.x, object.m_position.y, object.m_position.z };


            XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(object.m_orientation.y * 3.1415 / 180,
                object.m_orientation.x * 3.1415 / 180,
                object.m_orientation.z * 3.1415 / 180);


            XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

            object.m_model->meshes[0]->Draw(context, local, m_cam.GetViewMatrix(), m_cam.GetProjectionMatrix());	//last variable in draw,  make TRUE for wireframe       

        }
        m_deviceResources->PIXEndEvent();
    }

    //CAMERA POSITION ON HUD
    m_sprites->Begin();
    WCHAR   Buffer[256];
    std::wstring var = L"Mouse X: " + std::to_wstring(m_InputCommands.mouseLeftButton) + L"Mouse Y: " + std::to_wstring(m_InputCommands.mouseY);
    m_font->DrawString(m_sprites.get(), var.c_str(), XMFLOAT2(100, 10), Colors::Yellow);
    m_sprites->End();

    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetBackBufferRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}

void XM_CALLCONV Game::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
    m_deviceResources->PIXBeginEvent(L"Draw grid");

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthNone(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);

    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    xdivs = std::max<size_t>(1, xdivs);
    ydivs = std::max<size_t>(1, ydivs);

    for (size_t i = 0; i <= xdivs; ++i)
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    for (size_t i = 0; i <= ydivs; i++)
    {
        float fPercent = float(i) / float(ydivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    m_batch->End();

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
#ifdef DXTK_AUDIO
    m_audEngine->Suspend();
#endif
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

#ifdef DXTK_AUDIO
    m_audEngine->Resume();
#endif
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

}

void Game::BuildDisplayList(std::vector<SceneObject> * SceneGraph)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

    //Releases the m_texture_diffuse from memory
    for (int i = 0; i < m_displayList.size();i++) {
        m_displayList[i].m_texture_diffuse->Release();
        m_displayList[i].m_texture_diffuse = NULL;
    }

	if (!m_displayList.empty())		//is the vector empty
	{
		m_displayList.clear();		//if not, empty it
	}

	//for every item in the scenegraph
	int numObjects = SceneGraph->size();
	for (int i = 0; i < numObjects; i++)
	{
		
		//create a temp display object that we will populate then append to the display list.
		DisplayObject newDisplayObject;
		
		//load model
		//std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);							//convect string to Wchar
        std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);
        
        int c = modelwstr.find(StringToWCHART(".cmo"), 0);
        int s = modelwstr.find(StringToWCHART(".sdkmesh"), 0);

        if (c > -1) {
            newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)
        }
        else if(s > -1) {
            newDisplayObject.m_model = Model::CreateFromSDKMESH(device, modelwstr.c_str(), *m_fxFactory, true);
        }
      //Load Texture
		std::wstring texturewstr = StringToWCHART(SceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs = NULL;

        rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
        
		
		//if texture fails.  load error default
		if (rs)
		{
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
		}

		//apply new texture to models effect
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		{	
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);			
			}
		});
        newDisplayObject.m_ID = SceneGraph->at(i).ID;

		//set position
		newDisplayObject.m_position.x = SceneGraph->at(i).posX;
		newDisplayObject.m_position.y = SceneGraph->at(i).posY;
		newDisplayObject.m_position.z = SceneGraph->at(i).posZ;
		
		//setorientation
		newDisplayObject.m_orientation.x = SceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = SceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = SceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = SceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = SceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = SceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render		= SceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe	= SceneGraph->at(i).editor_wireframe;

		m_displayList.push_back(newDisplayObject);
        
        
	}
	
    //Releases the m_texture_diffuse from memory
    for (int i = 0; i < m_widgetList.size(); i++) {
        m_widgetList[i].m_texture_diffuse->Release();
        m_widgetList[i].m_texture_diffuse = NULL;
    }

    if (!m_displayList.empty())		//is the vector empty
    {
        m_widgetList.clear();		//if not, empty it
    }

    numObjects = 3;
    //Below is used to create display objects for the widget
    for (int i = 0; i < numObjects; i++)
    {
        //create a temp display object that we will populate then append to the display list.
        DisplayObject newDisplayObject;

        //load model
        //std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);							//convect string to Wchar
        std::wstring modelwstr = StringToWCHART("database/data/arrowX.cmo");
        newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

        //Load Texture
        std::wstring texturewstr = StringToWCHART("database/data/placeholder.dds");								//convect string to Wchar
        HRESULT rs = NULL;

        rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

        //if texture fails.  load error default
        if (rs)
        {
            CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
        }

        //apply new texture to models effect
        newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
        {
            auto lights = dynamic_cast<BasicEffect*>(effect);
            if (lights)
            {
                lights->SetTexture(newDisplayObject.m_texture_diffuse);
            }
        });

        newDisplayObject.m_ID = -1;

        //set position
        newDisplayObject.m_position.x = 0;
        newDisplayObject.m_position.y = 0;
        newDisplayObject.m_position.z = 0;

        //setorientation
        newDisplayObject.m_orientation.x = 0;
        newDisplayObject.m_orientation.y = 0;
        newDisplayObject.m_orientation.z = 0;
       
        //set scale
        newDisplayObject.m_scale.x = 0.75f;
        newDisplayObject.m_scale.y = 0.1f;
        newDisplayObject.m_scale.z = 0.1f;

        //set wireframe / render flags
        newDisplayObject.m_render = true;
        newDisplayObject.m_wireframe = false;

        m_widgetList.push_back(newDisplayObject);
    }
}

void Game::BuildDisplayChunk(ChunkObject * SceneChunk)
{
	//populate our local DISPLAYCHUNK with all the chunk info we need from the object stored in toolmain
	//which, to be honest, is almost all of it. Its mostly rendering related info so...
	m_displayChunk.PopulateChunkData(SceneChunk);		//migrate chunk data
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_terrainEffect->SetProjection(m_cam.GetProjectionMatrix());
	m_displayChunk.InitialiseBatch();
}

void Game::SaveDisplayChunk(ChunkObject * SceneChunk)
{
	m_displayChunk.SaveHeightMap();			//save heightmap to file.
}

#ifdef DXTK_AUDIO
void Game::NewAudioDevice()
{
    if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
}
#endif


#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"database/data/"); //fx Factory will look in the database directory
	m_fxFactory->SetSharing(false);	//we must set this to false otherwise it will share effects based on the initial tex loaded (When the model loads) rather than what we will change them to.

    m_sprites = std::make_unique<SpriteBatch>(context);

    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

    m_batchEffect = std::make_unique<BasicEffect>(device);
    m_batchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        DX::ThrowIfFailed(
            device->CreateInputLayout(VertexPositionColor::InputElements,
                VertexPositionColor::InputElementCount,
                shaderByteCode, byteCodeLength,
                m_batchInputLayout.ReleaseAndGetAddressOf())
        );
    }

    m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

//    m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

    // SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
    m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);
	

    // Load textures
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
    );

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
    );

}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{


    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    m_windowSize = size;
    m_cam.SetUpProjectionMatrix(aspectRatio);

    m_batchEffect->SetProjection(m_cam.GetProjectionMatrix());
	
}

//Handles mouse clicking in world
int Game::MousePicking()
{
    float rayDistance = INFINITY;
    float closestDistance = INFINITY;
    DisplayObject* currentDisplaySelected = nullptr;

    //Gets the mouse position in screen and forward
    Vector3 mouseScreenPos = Vector3(m_InputCommands.mouseX, m_InputCommands.mouseY, 0);
    Vector3 mouseForward = Vector3(m_InputCommands.mouseX, m_InputCommands.mouseY, 1.f);
    int id = 0;

    //Checks to see if the mouse collided with a widget
    for (int i = 0; i < m_widgetList.size(); i++) {
        rayDistance = INFINITY;
        DisplayObject currentobject = m_widgetList.at(i);
        //Objects tranlation data
        XMVECTOR  objectScale = { currentobject.m_scale.x,		currentobject.m_scale.y,		currentobject.m_scale.z };
        XMVECTOR  objectTranslation = { currentobject.m_position.x,	currentobject.m_position.y,	currentobject.m_position.z };
        XMVECTOR  objectRotation = Quaternion::CreateFromYawPitchRoll(currentobject.m_orientation.y * 3.1415 / 180, currentobject.m_orientation.x * 3.1415 / 180, currentobject.m_orientation.z * 3.1415 / 180);

        //Get World Matrix of Current Object
        XMMATRIX localWorldMatrix = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, objectScale, g_XMZero, objectRotation, objectTranslation);

        //Unprojects the mouse into the world
        Vector3  nearPlane = XMVector3Unproject(mouseScreenPos, 0, 0, m_windowSize.right, m_windowSize.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_cam.GetProjectionMatrix(), m_cam.GetViewMatrix(), localWorldMatrix);
        Vector3  farPlane = XMVector3Unproject(mouseForward, 0, 0, m_windowSize.right, m_windowSize.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_cam.GetProjectionMatrix(), m_cam.GetViewMatrix(), localWorldMatrix);

        Vector3  pickingVector = (farPlane - nearPlane);
        pickingVector = XMVector3Normalize(pickingVector);
        Ray ray = Ray(nearPlane, pickingVector);

        //Ray trace intersection
        if (ray.Intersects(currentobject.m_model->meshes[0]->boundingBox, rayDistance)) {
            m_widgetClicked = true;
            testPosition = nearPlane + pickingVector * rayDistance;
            m_currentWidgetSelected = i;
            return -2;
        }
        else {
            m_currentWidgetSelected = -1;
        }
    }

    //Checks to see if the mouse clicked an object in the world
    for (int i = 0; i < m_displayList.size(); i++) {
      
        //Objects tranlation data
        XMVECTOR objectScale = { m_displayList[i].m_scale.x,		m_displayList[i].m_scale.y,		m_displayList[i].m_scale.z };
        XMVECTOR objectTranslation = { m_displayList[i].m_position.x,	m_displayList[i].m_position.y,	m_displayList[i].m_position.z };
        XMVECTOR objectRotation = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y * 3.1415 / 180, m_displayList[i].m_orientation.x * 3.1415 / 180, m_displayList[i].m_orientation.z * 3.1415 / 180);

        //Get World Matrix of Current Object
        XMMATRIX localWorldMatrix = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, objectScale, g_XMZero, objectRotation, objectTranslation);

        //Unprojects the mouse into the world
        XMVECTOR nearPlane = XMVector3Unproject(mouseScreenPos, 0, 0, m_windowSize.right, m_windowSize.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_cam.GetProjectionMatrix(), m_cam.GetViewMatrix(), localWorldMatrix);
        XMVECTOR farPlane = XMVector3Unproject(mouseForward, 0, 0, m_windowSize.right, m_windowSize.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_cam.GetProjectionMatrix(), m_cam.GetViewMatrix(), localWorldMatrix);

        XMVECTOR pickingVector = (farPlane - nearPlane);
        pickingVector = XMVector3Normalize(pickingVector);
        Ray ray = Ray(nearPlane, pickingVector);

        //Below handles ray intersection and checks for the closest intersection
        for (int j = 0; j < m_displayList[i].m_model->meshes.size(); j++) {
            //Ray trace intersection
            if (ray.Intersects(m_displayList[i].m_model->meshes[j]->boundingBox, rayDistance)) {
                //Checks the current ray trace distance with the closest distance
                if (rayDistance < closestDistance) {
                    closestDistance = Vector3::Distance(m_displayList[i].m_position, nearPlane);
                    currentDisplaySelected = &m_displayList[i];
                    id = i;
                }
            }
        }
    }
    //Temporary fill in to show that the object is selected
    if (currentDisplaySelected) {
        currentDisplaySelected->m_scale = Vector3(5, 5, 5);
    }
    if (!currentDisplaySelected) return -1;

    return id;
}

void Game::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_batch.reset();
    m_batchEffect.reset();
    m_font.reset();
    m_shape.reset();
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

std::wstring StringToWCHART(std::string s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
