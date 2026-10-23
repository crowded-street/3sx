# Config

3SX supports a config file which allows you to change several useful options.

Config location:
- **Windows**: `C:\Users\<username>\AppData\Roaming\CrowdedStreet\3SX\config`
- **Linux**: `~/.local/share/CrowdedStreet/3SX/config`
- **macOS**: `~/Library/Application Support/CrowdedStreet/3SX/config`

## Options

### `fullscreen`

Whether the game should start in fullscreen mode.

### `window-width` / `window-height`

Window dimensions to use when `fullscreen` is set to `false`. The minimum window size is 298x224.

### `scale-mode`

The way the internal 384x224 buffer is scaled.

#### `nearest` (default)

Produces sharp pixels at the cost of sizing consistency.

#### `integer`

Produces a pixel-perfect image, but requires a 4K display.

> [!WARNING] 
> The image is gonna be cropped if your display resolution is smaller than 2688x2016.

#### `vertical-integer`

Scales the image height by the largest whole-number multiple of 224 that fits the window. The width follows `aspect-ratio`. If that width doesn't fit the window, the multiple is lowered until it does. Produces a pixel-perfect image vertically.

#### `square-pixels`

The internal buffer is scaled up by an integer (whole number) factor. Use this if you play on a CRT

### `scanlines`

Defines the strength of the scanline filter (from `0` to `100`). `0` means the filter is disabled.

### `aspect-ratio`

The display aspect ratio, written as a decimal number with six decimal places.

Useful for systems like Batocera or Recalbox, which use small or very wide resolutions. Examples:
- `1.333333`: 298x224
- `1.428580`: 320x224
- `1.428580`: 1920x1344 (with `vertical-integer` and `fullscreen` on a 2560x1440 display, produces a pixel-perfect image)
- `8.571429`: 1920x224

> [!NOTE]
> This setting only applies to the `nearest` and `vertical-integer` scale modes.

### `draw-players-above-hud`

Allow characters to render in front of the top HUD similar to Street Fighter IV.

> [!NOTE]
> With this setting on these stage decorations will be disabled to prevent overlapping with HUD:
> - Chun-Li's stage: Bamboo stick on the right
> - Makoto's stage: Tree on the right
> - Yang's stage: Rain overlay

### `arcade-balance`

Enables arcade balance instead of PS2 balance. 

> [!IMPORTANT]
> Requires `sfiii3nr1.zip` to be present in [`resources`](resources.md) directory.

> [!WARNING]
> Arcade balance is a work-in-progress. Expect bugs, crashes and instability when enabling this feature.
