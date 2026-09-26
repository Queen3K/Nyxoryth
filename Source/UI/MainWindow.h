#pragma once
#include <windows.h>
#include <string>
#include <vector>

namespace Gdiplus
{
class Image;
}

namespace Nyxoryth
{

class MainWindow
{
public:
    bool Initialize(HINSTANCE instance, int showCommand);
    int Run();

private:
    HWND hwnd = nullptr;
    HWND modeLabel = nullptr;
    HWND expressionDisplay = nullptr;
    HWND resultDisplay = nullptr;
    HWND historyList = nullptr;
    HWND clearHistoryButton = nullptr;
    HWND modeButton = nullptr;
    HWND angleButton = nullptr;
    HWND tipButton = nullptr;
    HWND programmerButton = nullptr;
    HWND settingsButton = nullptr;
    HWND copyResultButton = nullptr;
    HWND copyHistoryButton = nullptr;
    HWND alwaysOnTopButton = nullptr;
    HWND resetSettingsButton = nullptr;
    HWND aboutButton = nullptr;

    // Settings / appearance controls
    HWND backgroundStatus = nullptr;
    HWND scaleModeButton = nullptr;
    HWND overlayButton = nullptr;

    // Tip calculator controls
    HWND tipBillEdit = nullptr;
    HWND tipPercentEdit = nullptr;
    HWND tipPeopleEdit = nullptr;
    HWND tipAmountResult = nullptr;
    HWND tipTotalResult = nullptr;
    HWND tipPerPersonResult = nullptr;

    // Programmer / computer tools controls
    HWND programmerValueEdit = nullptr;
    HWND programmerDecResult = nullptr;
    HWND programmerHexResult = nullptr;
    HWND programmerOctResult = nullptr;
    HWND programmerBinResult = nullptr;
    HWND bitwiseAEdit = nullptr;
    HWND bitwiseBEdit = nullptr;
    HWND bitwiseResult = nullptr;
    HWND storageBytesEdit = nullptr;
    HWND storageResult = nullptr;
    HWND cidrIpEdit = nullptr;
    HWND cidrPrefixEdit = nullptr;
    HWND cidrResult = nullptr;
    HWND charCodeEdit = nullptr;
    HWND charCodeResult = nullptr;

    HFONT expressionFont = nullptr;
    HFONT resultFont = nullptr;
    HFONT buttonFont = nullptr;
    HBRUSH themeBackgroundBrush = nullptr;
    HBRUSH themeEditBrush = nullptr;

    ULONG_PTR gdiplusToken = 0;
    Gdiplus::Image* backgroundImage = nullptr;
    GUID backgroundFrameDimension{};
    std::vector<unsigned int> backgroundFrameDelays;
    unsigned int backgroundFrameCount = 0;
    unsigned int backgroundFrameIndex = 0;

    double accumulator = 0.0;
    wchar_t pendingOperator = 0;
    std::wstring currentInput = L"0";
    std::wstring expression;
    std::vector<std::wstring> history;
    std::vector<HWND> scientificControls;
    std::vector<HWND> tipControls;
    std::vector<HWND> programmerControls;
    std::vector<HWND> settingsControls;
    std::vector<HWND> allButtons;

    bool waitingForOperand = false;
    bool justEvaluated = false;
    bool errorState = false;
    bool historyVisible = false;
    bool scientificMode = false;
    bool tipMode = false;
    bool programmerMode = false;
    bool settingsMode = false;
    bool degreeMode = true;
    bool darkTheme = true;
    bool backgroundCover = true;
    bool alwaysOnTop = false;
    int backgroundOverlayPercent = 45;
    int savedWindowX = CW_USEDEFAULT;
    int savedWindowY = CW_USEDEFAULT;

    std::wstring settingsPath;
    std::wstring backgroundPath;
    std::wstring lastMode = L"Standard";

    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK ButtonSubclassProc(
        HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

    void CreateControls();
    void CreateScientificControls();
    void CreateTipControls();
    void CreateProgrammerControls();
    void CreateSettingsControls();
    void RegisterButton(HWND button);
    void DrawOwnerButton(const DRAWITEMSTRUCT* drawItem);
    void HandleButton(int id);
    void HandleCharacter(wchar_t ch);
    void UpdateDisplay();
    void UpdateExpressionForCurrentInput();
    void UpdateWindowLayout();
    void UpdateModeText();

    void InputDigit(wchar_t digit);
    void InputDecimal();
    void SetOperator(wchar_t op);
    void Evaluate();
    void ClearAll();
    void ClearEntry();
    void Backspace();
    void ToggleSign();
    void Percent();
    void SquareRoot();
    void Square();
    void Reciprocal();

    void ToggleScientificMode();
    void ToggleTipMode();
    void ToggleProgrammerMode();
    void ToggleSettingsMode();
    void ToggleAngleMode();

    void InitializeSettingsPath();
    void LoadSettings();
    void SaveSettings();
    void ApplyTheme();
    void ChooseBackground();
    void ClearBackground();
    bool LoadBackgroundImage(const std::wstring& path);
    void ReleaseBackgroundImage();
    void ToggleBackgroundScaleMode();
    void CycleBackgroundOverlay();
    void UpdateSettingsText();
    void CopyResult();
    void CopySelectedHistory();
    bool CopyTextToClipboard(const std::wstring& text);
    void ToggleAlwaysOnTop();
    void ResetSettings();
    void ShowAbout();
    void SaveWindowPosition();
    void ApplyStartupMode();
    void PaintBackground(HDC hdc);
    void AdvanceBackgroundFrame();
    unsigned int CurrentBackgroundFrameDelay() const;

    void CalculateTip();
    void SetTipPreset(double percent);

    void ConvertProgrammerValue();
    void ApplyBitwiseOperation(int operation);
    void ConvertStorage();
    void CalculateCidr();
    void ConvertCharacterCode();
    void Sine();
    void Cosine();
    void Tangent();
    void ArcSine();
    void ArcCosine();
    void ArcTangent();
    void Log10();
    void NaturalLog();
    void Factorial();
    void TenPower();
    void EPower();
    void AbsoluteValue();
    void InsertPi();
    void InsertE();
    void ApplyScientificUnary(double result, const std::wstring& label);

    bool ApplyBinary(double left, double right, wchar_t op, double& result);
    double CurrentValue() const;
    std::wstring FormatNumber(double value) const;
    std::wstring OperatorText(wchar_t op) const;
    void SetError(const std::wstring& message);

    void AddHistory(const std::wstring& item);
    void ToggleHistory();
    void ClearHistory();
    void ApplyFonts();
};

}
