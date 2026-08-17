# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Scope of work in this fork

This working copy is used to customize a **Planck** keyboard running the **default keymap**
(`keyboards/planck/keymaps/default/`), shared across two boards:

- `planck/rev6_drop` — STM32F303, `stm32-dfu` bootloader (primary/current board)
- `planck/rev5` — atmega32u4, `qmk-dfu` bootloader (older board, still in use)

Both boards build from the same `keymaps/default/keymap.c` + `config.h` + `rules.mk`; changes made
there apply to both unless guarded with board-specific `#ifdef`s. When editing, check whether a
change is meant for one specific revision or both.

**The startup chime is an intentional version marker, not a bug.** `STARTUP_SONG` in
`keyboards/planck/keymaps/default/config.h` is deliberately varied on each meaningful firmware
change so the jingle heard on boot identifies which build is flashed on a given board. Do not
"fix", simplify, or revert it back to the stock `PLANCK_SOUND` — changing the notes is itself the
point of a commit when versioning a change.

## Common commands

Build for a specific board/keymap:

```sh
qmk compile -kb planck/rev6_drop -km default
qmk compile -kb planck/rev5 -km default
```

(equivalently: `make planck/rev6_drop:default` / `make planck/rev5:default`)

Flash (puts the board in bootloader mode first — do not unplug mid-flash):

```sh
qmk flash -kb planck/rev6_drop -km default
qmk flash -kb planck/rev5 -km default
```

Lint a keyboard's config (checks `keyboard.json`/`rules.mk`/`config.h` for problems):

```sh
qmk lint -kb planck/rev6_drop -km default
qmk lint -kb planck/rev5 -km default
```

Set defaults so `-kb`/`-km` can be omitted:

```sh
qmk config user.keyboard=planck/rev6_drop
qmk config user.keymap=default
```

Run the C/unit test suite (for changes to `quantum/`, `tmk_core/`, drivers, etc. — not needed for
keymap-only edits):

```sh
make test:all
make test:<test_name>          # single test, e.g. make test:matrix
make list-tests                # list available test targets
```

Format C/Python sources and run the Python test suite (requires Docker; mirrors CI):

```sh
make format-core
make pytest
make format-and-pytest
```

## Architecture

QMK is a layered firmware project; a given build is assembled from several directories that each
contribute a piece:

- `tmk_core/` / `platforms/` — low-level MCU/platform startup, USB stack, protocol glue (chibios
  for the rev6_drop's STM32, LUFA/avr for the rev5's atmega32u4).
- `quantum/` — shared firmware logic used by all keyboards: keycode processing, layers, tap-hold,
  RGB/backlight, audio (`quantum/audio/`), OLED, etc.
- `keyboards/planck/` — hardware definition shared by all Planck revisions (matrix size, base
  layouts).
  - `keyboards/planck/rev6_drop/` and `keyboards/planck/rev5/` — per-revision hardware config
    (`keyboard.json` pins/processor/bootloader, `config.h`, `rules.mk`, matrix/board init code).
    `keyboard.json` is the modern source of truth for pins, features, and bootloader; older-style
    `config.h`/`rules.mk` settings layer on top of it.
  - `keyboards/planck/keymaps/default/` — the keymap actually in use: `keymap.c` (layers/keycodes,
    `process_record_user`, `layer_state_set_user`), `config.h` (tapping term, hold-on-other-key,
    `STARTUP_SONG`/layer-change songs), `rules.mk` (per-keymap feature toggles, e.g. `muse.c` when
    `AUDIO_ENABLE`).

Build resolution order for a `qmk compile -kb planck/rev6_drop -km default` is roughly:
`keyboards/planck/rev6_drop/{keyboard.json,config.h,rules.mk}` (board) →
`keyboards/planck/keymaps/default/{config.h,rules.mk}` (keymap, can override/extend board settings)
→ `keymap.c` compiled against whichever `LAYOUT_*` macro the board's `keyboard.json` aliases to
(`LAYOUT_planck_grid` → `LAYOUT_ortho_4x12` on both current boards).

The default keymap defines five layers (`_QWERTY`, `_COLEMAK`, `_LOWER`, `_RAISE`, `_ADJUST`), with
`_ADJUST` reached via `_LOWER`+`_RAISE` (tri-layer, wired up in `layer_state_set_user`). Audio
(`AUDIO_ENABLE`) is on for both boards, driving both the startup/version chime and per-layer switch
sounds.
