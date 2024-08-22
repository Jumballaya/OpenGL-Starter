#include "Gui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <imgui/imgui.h>

Gui::Gui() : guiShader(Core::GL::Shader("shaders/imgui/vertex.glsl", "shaders/imgui/fragment.glsl")) {
	glCreateVertexArrays(1, &vao);

	glCreateBuffers(1, &bufferVertex);
	glNamedBufferStorage(bufferVertex, 128 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);

	glCreateBuffers(1, &bufferElements);
	glNamedBufferStorage(bufferElements, 256 * 1024, nullptr, GL_DYNAMIC_STORAGE_BIT);

	glVertexArrayElementBuffer(vao, bufferElements);
	glVertexArrayVertexBuffer(vao, 0, bufferVertex, 0, sizeof(ImDrawVert));

	glEnableVertexArrayAttrib(vao, 0);
	glEnableVertexArrayAttrib(vao, 1);
	glEnableVertexArrayAttrib(vao, 2);

	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, pos));
	glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, IM_OFFSETOF(ImDrawVert, uv));
	glVertexArrayAttribFormat(vao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, IM_OFFSETOF(ImDrawVert, col));

	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribBinding(vao, 1, 0);
	glVertexArrayAttribBinding(vao, 2, 0);

	glBindVertexArray(vao);

	guiShader.bind();

	glCreateBuffers(1, &perFrameDataBuffer);
	glNamedBufferStorage(perFrameDataBuffer, sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, perFrameDataBuffer);

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	// Build texture atlas
	ImFontConfig cfg = ImFontConfig();
	cfg.FontDataOwnedByAtlas = false;
	cfg.RasterizerMultiply = 1.5f;
	cfg.SizePixels = 20.0f;
	cfg.PixelSnapH = true;
	cfg.OversampleH = 4;
	cfg.OversampleV = 4;
	ImFont* Font = io.Fonts->AddFontFromFileTTF("fonts/OpenSans-Light.ttf", cfg.SizePixels, &cfg);

	unsigned char* pixels = nullptr;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	glCreateTextures(GL_TEXTURE_2D, 1, &textureFont);
	glTextureParameteri(textureFont, GL_TEXTURE_MAX_LEVEL, 0);
	glTextureParameteri(textureFont, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureFont, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureStorage2D(textureFont, 1, GL_RGBA8, width, height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTextureSubImage2D(textureFont, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTextures(0, 1, &textureFont);

	io.Fonts->TexID = (ImTextureID)(intptr_t)textureFont;
	io.FontDefault = Font;
	io.DisplayFramebufferScale = ImVec2(1, 1);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
}

Gui::~Gui() {
	ImGui::DestroyContext();
}

void Gui::Draw(int width, int height) {
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	guiShader.bind();
	glBindVertexArray(vao);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)width, (float)height);
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	ImGui::Render();

	const ImDrawData* draw_data = ImGui::GetDrawData();

	const float L = draw_data->DisplayPos.x;
	const float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
	const float T = draw_data->DisplayPos.y;
	const float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
	const glm::mat4 orthoProjection = glm::ortho(L, R, B, T);

	glNamedBufferSubData(perFrameDataBuffer, 0, sizeof(glm::mat4), glm::value_ptr(orthoProjection));

	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		glNamedBufferSubData(bufferVertex, 0, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), cmd_list->VtxBuffer.Data);
		glNamedBufferSubData(bufferElements, 0, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), cmd_list->IdxBuffer.Data);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			const ImVec4 cr = pcmd->ClipRect;
			glScissor((int)cr.x, (int)(height - cr.w), (int)(cr.z - cr.x), (int)(cr.w - cr.y));
			glBindTextureUnit(0, (GLuint)(intptr_t)pcmd->TextureId);
			glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT,
				(void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)), (GLint)pcmd->VtxOffset);
		}
	}

	glScissor(0, 0, width, height);
}

void Gui::setupCallbacks(GLFWwindow* window) {
	glfwSetCursorPosCallback(
		window,
		[](auto* window, double x, double y)
		{
			ImGui::GetIO().MousePos = ImVec2((float)x, (float)y);
		}
	);

	glfwSetMouseButtonCallback(
		window,
		[](auto* window, int button, int action, int mods)
		{
			auto& io = ImGui::GetIO();
			const int idx = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : 1;
			io.MouseDown[idx] = action == GLFW_PRESS;
		}
	);
}
