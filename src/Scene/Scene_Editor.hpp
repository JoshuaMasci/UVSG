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

class Scene_Editor : public Scene
{
public:
	Gui* m_Gui;

	ShipCell* hullCell = nullptr;
	ShipCell* bridgeCell = nullptr;

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
			vector<vector3S> points = {vector3S(0)};
			for (int i = 0; i < 6; i++)
			{
				nodes.push_back(Node(vector3S(0), i));
			}
			hullCell = new ShipCell(nullptr, 200, nodes, points);
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

			vector<vector3S> points;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					for (int z = -2; z < 2; z++)
					{
						points.push_back(vector3S(x, y, z));
					}
				}
			}

			bridgeCell = new ShipCell(loadMaterialMeshFromFile("res/ShipParts/", "Bridge.obj"), 3000, nodes, points);
		}

		shipComponent->addCell(ShipCellData(bridgeCell, vector3S()));

		shipModelOutside->mesh = shipComponent->genOutsideMesh();
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
			double amount = ((double)pitchAxis) / 32767.0f;
			double angle = amount * deltaTime * rotSpeed;

			//Negitive angle because the joystick layout is backwards
			quaternionD pitchQuat = glm::normalize(glm::angleAxis(-angle, cameraRot * vector3D(-1.0f, 0.0f, 0.0f)));

			cameraRot = pitchQuat * cameraRot;
		}

		int yawAxis = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);

		if (yawAxis > deadzone || yawAxis < -deadzone)
		{
			//Get between -1 and 1
			double amount = ((double)yawAxis) / 32767.0f;
			double angle = amount * deltaTime * rotSpeed;

			quaternionD yawQuat = glm::normalize(glm::angleAxis(-angle, vector3D(0.0f, 1.0f, 0.0f)));

			cameraRot = yawQuat * cameraRot;
		}

		int rightTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		int leftTrigger = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);

		if (rightTrigger > deadzone || leftTrigger > deadzone)
		{
			int inputValue = -rightTrigger + leftTrigger;
			//Get between -1 and 1
			double amount = ((double)inputValue) / 32767.0f;

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
					shipComponent->addCell(ShipCellData(hullCell, pos));
					shipChanged = true;
				}
			}
			lastState = currentState;
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

		if (shipComponent->hasCellAtPos(m_cursorPos))
		{
			MaterialMesh* mesh = (MaterialMesh*)m_cursorModel->mesh;
			mesh->materials[0].diffuse_Color = vector3F(0.0f, 1.0f, 0.0f);
		}
		else
		{
			MaterialMesh* mesh = (MaterialMesh*)m_cursorModel->mesh;
			mesh->materials[0].diffuse_Color = vector3F(0.800000f, 0.254604f, 0.002949f);
		}

		//Load game world
		if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK))
		{
			return;

			printf("Loading Game World!!!\n");
			UVSG* instace = UVSG::getInstance();
			Scene_Game* game = new Scene_Game();
			instace->currentScene = game;
		}
	};

	virtual void render(RenderingManager* manager)
	{
		manager->window->set3dRendering();

		Camera* camera = &manager->camera;

		int width, height;
		manager->window->getWindowSize(width, height);
		camera->setProjection(45.0f, 0.01f, 1000.0f, width, height);

		vector3D camPos = (cameraRot * vector3D(0, 0, -1.0f)) * cameraDistance;
		camPos += camOrigin;

		manager->camera.setCameraTransform(camPos, cameraRot);

		Transform transform = Transform();

		/******************************************************************************************/
		manager->window->clearBuffer();

		renderModel(manager, camera, skybox, Transform(camera->getPos()));

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
				renderModelLight(manager, camera, model, Transform());
			}
		}
		delete model;

		if (shipModelOutside->mesh != nullptr)
		{
			renderModelLight(manager, camera, shipModelOutside, Transform());
		}

		//Cursor Draw
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		renderModel(manager, camera, m_cursorModel, Transform(m_cursorPos * (double) cubeSizeOutside));
		
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);


		//UI Rendering
		//Clear depth buffer so any other object in front of far objects.
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		manager->window->set2dRendering();

		manager->texturePool.loadTexture("res/Textures/metalPanel_blueCorner.png");
		manager->texturePool.bindTexture("res/Textures/metalPanel_blueCorner.png");
		m_Gui->renderGui(width,height);

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
	vector3D m_cursorPos = vector3D(0.0); 

private:
	void renderModel(RenderingManager* manager, Camera* camera, Model* model, Transform transform)
	{
		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();

		matrix4 modelMatrix = transform.getModleMatrix(camera->getPos());
		matrix3 normalMatrix = transform.getNormalMatrix();

		modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
		normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();

		model->shader->setActiveProgram();
		if (model->texture != "")
		{
			manager->texturePool.bindTexture(model->texture);
		}

		model->shader->setUniform("MVP", projectionMatrix * viewMatrix * modelMatrix);
		model->shader->setUniform("normalMatrix", normalMatrix);
		model->shader->setUniform("modelMatrix", modelMatrix);
		model->shader->setUniform("ambientLight", manager->ambientLight);

		model->mesh->draw(model->shader);

		model->shader->deactivateProgram();
	}

	void renderModelLight(RenderingManager* manager, Camera* camera, Model* model, Transform transform)
	{
		matrix4 projectionMatrix = camera->getProjectionMatrix();
		matrix4 viewMatrix = camera->getOriginViewMatrix();

		matrix4 modelMatrix = transform.getModleMatrix(camera->getPos());
		matrix3 normalMatrix = transform.getNormalMatrix();

		modelMatrix = modelMatrix * model->localOffset.getModleMatrix();
		normalMatrix = normalMatrix * model->localOffset.getNormalMatrix();
		matrix4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		model->shader->setActiveProgram();
		if (model->texture != "")
		{
			manager->texturePool.bindTexture(model->texture);
		}

		model->shader->setUniform("MVP", MVP);
		model->shader->setUniform("normalMatrix", normalMatrix);
		model->shader->setUniform("modelMatrix", modelMatrix);
		model->shader->setUniform("ambientLight", manager->ambientLight);

		model->mesh->draw(model->shader);

		model->shader->deactivateProgram();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(false);
		glDepthFunc(GL_EQUAL);

		DirectionalShader->setActiveProgram();
		DirectionalShader->setUniform("MVP", MVP);
		DirectionalShader->setUniform("normalMatrix", normalMatrix);
		DirectionalShader->setUniform("modelMatrix", modelMatrix);
		DirectionalShader->setUniform("directionalLight.color", directionalLight->getColor());
		DirectionalShader->setUniform("directionalLight.direction", directionalLight->getDirection());
		DirectionalShader->setUniform("directionalLight.intensity", directionalLight->getIntensity());
		model->mesh->draw(DirectionalShader);
		DirectionalShader->deactivateProgram();

		PointShader->setActiveProgram();
		PointShader->setUniform("MVP", MVP);
		PointShader->setUniform("normalMatrix", normalMatrix);
		PointShader->setUniform("modelMatrix", modelMatrix);
		PointShader->setUniform("pointLight.base.color", pointLight->getColor());
		PointShader->setUniform("pointLight.base.intensity", pointLight->getIntensity());
		PointShader->setUniform("pointLight.range", pointLight->m_range);
		PointShader->setUniform("pointLight.positionWorld", (vector3F)(pointLight->m_position - camera->getPos()));
		PointShader->setUniform("pointLight.atten.constant", pointLight->m_attenuation.x);
		PointShader->setUniform("pointLight.atten.linear", pointLight->m_attenuation.y);
		PointShader->setUniform("pointLight.atten.exponent", pointLight->m_attenuation.z);
		model->mesh->draw(PointShader);
		PointShader->deactivateProgram();

		glDepthFunc(GL_LESS);
		glDepthMask(true);
		glDisable(GL_BLEND);
	}
};

#endif //SCENE_EDITOR_HPP