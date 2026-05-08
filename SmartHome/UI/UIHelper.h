#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

using namespace std;

class UIHelper {
public:
    // ══════════════════════════════════════════════════
    //  ANSI Color Codes - Premium Palette
    // ══════════════════════════════════════════════════
    static inline const string RESET      = "\033[0m";
    static inline const string BOLD       = "\033[1m";
    static inline const string DIM        = "\033[2m";
    static inline const string ITALIC     = "\033[3m";
    static inline const string UNDERLINE  = "\033[4m";

    // Primary Colors
    static inline const string RED        = "\033[38;5;196m";
    static inline const string GREEN      = "\033[38;5;46m";
    static inline const string YELLOW     = "\033[38;5;220m";
    static inline const string BLUE       = "\033[38;5;39m";
    static inline const string CYAN       = "\033[38;5;51m";
    static inline const string MAGENTA    = "\033[38;5;201m";
    static inline const string WHITE      = "\033[38;5;255m";
    static inline const string ORANGE     = "\033[38;5;208m";

    // Accent Colors  
    static inline const string TEAL       = "\033[38;5;43m";
    static inline const string GOLD       = "\033[38;5;178m";
    static inline const string LIME       = "\033[38;5;118m";
    static inline const string PINK       = "\033[38;5;213m";
    static inline const string GRAY       = "\033[38;5;245m";
    static inline const string DARK_GRAY  = "\033[38;5;238m";

    // Background Colors
    static inline const string BG_DARK    = "\033[48;5;234m";
    static inline const string BG_BLUE    = "\033[48;5;24m";
    static inline const string BG_GREEN   = "\033[48;5;22m";
    static inline const string BG_RED     = "\033[48;5;52m";
    static inline const string BG_GOLD    = "\033[48;5;58m";

    // ══════════════════════════════════════════════════
    //  Unicode Box Drawing Characters
    // ══════════════════════════════════════════════════
    static inline const string BOX_TL     = "\xE2\x95\x94"; // ╔
    static inline const string BOX_TR     = "\xE2\x95\x97"; // ╗
    static inline const string BOX_BL     = "\xE2\x95\x9A"; // ╚
    static inline const string BOX_BR     = "\xE2\x95\x9D"; // ╝
    static inline const string BOX_H      = "\xE2\x95\x90"; // ═
    static inline const string BOX_V      = "\xE2\x95\x91"; // ║
    static inline const string BOX_LT     = "\xE2\x95\xA0"; // ╠
    static inline const string BOX_RT     = "\xE2\x95\xA3"; // ╣
    static inline const string BOX_HT     = "\xE2\x94\x80"; // ─ (thin)
    static inline const string BOX_VT     = "\xE2\x94\x82"; // │ (thin)

    // Icons (Narrow - 1 display column each)
    static inline const string ICON_CHECK = "\xE2\x9C\x93";      // ✓
    static inline const string ICON_CROSS = "\xE2\x9C\x97";      // ✗
    static inline const string ICON_ARROW = "\xE2\x96\xB6";      // ▶
    static inline const string ICON_DOT   = "\xE2\x97\x8F";      // ●
    static inline const string ICON_STAR  = "\xE2\x98\x85";      // ★
    static inline const string ICON_GEAR  = "\xE2\x9A\x99";      // ⚙
    static inline const string ICON_WARN  = "\xE2\x9A\xA0";      // ⚠
    static inline const string ICON_INFO  = "\xE2\x84\xB9";      // ℹ
    static inline const string ICON_POWER = "\xE2\x8F\xBB";      // ⏻
    static inline const string ICON_SNOW  = "\xE2\x9D\x84";      // ❄
    static inline const string ICON_PLUS  = "+";

    // Icons (Wide - 2 display columns each)
    static inline const string ICON_HOME  = "\xF0\x9F\x8F\xA0"; // 🏠
    static inline const string ICON_BULB  = "\xF0\x9F\x92\xA1"; // 💡
    static inline const string ICON_LOCK  = "\xF0\x9F\x94\x92"; // 🔒
    static inline const string ICON_DOOR  = "\xF0\x9F\x9A\xAA"; // 🚪

    // Narrow alternatives for use inside box-bordered headers
    static inline const string ICON_ROOM  = "\xE2\x96\xA3";     // ▣ (narrow room icon)

    static void EnableUTF8() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        // Enable Virtual Terminal Processing for ANSI colors
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    // ══════════════════════════════════════════════════
    //  Display Width Calculation (handles UTF-8 + ANSI)
    // ══════════════════════════════════════════════════
    static int DisplayWidth(const string& text) {
        int width = 0;
        bool inEscape = false;
        size_t i = 0;
        while (i < text.size()) {
            unsigned char c = text[i];
            if (c == '\033') {
                inEscape = true;
                i++;
            } else if (inEscape) {
                if (c == 'm') inEscape = false;
                i++;
            } else if (c >= 0xF0) {
                // 4-byte UTF-8 (supplementary plane) = wide char = 2 columns
                width += 2;
                i += 4;
            } else if (c >= 0xE0) {
                // 3-byte UTF-8 (BMP) = narrow char = 1 column
                width += 1;
                i += 3;
            } else if (c >= 0xC0) {
                // 2-byte UTF-8
                width += 1;
                i += 2;
            } else {
                width += 1;
                i++;
            }
        }
        return width;
    }

    static string PadRight(const string& text, int targetWidth) {
        int currentWidth = DisplayWidth(text);
        int padding = targetWidth - currentWidth;
        if (padding <= 0) return text;
        return text + string(padding, ' ');
    }

    static string CenterPad(const string& text, int targetWidth) {
        int currentWidth = DisplayWidth(text);
        int totalPad = targetWidth - currentWidth;
        if (totalPad <= 0) return text;
        int leftPad = totalPad / 2;
        int rightPad = totalPad - leftPad;
        return string(leftPad, ' ') + text + string(rightPad, ' ');
    }

    // ══════════════════════════════════════════════════
    //  String Utilities
    // ══════════════════════════════════════════════════
    static string RepeatStr(const string& str, int count) {
        string result;
        for (int i = 0; i < count; i++) result += str;
        return result;
    }

    // ══════════════════════════════════════════════════
    //  Box Drawing Utilities
    // ══════════════════════════════════════════════════
    static void DrawBoxTop(int width, const string& color = "") {
        cout << (color.empty() ? CYAN : color)
             << BOX_TL << RepeatStr(BOX_H, width) << BOX_TR
             << RESET << "\n";
    }

    static void DrawBoxBottom(int width, const string& color = "") {
        cout << (color.empty() ? CYAN : color)
             << BOX_BL << RepeatStr(BOX_H, width) << BOX_BR
             << RESET << "\n";
    }

    static void DrawBoxMiddle(int width, const string& color = "") {
        cout << (color.empty() ? CYAN : color)
             << BOX_LT << RepeatStr(BOX_H, width) << BOX_RT
             << RESET << "\n";
    }

    // ══════════════════════════════════════════════════
    //  Screen Headers
    // ══════════════════════════════════════════════════
    static void DrawScreenHeader(const string& title, const string& icon = "") {
        const int WIDTH = 52;
        cout << "\n";
        DrawBoxTop(WIDTH, CYAN);

        // Build the display text (icon + title)
        string displayText = icon.empty() ? title : (icon + "  " + title);
        string centered = CenterPad(displayText, WIDTH);

        cout << CYAN << BOX_V << RESET
             << BOLD << CYAN << centered
             << RESET << CYAN << BOX_V << RESET << "\n";

        DrawBoxBottom(WIDTH, CYAN);
    }

    static void DrawSubHeader(const string& title) {
        const int WIDTH = 52;
        cout << DARK_GRAY << "  " << RepeatStr(BOX_HT, WIDTH - 2) << RESET << "\n";
        cout << BOLD << TEAL << "  " << ICON_ARROW << "  " << title << RESET << "\n";
        cout << DARK_GRAY << "  " << RepeatStr(BOX_HT, WIDTH - 2) << RESET << "\n";
    }

    // ══════════════════════════════════════════════════
    //  Menu Drawing
    // ══════════════════════════════════════════════════
    static void DrawMenuItem(int number, const string& text, const string& icon = "") {
        cout << "   " << BLUE << "[" << BOLD << WHITE << number << RESET << BLUE << "]"
             << RESET << " ";
        if (!icon.empty()) cout << icon << "  ";
        cout << WHITE << text << RESET << "\n";
    }

    static void DrawMenuDivider() {
        cout << DARK_GRAY << "  " << RepeatStr(BOX_HT, 50) << RESET << "\n";
    }

    // ══════════════════════════════════════════════════
    //  Input Helpers
    // ══════════════════════════════════════════════════
    static string ReadString(const string& message) {
        string input;
        cout << "\n  " << YELLOW << ICON_ARROW << " " << message << RESET;
        getline(cin >> ws, input);
        return input;
    }

    static int ReadInt(const string& message) {
        int input;
        cout << "\n  " << YELLOW << ICON_ARROW << " " << message << RESET;
        cin >> input;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            PrintError("Invalid input. Please enter a valid number.");
            cout << "  " << YELLOW << ICON_ARROW << " " << message << RESET;
            cin >> input;
        }
        return input;
    }

    // ══════════════════════════════════════════════════
    //  Status Messages
    // ══════════════════════════════════════════════════
    static void PrintError(const string& msg) {
        cout << "\n  " << BG_RED << RED << BOLD << " " << ICON_CROSS << " ERROR " << RESET
             << " " << RED << msg << RESET << "\n";
    }

    static void PrintSuccess(const string& msg) {
        cout << "\n  " << BG_GREEN << GREEN << BOLD << " " << ICON_CHECK << " SUCCESS " << RESET
             << " " << GREEN << msg << RESET << "\n";
    }

    static void PrintWarning(const string& msg) {
        cout << "\n  " << BG_GOLD << YELLOW << BOLD << " " << ICON_WARN << " WARNING " << RESET
             << " " << YELLOW << msg << RESET << "\n";
    }

    static void PrintInfo(const string& msg) {
        cout << "\n  " << BLUE << ICON_INFO << " " << RESET << GRAY << msg << RESET << "\n";
    }

    // ══════════════════════════════════════════════════
    //  Device State Display
    // ══════════════════════════════════════════════════
    static void PrintDeviceState(const string& state) {
        if (state == "On" || state == "Opened") {
            cout << GREEN << BOLD << ICON_DOT << " " << state << RESET;
        } else {
            cout << RED << ICON_DOT << " " << state << RESET;
        }
    }

    static string GetDeviceIcon(const string& type) {
        if (type == "Light") return ICON_BULB;
        if (type == "AC") return ICON_SNOW;
        if (type == "Lock") return ICON_LOCK;
        return ICON_GEAR;
    }

    // ══════════════════════════════════════════════════
    //  Progress Bar (visual capacity indicator)
    // ══════════════════════════════════════════════════
    static void PrintCapacityBar(int current, int max, int barWidth = 20) {
        float ratio = (max > 0) ? (float)current / max : 0;
        int filled = (int)(ratio * barWidth);
        
        string color;
        if (ratio < 0.5f) color = GREEN;
        else if (ratio < 0.8f) color = YELLOW;
        else color = RED;

        cout << GRAY << "[" << RESET;
        for (int i = 0; i < barWidth; i++) {
            if (i < filled) cout << color << "\xE2\x96\x88" << RESET; // █
            else cout << DARK_GRAY << "\xE2\x96\x91" << RESET;       // ░
        }
        cout << GRAY << "] " << RESET << color << current << "/" << max << RESET;
    }

    // ══════════════════════════════════════════════════
    //  Table Drawing Utilities
    // ══════════════════════════════════════════════════
    static void DrawTableBorder(const string& left, const string& mid, const string& right,
                                 const string& fill, const int colWidths[], int numCols,
                                 const string& color = "") {
        string col = color.empty() ? CYAN : color;
        cout << "  " << col << left;
        for (int i = 0; i < numCols; i++) {
            cout << RepeatStr(fill, colWidths[i]);
            if (i < numCols - 1) cout << mid;
        }
        cout << right << RESET << "\n";
    }

    static void DrawTableRow(const string cells[], const string colors[], 
                              const int colWidths[], int numCols,
                              const string& borderColor = "") {
        string bCol = borderColor.empty() ? CYAN : borderColor;
        cout << "  " << bCol << BOX_V << RESET;
        for (int i = 0; i < numCols; i++) {
            string padded = PadRight(cells[i], colWidths[i] - 1); // -1 for leading space
            cout << " " << colors[i] << padded << RESET;
            if (i < numCols - 1)
                cout << GRAY << BOX_VT << RESET;
        }
        cout << bCol << BOX_V << RESET << "\n";
    }

    // ══════════════════════════════════════════════════
    //  Pause / Navigation
    // ══════════════════════════════════════════════════
    static void PauseScreen(const string& message = "Press Enter to continue...") {
        cout << "\n  " << DIM << GRAY << message << RESET;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        string dummy;
        getline(cin, dummy);
    }

    // ══════════════════════════════════════════════════
    //  Welcome Banner
    // ══════════════════════════════════════════════════
    static void DrawWelcomeBanner(const string& systemName) {
        const int W = 52;
        cout << "\n";
        
        DrawBoxTop(W, CYAN);

        // Empty line
        cout << CYAN << BOX_V << BG_BLUE << string(W, ' ') << RESET << CYAN << BOX_V << RESET << "\n";
        
        // System name centered (use narrow star icons instead of wide emojis)
        string titleContent = ICON_STAR + "  " + systemName + "  " + ICON_STAR;
        string centered = CenterPad(titleContent, W);
        cout << CYAN << BOX_V << BG_BLUE << BOLD << WHITE << centered
             << RESET << CYAN << BOX_V << RESET << "\n";

        // Subtitle
        string subtitle = "Smart Home Management System";
        string subCentered = CenterPad(subtitle, W);
        cout << CYAN << BOX_V << BG_BLUE << TEAL << subCentered
             << RESET << CYAN << BOX_V << RESET << "\n";

        // Empty line
        cout << CYAN << BOX_V << BG_BLUE << string(W, ' ') << RESET << CYAN << BOX_V << RESET << "\n";
        
        DrawBoxBottom(W, CYAN);
    }
};
