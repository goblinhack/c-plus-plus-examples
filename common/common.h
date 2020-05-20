// Black:   \u001b[30m
// Red:     \u001b[31m
// Green:   \u001b[32m
// Yellow:  \u001b[33m
// Blue:    \u001b[34m
// Magenta: \u001b[35m
// Cyan:    \u001b[36m
// White:   \u001b[37m
// Reset:   \u001b[0m
#define SUCCESS(x)  std::cout << "\033[0;35mSUCCESS: " << x << "\033[0m" << std::endl
#define FAILED(x) std::cout << "\033[0;34mFAILED: " << x << "\033[0m" << std::endl
#define DOC(x) std::cout << "\n\033[31;1;4m" << x << "\033[0m" << std::endl
