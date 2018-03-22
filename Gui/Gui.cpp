#include "Gui.h"

using namespace Client;

//[enc_string_enable /]
//[junk_enable /]

bool bIsGuiInitalize = false;
bool bIsGuiVisible = false;
WNDPROC WndProc_o = nullptr;

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

LRESULT WINAPI GUI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CGui::CGui() {}

CGui::~CGui()
{
	ImGui_ImplDX9_Shutdown();
}

void CGui::GUI_Init(IDirect3DDevice9 * pDevice)
{
	HWND hWindow = FindWindowA("Valve001", 0);

	ImGui_ImplDX9_Init(hWindow, pDevice);

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	io.IniFilename = GuiFile.c_str();

	ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Roboto-Regular.ttf",
		18.f, 0, io.Fonts->GetGlyphRangesCyrillic());

	//ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF( 
	//	Avalon_compressed_data , Avalon_compressed_size , 12.f );

	style.Alpha = 1.f;
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.FrameRounding = 3.0f;
	style.ItemInnerSpacing = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(600, 400);
	DefaultSheme();

	ImGui_ImplDX9_CreateDeviceObjects();

	WndProc_o = (WNDPROC)SetWindowLongA(hWindow, GWL_WNDPROC, (LONG)(LONG_PTR)GUI_WndProc);

	bIsGuiInitalize = true;
}

void CGui::GUI_Begin_Render()
{
	ImGui_ImplDX9_NewFrame();
}

void CGui::GUI_End_Render()
{
	ImGui::Render();
}

LRESULT WINAPI GUI_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool is_down = false;
	static bool is_clicked = false;
	static bool check_closed = false;

	if (GUI_KEY_DOWN(VK_INSERT))
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!GUI_KEY_DOWN(VK_INSERT) && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (!bIsGuiVisible && !is_clicked && check_closed)
	{
		string msg = "cl_mouseenable " + to_string(!bIsGuiVisible);
		Interfaces::Engine()->ClientCmd_Unrestricted2(msg.c_str());
		check_closed = false;
	}

	if (is_clicked)
	{
		bIsGuiVisible = !bIsGuiVisible;

		string msg = "cl_mouseenable " + to_string(!bIsGuiVisible);
		Interfaces::Engine()->ClientCmd_Unrestricted2(msg.c_str());

		if (!check_closed)
			check_closed = true;
	}

	if (bIsGuiVisible && ImGui_ImplDX9_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProcA(WndProc_o, hwnd, uMsg, wParam, lParam);
}

void CGui::GUI_Draw_Elements()
{
	if (!bIsGuiInitalize || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsActiveApp())
		return;

	g_pGui->GUI_Begin_Render();

	ImGui::GetIO().MouseDrawCursor = bIsGuiVisible;

	bool bOpenTimer = (bIsGuiVisible || (bC4Timer && iC4Timer));

	if (g_pEsp && Settings::Esp::esp_BombTimer && bOpenTimer)
	{
		if (GetKeyState(VK_DELETE))
		{
			ImVec2 OldMinSize = ImGui::GetStyle().WindowMinSize;

			ImGui::GetStyle().WindowMinSize = ImVec2(0.f, 0.f);

			ImGui::SetNextWindowSize(ImVec2(125.f, 30.f));

			if (ImGui::Begin("Bomb Timer", &bOpenTimer,
				ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{

				ImGui::Text(C4_TIMER_STRING, g_pEsp->fC4Timer);
				ImGui::End();

			}

			ImGui::GetStyle().WindowMinSize = OldMinSize;
		}
	}

	if (bIsGuiVisible)
	{
		int pX, pY;
		Interfaces::InputSystem()->GetCursorPosition(&pX, &pY);

		ImGui::GetIO().MousePos.x = (float)pX;
		ImGui::GetIO().MousePos.y = (float)pY;

		Client::OnRenderGUI();
	}

	if (g_pRadar)
		g_pRadar->OnRender();

	g_pGui->GUI_End_Render();
}


void CGui::DefaultSheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.11f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.00f, 0.00f, 0.02f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.12f, 0.11f, 0.21f, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.09f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.05f, 0.02f, 0.05f, 0.98f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.02f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.12f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.18f, 0.18f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.12f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.09f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.11f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.09f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.18f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.09f, 0.09f, 0.12f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.18f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.12f, 0.11f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.18f, 0.18f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.18f, 0.18f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.02f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	IMGUI_API bool ComboBoxArray(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	IMGUI_API bool TabLabels(const char **tabLabels, int tabSize, int &tabIndex, int *tabOrder)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		const ImVec2 itemSpacing = style.ItemSpacing;
		const ImVec4 color = style.Colors[ImGuiCol_Button];
		const ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
		const ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
		const ImVec4 colorText = style.Colors[ImGuiCol_Text];
		style.ItemSpacing.x = 1;
		style.ItemSpacing.y = 1;
		const ImVec4 colorSelectedTab = ImVec4(color.x, color.y, color.z, color.w*0.5f);
		const ImVec4 colorSelectedTabHovered = ImVec4(colorHover.x, colorHover.y, colorHover.z, colorHover.w*0.5f);
		const ImVec4 colorSelectedTabText = ImVec4(colorText.x*0.8f, colorText.y*0.8f, colorText.z*0.8f, colorText.w*0.8f);

		if (tabSize>0 && (tabIndex<0 || tabIndex >= tabSize))
		{
			if (!tabOrder)
			{
				tabIndex = 0;
			}
			else
			{
				tabIndex = -1;
			}
		}

		float windowWidth = 0.f, sumX = 0.f;
		windowWidth = ImGui::GetWindowWidth() - style.WindowPadding.x - (ImGui::GetScrollMaxY()>0 ? style.ScrollbarSize : 0.f);

		static int draggingTabIndex = -1; int draggingTabTargetIndex = -1;
		static ImVec2 draggingtabSize(0, 0);
		static ImVec2 draggingTabOffset(0, 0);

		const bool isMMBreleased = ImGui::IsMouseReleased(2);
		const bool isMouseDragging = ImGui::IsMouseDragging(0, 2.f);
		int justClosedTabIndex = -1, newtabIndex = tabIndex;

		bool selection_changed = false; bool noButtonDrawn = true;

		for (int j = 0, i; j < tabSize; j++)
		{
			i = tabOrder ? tabOrder[j] : j;
			if (i == -1) continue;

			if (sumX > 0.f)
			{
				sumX += style.ItemSpacing.x;
				sumX += ImGui::CalcTextSize(tabLabels[i]).x + 2.f*style.FramePadding.x;

				if (sumX>windowWidth)
				{
					sumX = 0.f;
				}
				else
				{
					ImGui::SameLine();
				}
			}

			if (i != tabIndex)
			{
				// Push the style
				style.Colors[ImGuiCol_Button] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonActive] = colorSelectedTab;
				style.Colors[ImGuiCol_ButtonHovered] = colorSelectedTabHovered;
				style.Colors[ImGuiCol_Text] = colorSelectedTabText;
			}
			// Draw the button
			ImGui::PushID(i);   // otherwise two tabs with the same name would clash.
			if (ImGui::Button(tabLabels[i], ImVec2(116.f, 50.f))) { selection_changed = (tabIndex != i); newtabIndex = i; }
			ImGui::PopID();
			if (i != tabIndex)
			{
				// Reset the style
				style.Colors[ImGuiCol_Button] = color;
				style.Colors[ImGuiCol_ButtonActive] = colorActive;
				style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				style.Colors[ImGuiCol_Text] = colorText;
			}
			noButtonDrawn = false;

			if (sumX == 0.f) sumX = style.WindowPadding.x + ImGui::GetItemRectSize().x; // First element of a line

			if (ImGui::IsItemHoveredRect())
			{
				if (tabOrder)
				{
					// tab reordering
					if (isMouseDragging)
					{
						if (draggingTabIndex == -1)
						{
							draggingTabIndex = j;
							draggingtabSize = ImGui::GetItemRectSize();
							const ImVec2& mp = ImGui::GetIO().MousePos;
							const ImVec2 draggingTabCursorPos = ImGui::GetCursorPos();
							draggingTabOffset = ImVec2(
								mp.x + draggingtabSize.x*0.5f - sumX + ImGui::GetScrollX(),
								mp.y + draggingtabSize.y*0.5f - draggingTabCursorPos.y + ImGui::GetScrollY()
							);

						}
					}
					else if (draggingTabIndex >= 0 && draggingTabIndex<tabSize && draggingTabIndex != j)
					{
						draggingTabTargetIndex = j; // For some odd reasons this seems to get called only when draggingTabIndex < i ! (Probably during mouse dragging ImGui owns the mouse someway and sometimes ImGui::IsItemHovered() is not getting called)
					}
				}
			}

		}

		tabIndex = newtabIndex;

		// Draw tab label while mouse drags it
		if (draggingTabIndex >= 0 && draggingTabIndex<tabSize)
		{
			const ImVec2& mp = ImGui::GetIO().MousePos;
			const ImVec2 wp = ImGui::GetWindowPos();
			ImVec2 start(wp.x + mp.x - draggingTabOffset.x - draggingtabSize.x*0.5f, wp.y + mp.y - draggingTabOffset.y - draggingtabSize.y*0.5f);
			const ImVec2 end(start.x + draggingtabSize.x, start.y + draggingtabSize.y);
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			const float draggedBtnAlpha = 0.65f;
			const ImVec4& btnColor = style.Colors[ImGuiCol_Button];
			drawList->AddRectFilled(start, end, ImColor(btnColor.x, btnColor.y, btnColor.z, btnColor.w*draggedBtnAlpha), style.FrameRounding);
			start.x += style.FramePadding.x; start.y += style.FramePadding.y;
			const ImVec4& txtColor = style.Colors[ImGuiCol_Text];
			drawList->AddText(start, ImColor(txtColor.x, txtColor.y, txtColor.z, txtColor.w*draggedBtnAlpha), tabLabels[tabOrder[draggingTabIndex]]);

			ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
		}

		// Drop tab label
		if (draggingTabTargetIndex != -1)
		{
			// swap draggingTabIndex and draggingTabTargetIndex in tabOrder
			const int tmp = tabOrder[draggingTabTargetIndex];
			tabOrder[draggingTabTargetIndex] = tabOrder[draggingTabIndex];
			tabOrder[draggingTabIndex] = tmp;
			//fprintf(stderr,"%d %d\n",draggingTabIndex,draggingTabTargetIndex);
			draggingTabTargetIndex = draggingTabIndex = -1;
		}

		// Reset draggingTabIndex if necessary
		if (!isMouseDragging) draggingTabIndex = -1;

		// Change selected tab when user closes the selected tab
		if (tabIndex == justClosedTabIndex && tabIndex >= 0)
		{
			tabIndex = -1;
			for (int j = 0, i; j < tabSize; j++)
			{
				i = tabOrder ? tabOrder[j] : j;
				if (i == -1)
				{
					continue;
				}
				tabIndex = i;
				break;
			}
		}

		// Restore the style
		style.Colors[ImGuiCol_Button] = color;
		style.Colors[ImGuiCol_ButtonActive] = colorActive;
		style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		style.Colors[ImGuiCol_Text] = colorText;
		style.ItemSpacing = itemSpacing;

		return selection_changed;
	}
}