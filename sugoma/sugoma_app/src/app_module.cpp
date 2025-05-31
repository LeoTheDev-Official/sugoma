#include "app_module.h"

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
