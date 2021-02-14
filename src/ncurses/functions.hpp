#pragma once
#include "utils/Structs.hpp"
#include <string>

namespace memo {

const bool ENABLE = true;
const bool DISABLE = false;

namespace curses {

    class IWindow;

///
/// \brief Initializes the main screen. Needs to be called before calling other
/// functions.
///
void InitCurses();

///
/// \brief Finalizes and closes the ncurses screen. This operation should be called
/// at the very end.
///
void CloseCurses();

///
/// \brief Redraw everything on the screen.
///
void Refresh();

///
/// \brief Modifies the visibility of the cursor on screen.
/// \param visible
///
void CursorVisible(bool visible);

///
/// \brief If enabled, text is being buffered as it's being typed. It becomes available
/// to the program once user types an interrupt or a control character such as <ENTER>.
/// If disabled, typed characters are immediately passed to the program without entering
/// an intermediate buffer (does not wait for a control character to be typed).
/// \param enable True/false. Whether or not to enable buffering.
///
void LineBuffering(bool enable);

///
/// \brief If enabled, characters are printed as they're being typed by the user.
/// If disabled, character input will not get printed out on the screen.
/// \param enable True/false. Whether or not to enable printing typed characters.
///
void PrintTypedInput(bool enable);

///
/// \brief If enabled, the main screen will accept function keys (F1-F12, arrow keys, ...).
/// \param enable True/false. Whether or not to enable special keys.
///
void KeyPad(bool enable);

///
/// \brief If enabled, the provided window will accept function keys (F1-F12,
/// arrow keys, ...).
/// \param enable True/false. Whether or not to enable special keys.
///
void KeyPad(const IWindow& window, bool enable);

///
/// \brief Reads the next input character typed by the user. The blocks the main thread
/// and waits for the user input.
/// \return Integer representation of the read character.
///
int ReadChar();

///
/// \brief Reads the next input character typed by the user in the given window. The
/// operation blocks the main thread and waits for the user input.
/// \return Integer representation of the read character.
///
int ReadChar(const IWindow& window);

///
/// \brief Reads the next input character typed by the user from the specified position
/// on the screen. The cursor is moved to the given location before the input is read.
/// The operation blocks the main thread and waits for the user input.
/// \return Integer representation of the read character.
///
int ReadCharAt(const Position& position);

///
/// \brief Reads the next input character typed by the user in the given window and position
/// on the screen. The cursor is moved to the given location before the input is read. The
/// operation blocks the main thread and waits for the user input.
/// \return Integer representation of the read character.
///
int ReadCharAt(const IWindow& window, const Position& position);

///
/// \brief Prints the given text at the specified position on the screen.
/// \param text Text to print
/// \param position Position on the screen.
/// \return curses error code
///
int PrintText(const std::string& text, const Position& position);

///
/// \brief Prints the given text at the specified position in the window.
/// \param text Text to print
/// \param window Window on which to print
/// \param position Position on the screen
/// \return curses error code
///
int PrintText(const std::string& text, const IWindow& window, const Position& position);

Size ScreenSize();

int ScreenWidth();

int ScreenHeight();

///
/// \brief Corrects the given size, so that if the width/height is less than one,
/// it is set to the screen width/height. This is how curses treats "invalid"
/// dimensions when drawing window borders.
/// \param size The size to be corrected.
/// \return The corrected size.
///
Size CorrectSize(const Size& size);

Border DefaultBorder();

}
}
