#include "Gui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <imgui/imgui.h>

static int translate_keycode(int key, int scancode) {
	if (key >= GLFW_KEY_KP_0 && key <= GLFW_KEY_KP_EQUAL) return key;
	const char* key_name = glfwGetKeyName(key, scancode);
	if (key_name && key_name[0] != 0 && key_name[1] == 0)
	{
		const char char_names[] = "`-=[]\\,;\'./";
		const int char_keys[] = { GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_MINUS, GLFW_KEY_EQUAL, GLFW_KEY_LEFT_BRACKET, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_COMMA, GLFW_KEY_SEMICOLON, GLFW_KEY_APOSTROPHE, GLFW_KEY_PERIOD, GLFW_KEY_SLASH, 0 };
		IM_ASSERT(IM_ARRAYSIZE(char_names) == IM_ARRAYSIZE(char_keys));
		if (key_name[0] >= '0' && key_name[0] <= '9') { key = GLFW_KEY_0 + (key_name[0] - '0'); }
		else if (key_name[0] >= 'A' && key_name[0] <= 'Z') { key = GLFW_KEY_A + (key_name[0] - 'A'); }
		else if (key_name[0] >= 'a' && key_name[0] <= 'z') { key = GLFW_KEY_A + (key_name[0] - 'a'); }
		else if (const char* p = strchr(char_names, key_name[0])) { key = char_keys[p - char_names]; }
	}
	return key;
}

static ImGuiKey get_imgui_key(int key) {
	switch (key)
	{
	case GLFW_KEY_TAB: return ImGuiKey_Tab;
	case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
	case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
	case GLFW_KEY_UP: return ImGuiKey_UpArrow;
	case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
	case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
	case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
	case GLFW_KEY_HOME: return ImGuiKey_Home;
	case GLFW_KEY_END: return ImGuiKey_End;
	case GLFW_KEY_INSERT: return ImGuiKey_Insert;
	case GLFW_KEY_DELETE: return ImGuiKey_Delete;
	case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
	case GLFW_KEY_SPACE: return ImGuiKey_Space;
	case GLFW_KEY_ENTER: return ImGuiKey_Enter;
	case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
	case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
	case GLFW_KEY_COMMA: return ImGuiKey_Comma;
	case GLFW_KEY_MINUS: return ImGuiKey_Minus;
	case GLFW_KEY_PERIOD: return ImGuiKey_Period;
	case GLFW_KEY_SLASH: return ImGuiKey_Slash;
	case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
	case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
	case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
	case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
	case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
	case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
	case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
	case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
	case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
	case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
	case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
	case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
	case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
	case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
	case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
	case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
	case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
	case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
	case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
	case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
	case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
	case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
	case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
	case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
	case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
	case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
	case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
	case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
	case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
	case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
	case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
	case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
	case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
	case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
	case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
	case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
	case GLFW_KEY_MENU: return ImGuiKey_Menu;
	case GLFW_KEY_0: return ImGuiKey_0;
	case GLFW_KEY_1: return ImGuiKey_1;
	case GLFW_KEY_2: return ImGuiKey_2;
	case GLFW_KEY_3: return ImGuiKey_3;
	case GLFW_KEY_4: return ImGuiKey_4;
	case GLFW_KEY_5: return ImGuiKey_5;
	case GLFW_KEY_6: return ImGuiKey_6;
	case GLFW_KEY_7: return ImGuiKey_7;
	case GLFW_KEY_8: return ImGuiKey_8;
	case GLFW_KEY_9: return ImGuiKey_9;
	case GLFW_KEY_A: return ImGuiKey_A;
	case GLFW_KEY_B: return ImGuiKey_B;
	case GLFW_KEY_C: return ImGuiKey_C;
	case GLFW_KEY_D: return ImGuiKey_D;
	case GLFW_KEY_E: return ImGuiKey_E;
	case GLFW_KEY_F: return ImGuiKey_F;
	case GLFW_KEY_G: return ImGuiKey_G;
	case GLFW_KEY_H: return ImGuiKey_H;
	case GLFW_KEY_I: return ImGuiKey_I;
	case GLFW_KEY_J: return ImGuiKey_J;
	case GLFW_KEY_K: return ImGuiKey_K;
	case GLFW_KEY_L: return ImGuiKey_L;
	case GLFW_KEY_M: return ImGuiKey_M;
	case GLFW_KEY_N: return ImGuiKey_N;
	case GLFW_KEY_O: return ImGuiKey_O;
	case GLFW_KEY_P: return ImGuiKey_P;
	case GLFW_KEY_Q: return ImGuiKey_Q;
	case GLFW_KEY_R: return ImGuiKey_R;
	case GLFW_KEY_S: return ImGuiKey_S;
	case GLFW_KEY_T: return ImGuiKey_T;
	case GLFW_KEY_U: return ImGuiKey_U;
	case GLFW_KEY_V: return ImGuiKey_V;
	case GLFW_KEY_W: return ImGuiKey_W;
	case GLFW_KEY_X: return ImGuiKey_X;
	case GLFW_KEY_Y: return ImGuiKey_Y;
	case GLFW_KEY_Z: return ImGuiKey_Z;
	case GLFW_KEY_F1: return ImGuiKey_F1;
	case GLFW_KEY_F2: return ImGuiKey_F2;
	case GLFW_KEY_F3: return ImGuiKey_F3;
	case GLFW_KEY_F4: return ImGuiKey_F4;
	case GLFW_KEY_F5: return ImGuiKey_F5;
	case GLFW_KEY_F6: return ImGuiKey_F6;
	case GLFW_KEY_F7: return ImGuiKey_F7;
	case GLFW_KEY_F8: return ImGuiKey_F8;
	case GLFW_KEY_F9: return ImGuiKey_F9;
	case GLFW_KEY_F10: return ImGuiKey_F10;
	case GLFW_KEY_F11: return ImGuiKey_F11;
	case GLFW_KEY_F12: return ImGuiKey_F12;
	case GLFW_KEY_F13: return ImGuiKey_F13;
	case GLFW_KEY_F14: return ImGuiKey_F14;
	case GLFW_KEY_F15: return ImGuiKey_F15;
	case GLFW_KEY_F16: return ImGuiKey_F16;
	case GLFW_KEY_F17: return ImGuiKey_F17;
	case GLFW_KEY_F18: return ImGuiKey_F18;
	case GLFW_KEY_F19: return ImGuiKey_F19;
	case GLFW_KEY_F20: return ImGuiKey_F20;
	case GLFW_KEY_F21: return ImGuiKey_F21;
	case GLFW_KEY_F22: return ImGuiKey_F22;
	case GLFW_KEY_F23: return ImGuiKey_F23;
	case GLFW_KEY_F24: return ImGuiKey_F24;
	default: return ImGuiKey_None;
	}
}

Gui::Gui() {}

void Gui::setup() {
	guiShader.setup("shaders/imgui/vertex.glsl", "shaders/imgui/fragment.glsl");
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

	// Core::GL::UniformBuffer<glm::mat4> perFrameUbo;
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

	Core::GL::TextureOptions opts;
	opts.format = GL_RGBA;
	opts.internalFormat = GL_RGBA8;
	texFont.setup(pixels, width, height, opts);
	texFont.name = "Texture";

	io.Fonts->TexID = (ImTextureID)(intptr_t)texFont.getId();
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
	destroy();
}

void Gui::destroy() {
	ImGui::DestroyContext();
}

void Gui::draw(int width, int height) {
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

	for (int n = 0; n < draw_data->CmdListsCount; n++) {
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		glNamedBufferSubData(bufferVertex, 0, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), cmd_list->VtxBuffer.Data);
		glNamedBufferSubData(bufferElements, 0, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), cmd_list->IdxBuffer.Data);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
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

void Gui::setCursorPosCallback(GLFWwindow* window, double x, double y) {
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent((float)x, (float)y);
}

void Gui::setMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	auto& io = ImGui::GetIO();
	const int idx = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : 1;
	io.AddMouseButtonEvent(idx, action == GLFW_PRESS);
}

void Gui::setScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent((float)xOffset, (float)yOffset);
}

void Gui::setCharCallback(GLFWwindow* window, unsigned int c) {
	auto& io = ImGui::GetIO();
	io.AddInputCharacter(c);
}

void Gui::setKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

	auto& io = ImGui::GetIO();
	io.AddKeyEvent(ImGuiMod_Ctrl, (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS));
	io.AddKeyEvent(ImGuiMod_Shift, (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS));
	io.AddKeyEvent(ImGuiMod_Alt, (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS));
	io.AddKeyEvent(ImGuiMod_Super, (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS));

	int keycode = translate_keycode(key, scancode);

	ImGuiKey imgui_key = get_imgui_key(keycode);
	io.AddKeyEvent(imgui_key, action == GLFW_PRESS);
	io.SetKeyEventNativeData(imgui_key, keycode, scancode);
}