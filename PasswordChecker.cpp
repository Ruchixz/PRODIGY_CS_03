#include <windows.h>
#include <string>
#include <cctype>

// Function to validate the password
std::string validatePassword(const std::string &password) {
    if (password.length() < 6) return "Password must be at least 6 characters long.";
    if (password.length() > 20) return "Password must not exceed 20 characters.";

    bool containsLowerCase = false;
    bool containsUpperCase = false;
    bool containsDigit = false;
    bool containsSpecialCharacter = false;

    for (char ch : password) {
        if (std::isupper(ch)) containsUpperCase = true;
        if (std::islower(ch)) containsLowerCase = true;
        if (std::isdigit(ch)) containsDigit = true;
        if (!std::isalnum(ch) && !std::isspace(ch)) containsSpecialCharacter = true;
    }

    if (!containsLowerCase) return "Password must contain at least one lowercase letter.";
    if (!containsUpperCase) return "Password must contain at least one uppercase letter.";
    if (!containsDigit) return "Password must contain at least one digit.";
    if (!containsSpecialCharacter) return "Password must contain at least one special character.";

    return "Valid";
}

// Callback function to process events
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hPasswordInput, hValidateButton, hFeedback, hShowPasswordButton;
    static bool isPasswordVisible = false;

    switch (message) {
        case WM_CREATE: {
            // Create input box for password
            CreateWindow("STATIC", "Enter Password:", WS_VISIBLE | WS_CHILD, 20, 20, 150, 20, hwnd, NULL, NULL, NULL);
            hPasswordInput = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD, 
                                          20, 50, 200, 20, hwnd, NULL, NULL, NULL);

            // Create validate button
            hValidateButton = CreateWindow("BUTTON", "Validate Password", WS_VISIBLE | WS_CHILD, 20, 90, 150, 30, hwnd, (HMENU)1, NULL, NULL);

            // Create show password button
            hShowPasswordButton = CreateWindow("BUTTON", "Show Password", WS_VISIBLE | WS_CHILD, 180, 90, 120, 30, hwnd, (HMENU)2, NULL, NULL);

            // Create feedback display
            hFeedback = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD, 20, 140, 300, 20, hwnd, NULL, NULL, NULL);
        }
        break;

        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) { // Validate button clicked
                char password[256];
                GetWindowText(hPasswordInput, password, 256);

                std::string result = validatePassword(password);

                if (result == "Valid") {
                    SetWindowText(hFeedback, "Password is valid!");
                } else {
                    SetWindowText(hFeedback, result.c_str());
                }
            } else if (LOWORD(wParam) == 2) { // Show Password button clicked
                isPasswordVisible = !isPasswordVisible;

                // Toggle password visibility
                SendMessage(hPasswordInput, EM_SETPASSWORDCHAR, isPasswordVisible ? 0 : '*', 0);
                InvalidateRect(hPasswordInput, NULL, TRUE); // Force refresh the input box

                // Update button text
                SetWindowText(hShowPasswordButton, isPasswordVisible ? "Hide Password" : "Show Password");
            }
        }
        break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "PasswordValidator";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Password Validator",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 350, 250,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
