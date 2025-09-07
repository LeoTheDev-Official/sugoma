#include "app_module.h"
#include "imgui.h"
#include "window.h"
/*
#include "utility/guid.h"
#include "debug/logger.h"

#include "renderer/renderer_2d.h"
#include "assets/types/texture_asset.h"
#include "textures/texture2D.h"
#include "textures/texture_cube.h";
#include "mesh/mesh.h"

#include "engine/engine.h"
#include "graphics_module.h"
#include "window.h"

#include "systems/time.h"
#include "systems/input_system.h"

#include "assets/asset_pack.h"
AppModule::AppModule() : Module("AppModule") {  }

AppModule::~AppModule() { }

using namespace sugoma::graphics;
using namespace sugoma;
Renderer2D* renderer;
Camera cam;

GraphicsModule* gmod;

Transform t{};

AssetPack pack("data.saf");

AssetRef<Texture2D> textures[] =
{
	AssetRef<Texture2D>("F227559E2428D05D39F0EF9C4C915E04"),
	AssetRef<Texture2D>("976102F609D29FF2A40C87B50C9490A2"),
	AssetRef<Texture2D>("66D20686FAAEA8913939A24F7AFEE4DD"),
	AssetRef<Texture2D>("A8C5F0ED70D8AEC762CC8B56C3069C7E"),
	AssetRef<Texture2D>("B57A6130D188DCFEE66F7A99F9D14409"),
	AssetRef<Texture2D>("50DC0A3A1594C5DDC899E2AFF5B6A409")
};
uint32_t tex_index = 0;
AssetRef<Texture2D> active_texture;

void AppModule::OnAttach()
{
	gmod = reinterpret_cast<GraphicsModule*>(Context()->QueryModule("GraphicsModule"));
	RendererSpecification spec{};
	spec.magFilter = LINEAR;
	spec.minFilter = LINEAR;
	spec.resolution = { 1920, 1080 };
	renderer = new Renderer2D(spec);

	TextureMetadata meta{};
	meta.textureFormat = RGB8;

	//Buffer b = sugoma::AssetImpl<Texture>::PackAssetFile("data/a2.jpg", &meta);
	//testTex = sugoma::AssetImpl<Texture>::UnpackAsset(b);
	cam.MakeOrthographic(10, 16.0f / 9.0f);
	t.scale = glm::vec3(3, 3 * 1.08f, 1);

	AssetPack pack("test.saf");
	pack.Load();
	AssetRef<Texture2D> asset = pack.FindAsset("test_hdri");
	if (asset) {
		active_texture = asset;

		TextureCreateInfo tinfo{};
		tinfo.height = asset.get()->Height() / 2;
		tinfo.width = tinfo.height;

		Ref<TextureCube> cube = TextureCube::FromHDRI(asset, tinfo);
	}
	AssetRef<Mesh> mesh = pack.FindAsset("test_mesh");
	Ref<Mesh> m = mesh.get();
	sugoma_log("VertexCount : " << m->VertexCount() << " | IndexCount : " << m->IndexCount() << " | Submeshes : " << m->SubmeshCount());

}

void AppModule::OnDetach()
{
	pack.Unload();
}

bool AppModule::Validate()
{
	return true;
}
void AppModule::Tick()
{
	renderer->BeginScene(cam);

	if (active_texture) {
		Ref<Texture2D> tr = active_texture;
		t.scale = glm::vec3(3, 3 * tr->Height() / (float)tr->Width(), 1);
	}

	renderer->DrawQuad(t.matrix(), active_texture, glm::vec4(1));

	renderer->EndScene();

	gmod->GetWindow()->Data(renderer->GetFramebuffer(), 0, LINEAR);

	if (InputSystem::GetKeyDown(KeyCode::L)) { pack.Load(); }
	if (InputSystem::GetKeyDown(KeyCode::U)) { pack.Unload(); }
	if (InputSystem::GetKeyDown(KeyCode::R)) { pack.Unload(); pack.Load(); }

	if (InputSystem::GetKeyDown(KeyCode::Space))
		active_texture = textures[++tex_index % (sizeof(textures) / sizeof(textures[0]))];
}
*/

#include "renderer/drags/drags.h"
#include "debug/logger.h"
#include "assets/asset_pack.h"

#include "systems/time.h"
#include "systems/input_system.h"

#include "debug/fps_camera_controller.h"

using namespace sugoma;
AppModule::AppModule() : Module("AppModule") {}

AppModule::~AppModule()
{

}
DragsRenderer* dragsRenderer = nullptr;
Camera camera{};
Transform transform{};
DragsMaterial material{};

Ref<Texture2D> test_texture;

AssetPack pack("data.saf");

AssetRef<Texture2D> textures[] =
{
	AssetRef<Texture2D>("F227559E2428D05D39F0EF9C4C915E04"),
	AssetRef<Texture2D>("976102F609D29FF2A40C87B50C9490A2"),
	AssetRef<Texture2D>("66D20686FAAEA8913939A24F7AFEE4DD"),
	AssetRef<Texture2D>("A8C5F0ED70D8AEC762CC8B56C3069C7E"),
	AssetRef<Texture2D>("B57A6130D188DCFEE66F7A99F9D14409"),
	AssetRef<Texture2D>("50DC0A3A1594C5DDC899E2AFF5B6A409")
};
AssetPack test_pack("test.saf");
AssetRef<Mesh> teapot_mesh("EC6CF0139CC33485DD6D5BADE2A0C2BA");
AssetRef<TextureCube> skybox("A44EFEF847DFFFE576C6595FE1272ACC");


Transform transforms[sizeof(textures) / sizeof(textures[0])] = {};
DragsMaterial materials[sizeof(textures) / sizeof(textures[0])] = {};

glm::vec3 sunColor = glm::vec3(1);
float sunIntensity = 1.0f;
glm::vec3 sunEulerAngles = { -45, 180, 0 };
AssetRef<Mesh> sphere("25E85DFA48BB5740FB1F45216918D69C");
void AppModule::OnAttach()
{
	RendererSpecification spec{};
	spec.resolution = { 1920, 1080 };
	spec.samples = 1;
	dragsRenderer = new DragsRenderer(spec);

	//camera.MakeOrthographic(16.0f / 9.0f, 5, 0);
	camera.MakePerspective(16.0f / 9.0f, 60);

	TextureCreateInfo texInfo{};
	texInfo.width = 2;
	texInfo.height = 2;
	texInfo.magFilter = TextureFilter::NEAREST;
	texInfo.minFilter = TextureFilter::NEAREST;
	texInfo.format = TextureFormat::RGBA8;
	glm::vec4 data[] = { {1, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 1}, {1, 0, 1, 1} };
	test_texture = Texture2D::Create(texInfo, data, TextureFormat::RGBA, TextureFormatComponent::FLOAT);
	//material.albedoMap = test_texture;

	transform.position.z = -4;
	//camera.transform.position.y = 2;
	//camera.transform.eulerAngles.x = -30.0f;

	test_pack.Load();
	transform.scale = glm::vec3(10);
	teapot_mesh.get()->RecalculateTangents();
	teapot_mesh.get()->Update();

	sphere.get()->RecalculateTangents();
	sphere.get()->Update();

	material.albedoMap = AssetRef<Texture2D>("BF214A83DEC4E3ADBD59D4BCBFF2C47D");
	material.normalMap = AssetRef<Texture2D>("36CCC50F95C5D2BC2933E8A32F1CCABF");
	material.metallicMap = AssetRef<Texture2D>("A4C31150BBD282ECD59DC007CD875ACE");
	material.roughnessMap = AssetRef<Texture2D>("B1310B6687CA690F4EE343895E67AE22");

	camera.clearMode = ClearMode::Skybox;
	camera.skybox = skybox;

	pack.Load();
	uint32_t count = sizeof(textures) / sizeof(textures[0]);
	float width = count * (3 + 0.1f);
	for (uint32_t i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i) 
	{
		DragsMaterial& mat = materials[i];
		Transform& t = transforms[i];
		mat.albedoMap = textures[i];
		t.scale = glm::vec3( 3, 3 * textures[i].get()->Height() / (float)textures[i].get()->Width(), 1 );
		t.position = { -width / 2 + i * (3 + 0.1f), 3, 5 };
		t.eulerAngles = { 0, 180, 0 };
	}
	DragsEnviorment env
	{
		.sunDirection = Transform{.eulerAngles = sunEulerAngles}.forward(),
		.sunColor = sunColor * sunIntensity
	};
	dragsRenderer->SetEnviorment(env);
}

uint32_t at_index = 0;
bool loaded = false;

void AppModule::OnDetach()
{

}

bool AppModule::Validate()
{
	return true;
}
float tim = 0;
float swap_time = 2.0f;

int32_t submesh_index = -1;
uint32_t attachment = 0;

FPSCameraController controller{};
DragsMaterial plane_mat{ .albedo = glm::vec4(0.3f) };


DragsMaterial sphere_mat{};
Transform sphere_tr{ .position = glm::vec3(0, 2, 0) };

void DrawDebugPanels() 
{
	for (uint32_t i = 0; i < (sizeof(textures) / sizeof(textures[0])); ++i)
	{
		dragsRenderer->DrawMesh(Primitives::Quad(), transforms[i].matrix(), materials[i], DragsRenderStep::Opaque, -1);
		Transform& t = transforms[i];
		t.position.y += glm::sin(Time::UnscaledTime() + t.position.x * 0.5f) * Time::DeltaTime() * 0.3f;
	}
}

void AppModule::Tick()
{
	controller.update(Time::DeltaTime());
	camera.transform = controller.transform;
	Mesh* m = sphere.get().get();
	dragsRenderer->Begin(camera);
	dragsRenderer->DrawMesh(teapot_mesh, transform.matrix(), material, DragsRenderStep::Opaque, submesh_index);
	dragsRenderer->DrawMesh(Primitives::Plane(), glm::scale(glm::mat4(1), {30, 30, 30}), plane_mat, DragsRenderStep::Opaque, -1);
	dragsRenderer->DrawMesh(sphere, sphere_tr.matrix(), sphere_mat, DragsRenderStep::Opaque);

	//DrawDebugPanels();

	dragsRenderer->End();
	graphics::GraphicsContext()->Data(dragsRenderer->OutputFramebuffer().get(), attachment, LINEAR);

	transform.eulerAngles += Time::DeltaTime() * 90.0f * glm::vec3(0, 1, 0);

	if (ImGui::Begin("Drags Renderer"))
	{
		ImGui::InputInt("Attachment", (int*)&attachment);
		if(ImGui::Button("Invalidate")) dragsRenderer->Invalidate();
		ImGui::End();
	}
	if (ImGui::Begin("Enviorment")) 
	{
		bool changed = ImGui::DragFloat3("Sun Rotation", &sunEulerAngles.x, 0.01f);
		changed |= ImGui::ColorEdit4("Sun Color", &sunColor.x);
		changed |= ImGui::DragFloat("Intensity", &sunIntensity);
		if (changed) 
		{
			DragsEnviorment env
			{
				.sunDirection = Transform{.eulerAngles = sunEulerAngles}.forward(),
				.sunColor = sunColor * sunIntensity
			};
			dragsRenderer->SetEnviorment(env);
		}
		ImGui::Spacing();
		ImGui::ColorEdit4("Albedo", &sphere_mat.albedo.x);
		ImGui::DragFloat("Metallic", &sphere_mat.metallic, 0.01f);
		ImGui::DragFloat("Roughness", &sphere_mat.roughness, 0.01f);


		ImGui::End();
	}

	if(InputSystem::GetKeyDown(KeyCode::L))
	{
		if (!loaded)
		{
			pack.Load();
			loaded = true;
			transform.scale = glm::vec3(3, 3 * textures[at_index].get()->Height() / (float)textures[at_index].get()->Width(), 1);
			material.albedoMap = textures[at_index];
		}
		else
		{
			pack.Unload();
			loaded = false;
		}
	}
	if (!loaded) return;
	if (InputSystem::GetKeyDown(KeyCode::Space)) 
	{
		at_index = (at_index + 1) % (sizeof(textures) / sizeof(textures[0]));
		transform.scale = glm::vec3(3, 3 * textures[at_index].get()->Height() / (float)textures[at_index].get()->Width(), 1);
		material.albedoMap = textures[at_index];
	}
	tim += Time::DeltaTime();
	if (tim >= swap_time) 
	{
		tim = 0;
		at_index = (at_index + 1) % (sizeof(textures) / sizeof(textures[0]));
		transform.scale = glm::vec3(3, 3 * textures[at_index].get()->Height() / (float)textures[at_index].get()->Width(), 1);
		material.albedoMap = textures[at_index];
	}

}

