#ifndef SCENE_EDITOR_HPP
#define SCENE_EDITOR_HPP

#include "Scene/Scene.hpp"
#include "World/EntityManager.hpp"
#include "World/World.hpp"
#include "Rendering/RenderingManager.hpp"
#include "Rendering/ObjLoader.hpp"
#include "Gui/Gui.hpp"

#include "Ship/ShipComponent.hpp"
#include "Ship/ShipCell.hpp"

#include "Components/ShipFlightControl.hpp"

class Scene_Editor : public Scene
{
public:
	Gui* m_Gui;

	ShipCell* hullCell = nullptr;
	ShipCell* bridgeCell = nullptr;
	ShipCell* cargoCell = nullptr;
	ShipCell* bridge1Cell = nullptr;

	ShipCell* cursorCell = nullptr;

	//Temp int Mouse
	int intMouse = 0;

	int buildLevel = 0;

	Scene_Editor(SDL_GameController* controllerToUse)
	{
		m_Gui = new Gui();

		controller = controllerToUse;

		//Load Skybox
		skybox = new Model();
		skybox->mesh = loadMeshFromFile("res/Skybox.obj");
		skybox->texture = "res/Skybox.png";
		skybox->shader = new ShaderProgram("res/Textured.vs", "res/Textured.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_TexCoord" } });

		MaterialShader = new ShaderProgram("res/Material.vs", "res/Material.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

		directionalLight = new DirectionalLight(vector3F(1.0f, -1.0f, -1.0f), vector3F(1.0f, 1.0f, 1.0f), 0.6f);
		DirectionalShader = new ShaderProgram("res/Material.vs", "res/foward-directional.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });
		pointLight = new PointLight(vector3D(0.0, 10.0, 0.0), 20.0f, vector3F(1.0), 0.4f, vector3F(0.0f, 0.0f, 0.02f));
		PointShader = new ShaderProgram("res/Material.vs", "res/foward-point.fs", { { 0, "in_Position" }, { 1, "in_Normal" }, { 2, "in_Material" } });

		shipModelOutside = new Model();
		shipModelOutside->localOffset = Transform();
		shipModelOutside->shader = MaterialShader;
		shipModelOutside->mesh = nullptr;
		shipModelOutside->texture = "";

		m_cursorModel = new Model();
		m_cursorModel->localOffset = Transform();
		m_cursorModel->shader = MaterialShader;
		m_cursorModel->mesh = loadMaterialMeshFromFile("res/", "Cursor.obj");
		m_cursorModel->texture = "";

		shipComponent = new ShipComponent();
		this->shipChanged = true;

		if (true)
		{
			vector<Node> nodes;
			for (int i = 0; i < 6; i++)
			{
				nodes.push_back(Node(vector3S(0), i));
			}
			hullCell = new ShipCell(nullptr, loadMaterialMeshFromFile("res/ShipParts/", "Hull_Cursor.obj"), 200, nodes, AABB());
		}
		

		if (true)
		{
			vector<Node> nodes = 
			{
				Node(vector3S(0, 1, 0), UP),
				Node(vector3S(0, -1, 0), DOWN),
				Node(vector3S(0, 0, -2), BACKWARD),
				Node(vector3S(1, 0, 0), LEFT),
				Node(vector3S(-1, 0, 0), RIGHT),
			};

			bridgeCell = new ShipCell(loadMaterialMeshFromFile("res/ShipParts/", "Bridge.obj"), loadMaterialMeshFromFile("res/ShipParts/", "Bridge_Cursor.obj"), 3000, nodes, AABB(vector3D(-1.5, -1.5, -2.5), vector3D(1.5, 1.5, 1.5)));
		}

		if (true)
		{

		}


		if (true)
		{
			vector<Node> nodes =
			{
				Node(vector3S(0, 0, 0), BACKWARD),
				Node(vector3S(1, 0, 1), LEFT),
				Node(vector3S(-1, 0, 1), RIGHT),
			};

			bridge1Cell = new ShipCell(loadMaterialMeshFromFile("res/ShipParts/", "Bridge1.obj"), loadMaterialMeshFromFile("res/ShipParts/", "Bridge1_Cursor.obj"), 3000, nodes, AABB(vector3D(-1.5, -0.95, -0.5), vector3D(1.5, 0.95, 2.5)));
		}

		cursorCell = bridge1Cell;
	};

	virtual ~Scene_Editor()
	{
		delete shipComponent;
		delete shipModelInside;
		delete shipModelOutside;

		delete MaterialShader;
		delete PointShader;
		delete DirectionalShader;

		delete skybox;
		delete m_cursorModel;
	};

	virtual void update(double deltaTime)
	{
		const double stepTime = 0.3;

		//UP cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, 0, 1);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, 0, 1);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Down cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, 0, -1);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, 0, -1);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Left cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(1, 0, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(1, 0, 0);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Right cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(-1, 0, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(-1, 0, 0);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Up cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, 1, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, 1, 0);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Down cursor Movement
		if (true)
		{
			static double time = 0.0;
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
			{
				//First movement happens when the button is first pushed
				if (time == -1.0)
				{
					m_cursorPos += vector3D(0, -1, 0);
					time = 0.0;
				}

				time += deltaTime;
				if (time >= stepTime)
				{
					m_cursorPos += vector3D(0, -1, 0);
					time = 0.0;
				}
			}
			else
			{
				time = -1.0;
			}
		}

		//Camera Update
		int deadzone = 4000;

		int pitchAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);

		if (pitchAxis > deadzone || pitchAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)pitchAxis) / 32767.0;
			double angle = amount * deltaTime * rotSpeed;

			quaternionD pitchQuat = glm::normalize(glm::angleAxis(-angle, cameraRot * vector3D(-1.0, 0.0, 0.0)));

			cameraRot = pitchQuat * cameraRot;
		}

		int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

		if (yawAxis > deadzone || yawAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)yawAxis) / 32767.0;
			double angle = amount * deltaTime * rotSpeed;

			quaternionD yawQuat = glm::normalize(glm::angleAxis(-angle, vector3D(0.0, 1.0, 0.0)));

			cameraRot = yawQuat * cameraRot;
		}

		int rightTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		int leftTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

		if (rightTrigger > deadzone || leftTrigger > deadzone)
		{
			int inputValue = -rightTrigger + leftTrigger;
			//Get between -1 and 1
			double amount = ((double)inputValue) / 32767.0;

			cameraDistance += amount * deltaTime * zoomSpeed;

			if (cameraDistance < minDistance)
			{
				cameraDistance = minDistance;
			}

			if (cameraDistance > maxDistance)
			{
				cameraDistance = maxDistance;
			}
		}

		//MOUSE CONTROLLS
		static int last_MousePos[] = { 0, 0 };
		int current_MousePos[] = { 0, 0 };
		if (SDL_GetMouseState(&current_MousePos[0], &current_MousePos[1]) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			int delta_Mouse[] = { last_MousePos[0] - current_MousePos[0], last_MousePos[1] - current_MousePos[1] };

			if (true)
			{
				//Get between -1 and 1
				double amount = delta_Mouse[1];
				double angle = amount * deltaTime * 0.1;

				quaternionD pitchQuat = glm::normalize(glm::angleAxis(angle, cameraRot * vector3D(-1.0f, 0.0f, 0.0f)));

				cameraRot = pitchQuat * cameraRot;
			}

			if (true)
			{
				//Get between -1 and 1
				double amount = delta_Mouse[0];
				double angle = amount * deltaTime  * 0.1;

				quaternionD yawQuat = glm::normalize(glm::angleAxis(angle, vector3D(0.0f, 1.0f, 0.0f)));

				cameraRot = yawQuat * cameraRot;
			}

			if (true)
			{
				double amount = -intMouse;

				cameraDistance += amount * deltaTime * zoomSpeed * 2.0;

				if (cameraDistance < minDistance)
				{
					cameraDistance = minDistance;
				}

				if (cameraDistance > maxDistance)
				{
					cameraDistance = maxDistance;
				}
			}
		}
		else
		{
			buildLevel += intMouse;
		}
		last_MousePos[0] = current_MousePos[0];
		last_MousePos[1] = current_MousePos[1];

		intMouse = 0;

		Camera* camera = &UVSG::getInstance()->renderingManager->camera;
		vector3D camPos = (cameraRot * vector3D(0, 0, -1.0f)) * cameraDistance;
		camPos += camOrigin;
		camera->setCameraTransform(camPos, cameraRot);

		if (true)
		{
			static int lastState = 0;
			int currentState = SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT);
			if (currentState && !lastState)
			{
				vector3S pos = (vector3S)m_cursorPos;
				if (shipComponent->hasCellAtPos(pos))
				{
					shipComponent->removeCell(pos);
					shipChanged = true;
				}
				else
				{
					ShipCellData newCell = ShipCellData(cursorCell, (vector3S)m_cursorPos);
					if (shipComponent->canPlaceCell(newCell))
					{
						shipComponent->addCell(newCell);
						shipChanged = true;
					}
				}
			}
			lastState = currentState;
		}


		//EDITOR CONTROLS
		if (true)
		{
			static int lastState = 0;
			int currentState = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
			if (currentState && !lastState)
			{
				vector3S pos = (vector3S)m_cursorPos;
				if (shipComponent->hasCellAtPos(pos))
				{
					shipComponent->removeCell(pos);
					shipChanged = true;
				}
				else
				{
					ShipCellData newCell = ShipCellData(cursorCell, (vector3S)m_cursorPos);
					if (shipComponent->canPlaceCell(newCell))
					{
						shipComponent->addCell(newCell);
						shipChanged = true;
					}
				}
			}
			lastState = currentState;
		}

		if (true)
		{
			static int lastState = 0;
			int currentState = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
			if (currentState && !lastState)
			{
				cursorCell = bridgeCell;
			}
		}

		if (true)
		{
			static int lastState = 0;
			int currentState = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X);
			if (currentState && !lastState)
			{
				cursorCell = hullCell;
			}
		}

		if (true)
		{
			static int lastState = 0;
			int currentState = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y);
			if (currentState && !lastState)
			{
				cursorCell = cargoCell;
			}
		}


		if (shipChanged == true)
		{
			if (shipModelOutside->mesh != nullptr)
			{
				delete shipModelOutside->mesh;
			}

			shipModelOutside->mesh = shipComponent->genOutsideMesh();

			shipChanged = false;
		}

		if (shipComponent->canPlaceCell(ShipCellData(cursorCell, (vector3S)m_cursorPos)))
		{
			MaterialMesh* mesh = (MaterialMesh*)m_cursorModel->mesh;
			mesh->materials[0].diffuse_Color = vector3F(0.0f, 1.0f, 0.0f);
		}
		else
		{
			MaterialMesh* mesh = (MaterialMesh*)m_cursorModel->mesh;
			mesh->materials[0].diffuse_Color = vector3F(0.800000f, 0.254604f, 0.002949f);
		}

	};

	virtual void render(RenderingManager* manager)
	{
		manager->window->set3dRendering();

		Camera* camera = &manager->camera;

		int width, height;
		manager->window->getWindowSize(width, height);
		camera->setProjection(45.0f, 0.01f, 1000.0f, width, height);

		Transform transform = Transform();

		/******************************************************************************************/
		manager->window->clearBuffer();

		manager->renderModel(camera, skybox, Transform(camera->getPos()));

		//Clear depth buffer so any other object in front of far objects.
		glClear(GL_DEPTH_BUFFER_BIT);

		Model* model = new Model();
		model->shader = MaterialShader;
		for (auto it = shipComponent->m_shipCells.begin(); it != shipComponent->m_shipCells.end(); ++it)
		{
			model->mesh = it->second.getMesh();
			if (model->mesh != nullptr)
			{
				model->localOffset = Transform((vector3F)it->second.m_position * cubeSizeOutside);
				manager->renderModelLight(camera, model, Transform());
			}
		}
		delete model;

		if (shipModelOutside->mesh != nullptr)
		{
			manager->renderModelLight(camera, shipModelOutside, Transform());
		}

		//Cursor Draw
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (shipComponent->canPlaceCell(ShipCellData(cursorCell, m_cursorPos)))
		{
			Model* model = new Model();
			model->shader = MaterialShader;
			model->mesh = cursorCell->getCursorMesh();

			if (model->mesh != nullptr)
				manager->renderModelLight(camera, model, Transform((vector3D)m_cursorPos * (double)cubeSizeOutside), 0.5f);

			delete model;
		}

		glDisable(GL_DEPTH_TEST);
		manager->renderModelLight(camera, m_cursorModel, Transform((vector3D)m_cursorPos * (double)cubeSizeOutside));
		glEnable(GL_DEPTH_TEST);

		glDisable(GL_BLEND);

		//UI Rendering
		//Clear depth buffer so any other object in front of far objects.
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		manager->window->set2dRendering();

		//manager->texturePool.loadTexture("res/Textures/metalPanel_Cells.png");
		//manager->texturePool.bindTexture("res/Textures/metalPanel_Cells.png");
		//m_Gui->renderGui(width,height);

		glDisable(GL_BLEND);

		manager->window->updateBuffer();
	};

	float cubeSizeOutside = 3.0f;
	float cubeSizeInside = 2.6f;

	ShipComponent* shipComponent = nullptr;
	ShaderProgram* MaterialShader = nullptr;
	Model* shipModelOutside = nullptr;
	Model* shipModelInside = nullptr;
	Model* skybox = nullptr;
	Model* UIBox = nullptr;

	DirectionalLight* directionalLight = nullptr;
	ShaderProgram* DirectionalShader = nullptr;

	PointLight* pointLight = nullptr;
	ShaderProgram* PointShader = nullptr;

	bool shipChanged = false;
	SDL_GameController* controller;
	vector3D camOrigin = vector3D(0.0);

	double cameraDistance = 20.0;
	double maxDistance = 200.0;
	double minDistance = 0.0;

	double rotSpeed = 3.0;
	double zoomSpeed = 150.0;

	quaternionD cameraRot;

	Model* m_cursorModel = nullptr;
	vector3S m_cursorPos = vector3D(0.0); 

private:
	
};

#endif //SCENE_EDITOR_HPP