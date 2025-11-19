
#pragma once

namespace ansi {

// Reset
constexpr const char *reset        = "\e[0m";

// Styles
constexpr const char *bold         = "\e[1m";
constexpr const char *dim          = "\e[2m";
constexpr const char *italic       = "\e[3m";
constexpr const char *underline    = "\e[4m";
constexpr const char *blink        = "\e[5m";
constexpr const char *inverse      = "\e[7m";
constexpr const char *hidden       = "\e[8m";
constexpr const char *strikethrough = "\e[9m";

// Foreground colors
constexpr const char *black        = "\e[30m";
constexpr const char *red          = "\e[31m";
constexpr const char *green        = "\e[32m";
constexpr const char *yellow       = "\e[33m";
constexpr const char *blue         = "\e[34m";
constexpr const char *magenta      = "\e[35m";
constexpr const char *cyan         = "\e[36m";
constexpr const char *white        = "\e[37m";
constexpr const char *bright_black = "\e[90m";
constexpr const char *bright_red   = "\e[91m";
constexpr const char *bright_green = "\e[92m";
constexpr const char *bright_yellow = "\e[93m";
constexpr const char *bright_blue  = "\e[94m";
constexpr const char *bright_magenta = "\e[95m";
constexpr const char *bright_cyan  = "\e[96m";
constexpr const char *bright_white = "\e[97m";

// Background colors
constexpr const char *bg_black        = "\e[40m";
constexpr const char *bg_red          = "\e[41m";
constexpr const char *bg_green        = "\e[42m";
constexpr const char *bg_yellow       = "\e[43m";
constexpr const char *bg_blue         = "\e[44m";
constexpr const char *bg_magenta      = "\e[45m";
constexpr const char *bg_cyan         = "\e[46m";
constexpr const char *bg_white        = "\e[47m";
constexpr const char *bg_bright_black = "\e[100m";
constexpr const char *bg_bright_red   = "\e[101m";
constexpr const char *bg_bright_green = "\e[102m";
constexpr const char *bg_bright_yellow = "\e[103m";
constexpr const char *bg_bright_blue  = "\e[104m";
constexpr const char *bg_bright_magenta = "\e[105m";
constexpr const char *bg_bright_cyan  = "\e[106m";
constexpr const char *bg_bright_white = "\e[107m";

} // namespace ansi
