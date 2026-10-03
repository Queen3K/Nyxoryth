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
    HWND navigationButton = nullptr;
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

    // Scientific+ controls
    HWND scientificAdvancedXEdit = nullptr;
    HWND scientificAdvancedYEdit = nullptr;
    HWND scientificAdvancedResult = nullptr;

    // Statistics controls
    HWND statisticsDataEdit = nullptr;
    HWND statisticsResult = nullptr;

    // Fraction controls
    HWND fractionANumeratorEdit = nullptr;
    HWND fractionADenominatorEdit = nullptr;
    HWND fractionBNumeratorEdit = nullptr;
    HWND fractionBDenominatorEdit = nullptr;
    HWND fractionMixedWholeEdit = nullptr;
    HWND fractionMixedNumeratorEdit = nullptr;
    HWND fractionMixedDenominatorEdit = nullptr;
    HWND fractionResult = nullptr;

    // Equation solver controls
    HWND linearAEdit = nullptr;
    HWND linearBEdit = nullptr;
    HWND linearCEdit = nullptr;
    HWND quadraticAEdit = nullptr;
    HWND quadraticBEdit = nullptr;
    HWND quadraticCEdit = nullptr;
    HWND equationResult = nullptr;

    // Complex-number controls
    HWND complexReal1Edit = nullptr;
    HWND complexImag1Edit = nullptr;
    HWND complexReal2Edit = nullptr;
    HWND complexImag2Edit = nullptr;
    HWND complexResult = nullptr;

    // Unit-converter controls
    HWND unitCategoryCombo = nullptr;
    HWND unitFromCombo = nullptr;
    HWND unitToCombo = nullptr;
    HWND unitValueEdit = nullptr;
    HWND unitRateEdit = nullptr;
    HWND unitResult = nullptr;

    // Date/time controls
    HWND dateAEdit = nullptr;
    HWND dateBEdit = nullptr;
    HWND dateOffsetEdit = nullptr;
    HWND dateTimeEdit = nullptr;
    HWND unixTimestampEdit = nullptr;
    HWND durationSecondsEdit = nullptr;
    HWND dateResult = nullptr;

    // Advanced bit-tool controls
    HWND bitToolValueEdit = nullptr;
    HWND bitToolWordSizeCombo = nullptr;
    HWND bitToolSignedCombo = nullptr;
    HWND bitToolIndexEdit = nullptr;
    HWND bitToolResult = nullptr;
    HWND unicodeCodeEdit = nullptr;
    HWND unicodeResult = nullptr;
    HWND asciiList = nullptr;

    // Computer/storage-math controls
    HWND transferSizeEdit = nullptr;
    HWND transferSizeUnitCombo = nullptr;
    HWND transferRateEdit = nullptr;
    HWND transferRateUnitCombo = nullptr;
    HWND bitrateDurationEdit = nullptr;
    HWND bitrateRateEdit = nullptr;
    HWND resolutionWidthEdit = nullptr;
    HWND resolutionHeightEdit = nullptr;
    HWND resolutionDiagonalEdit = nullptr;
    HWND raidDriveCountEdit = nullptr;
    HWND raidDriveSizeEdit = nullptr;
    HWND raidModeCombo = nullptr;
    HWND computerMathResult = nullptr;

    HFONT expressionFont = nullptr;
    HFONT resultFont = nullptr;
    HFONT buttonFont = nullptr;
    HBRUSH themeBackgroundBrush = nullptr;
    HBRUSH themeEditBrush = nullptr;

    // Reusable background render surface. Keeping this alive between paints
    // avoids allocating a full-window bitmap for every animated GIF frame.
    HDC backgroundBufferDc = nullptr;
    HBITMAP backgroundBufferBitmap = nullptr;
    HGDIOBJ backgroundBufferPreviousBitmap = nullptr;
    int backgroundBufferWidth = 0;
    int backgroundBufferHeight = 0;
    bool backgroundFrameDirty = true;
    bool backgroundAnimationPaused = false;
    bool windowMinimized = false;

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
    std::vector<HWND> standardControls;
    std::vector<HWND> scientificControls;
    std::vector<HWND> tipControls;
    std::vector<HWND> programmerControls;
    std::vector<HWND> scientificAdvancedControls;
    std::vector<HWND> statisticsControls;
    std::vector<HWND> fractionControls;
    std::vector<HWND> equationControls;
    std::vector<HWND> complexControls;
    std::vector<HWND> unitControls;
    std::vector<HWND> dateControls;
    std::vector<HWND> bitToolsControls;
    std::vector<HWND> computerMathControls;
    std::vector<HWND> settingsControls;
    std::vector<HWND> allButtons;

    bool waitingForOperand = false;
    bool justEvaluated = false;
    bool errorState = false;
    bool historyVisible = false;
    bool scientificMode = false;
    bool tipMode = false;
    bool programmerMode = false;
    bool scientificAdvancedMode = false;
    bool statisticsMode = false;
    bool fractionMode = false;
    bool equationMode = false;
    bool complexMode = false;
    bool unitMode = false;
    bool dateMode = false;
    bool bitToolsMode = false;
    bool computerMathMode = false;
    bool settingsMode = false;
    bool degreeMode = true;
    bool darkTheme = true;
    bool backgroundCover = true;
    bool alwaysOnTop = false;
    int backgroundOverlayPercent = 45;
    int savedWindowX = CW_USEDEFAULT;
    int savedWindowY = CW_USEDEFAULT;
    int savedWindowWidth = 500;
    int savedWindowHeight = 670;

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
    void CreateScientificAdvancedControls();
    void CreateStatisticsControls();
    void CreateFractionControls();
    void CreateEquationControls();
    void CreateComplexControls();
    void CreateUnitControls();
    void CreateDateControls();
    void CreateBitToolsControls();
    void CreateComputerMathControls();
    void CreateSettingsControls();
    void RegisterButton(HWND button);
    void DrawOwnerButton(const DRAWITEMSTRUCT* drawItem);
    void HandleButton(int id);
    void HandleCharacter(wchar_t ch);
    bool HandleGlobalKey(const MSG& message);
    void FocusPrimaryControlForCurrentScreen();
    void UpdateDisplay();
    void UpdateExpressionForCurrentInput();
    void UpdateWindowLayout();
    void UpdateModeText();
    void ShowNavigationMenu();
    void ActivateScreen(int commandId);

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
    void MarkBackgroundDirty();
    bool EnsureBackgroundBuffer(HDC targetDc, int width, int height);
    void ReleaseBackgroundBuffer();
    void StartBackgroundTimer();
    void StopBackgroundTimer();
    void AdvanceBackgroundFrame();
    unsigned int CurrentBackgroundFrameDelay() const;
    int RequiredClientWidth() const;
    int RequiredClientHeight() const;
    void EnsureMinimumWindowSize();

    void RunAdvancedScientific(int operation);
    void CalculateStatistics();
    void CalculateFraction(int operation);
    void SolveEquation(int operation);
    void CalculateComplex(int operation);
    void UpdateUnitChoices();
    void ConvertUnits();
    void RunDateTool(int operation);
    void RunBitTool(int operation);
    void LookupUnicodeCodepoint();
    void RunComputerMath(int operation);

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
