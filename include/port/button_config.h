#ifndef PORT_BUTTON_CONFIG_H
#define PORT_BUTTON_CONFIG_H

/// Load button mappings from buttons.ini (next to the executable).
/// Should be called after Setup_Default_Game_Option() so save_w is initialized.
void ButtonConfig_Load();

/// Save current button mappings to buttons.ini (next to the executable).
/// Should be called whenever Save_Game_Data() runs.
void ButtonConfig_Save();

#endif
